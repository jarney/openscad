#include "NodeProgramGraphModel.hpp"

#include <QtNodes/internal/ConnectionIdHash.hpp>
#include <QtNodes/Definitions>

#include <QJsonArray>

#include <stack>
#include <stdexcept>

NodeProgramGraphModel::NodeProgramGraphModel(std::shared_ptr<NodeProgramModelRegistry> registry, NodeProgram & parent)
    : _registry(std::move(registry))
    , _nextNodeId{0}
    , _parent(parent)
{}

std::unordered_set<QtNodes::NodeId> NodeProgramGraphModel::allNodeIds() const
{
    std::unordered_set<QtNodes::NodeId> nodeIds;
    for_each(_models.begin(), _models.end(), [&nodeIds](const auto &p) { nodeIds.insert(p.first); });

    return nodeIds;
}

std::unordered_set<QtNodes::ConnectionId> NodeProgramGraphModel::allConnectionIds(QtNodes::NodeId const nodeId) const
{
    std::unordered_set<QtNodes::ConnectionId> result;

    std::copy_if(_connectivity.begin(),
                 _connectivity.end(),
                 std::inserter(result, std::end(result)),
                 [&nodeId](QtNodes::ConnectionId const &cid) {
                     return cid.inNodeId == nodeId || cid.outNodeId == nodeId;
                 });

    return result;
}

std::unordered_set<QtNodes::ConnectionId> NodeProgramGraphModel::connections(QtNodes::NodeId nodeId,
                                                                 QtNodes::PortType portType,
                                                                 QtNodes::PortIndex portIndex) const
{
    std::unordered_set<QtNodes::ConnectionId> result;

    std::copy_if(_connectivity.begin(),
                 _connectivity.end(),
                 std::inserter(result, std::end(result)),
                 [&portType, &portIndex, &nodeId](QtNodes::ConnectionId const &cid) {
                     return (getNodeId(portType, cid) == nodeId
                             && getPortIndex(portType, cid) == portIndex);
                 });

    return result;
}

bool NodeProgramGraphModel::connectionExists(QtNodes::ConnectionId const connectionId) const
{
    return (_connectivity.find(connectionId) != _connectivity.end());
}

QtNodes::NodeId NodeProgramGraphModel::addNode(QString const nodeType)
{
    std::unique_ptr<QtNodes::NodeDelegateModel> model = _registry->create(nodeType, *this);

    if (model) {
	return addNode(std::move(model));
    }

    return QtNodes::InvalidNodeId;
}

QtNodes::NodeId NodeProgramGraphModel::addNode(std::unique_ptr<QtNodes::NodeDelegateModel> model)
{
    QtNodes::NodeId newId = newNodeId();

    connect(model.get(),
            &QtNodes::NodeDelegateModel::dataUpdated,
            [newId, this](QtNodes::PortIndex const portIndex) {
                onOutPortDataUpdated(newId, portIndex);
            });

    connect(model.get(),
            &QtNodes::NodeDelegateModel::portsAboutToBeDeleted,
            this,
            [newId, this](QtNodes::PortType const portType, QtNodes::PortIndex const first, QtNodes::PortIndex const last) {
                portsAboutToBeDeleted(newId, portType, first, last);
            });

    connect(model.get(),
            &QtNodes::NodeDelegateModel::portsDeleted,
            this,
            &NodeProgramGraphModel::portsDeleted);

    connect(model.get(),
            &QtNodes::NodeDelegateModel::portsAboutToBeInserted,
            this,
            [newId, this](QtNodes::PortType const portType, QtNodes::PortIndex const first, QtNodes::PortIndex const last) {
                portsAboutToBeInserted(newId, portType, first, last);
            });

    connect(model.get(),
            &QtNodes::NodeDelegateModel::portsInserted,
            this,
            &NodeProgramGraphModel::portsInserted);

    connect(model.get(), &QtNodes::NodeDelegateModel::requestNodeUpdate, this, [newId, this]() {
        Q_EMIT nodeUpdated(newId);
    });

    _models[newId] = std::move(model);

    _labels[newId] = _models[newId]->label();
    _labelsVisible[newId] = _models[newId]->labelVisible();

    Q_EMIT nodeCreated(newId);

    return newId;
}

bool NodeProgramGraphModel::dataTypeConnectionAllowed(const QtNodes::NodeDataType & outType, const QtNodes::NodeDataType & inType) const
{
    return outType.id == inType.id;
}

bool NodeProgramGraphModel::connectionPossible(QtNodes::ConnectionId const connectionId) const
{
    // Check if nodes exist
    if (!nodeExists(connectionId.outNodeId) || !nodeExists(connectionId.inNodeId)) {
        return false;
    }

    // Check port bounds, i.e. that we do not connect non-existing port numbers
    auto checkPortBounds = [&](QtNodes::PortType const portType) {
        QtNodes::NodeId const nodeId = getNodeId(portType, connectionId);
        auto portCountRole = (portType == QtNodes::PortType::Out) ? QtNodes::NodeRole::OutPortCount
                                                         : QtNodes::NodeRole::InPortCount;

        std::size_t const portCount = nodeData(nodeId, portCountRole).toUInt();

        return getPortIndex(portType, connectionId) < portCount;
    };

    auto getDataType = [&](QtNodes::PortType const portType) {
        return portData(getNodeId(portType, connectionId),
                        portType,
                        getPortIndex(portType, connectionId),
                        QtNodes::PortRole::DataType)
            .value<QtNodes::NodeDataType>();
    };

    auto portVacant = [&](QtNodes::PortType const portType) {
        QtNodes::NodeId const nodeId = getNodeId(portType, connectionId);
        QtNodes::PortIndex const portIndex = getPortIndex(portType, connectionId);
        const auto connected = connections(nodeId, portType, portIndex);

        auto policy = portData(nodeId, portType, portIndex, QtNodes::PortRole::ConnectionPolicyRole)
	    .value<QtNodes::ConnectionPolicy>();

        return connected.empty() || (policy == QtNodes::ConnectionPolicy::Many);
    };

    bool typeCheck = dataTypeConnectionAllowed(getDataType(QtNodes::PortType::Out), getDataType(QtNodes::PortType::In));

    bool const basicChecks = typeCheck
                             && portVacant(QtNodes::PortType::Out) && portVacant(QtNodes::PortType::In)
                             && checkPortBounds(QtNodes::PortType::Out) && checkPortBounds(QtNodes::PortType::In);

    // In data-flow mode (this class) it's important to forbid graph loops.
    // We perform depth-first graph traversal starting from the "Input" port of
    // the given connection. We should never encounter the starting "Out" node.

    auto hasLoops = [this, &connectionId]() -> bool {
        std::stack<QtNodes::NodeId> filo;
        filo.push(connectionId.inNodeId);

        while (!filo.empty()) {
            auto id = filo.top();
            filo.pop();

            if (id == connectionId.outNodeId) { // LOOP!
                return true;
            }

            // Add out-connections to continue interations
            std::size_t const nOutPorts = nodeData(id, QtNodes::NodeRole::OutPortCount).toUInt();

            for (QtNodes::PortIndex index = 0; index < nOutPorts; ++index) {
                const auto &outConnectionIds = connections(id, QtNodes::PortType::Out, index);

                for (auto cid : outConnectionIds) {
                    filo.push(cid.inNodeId);
                }
            }
        }

        return false;
    };

    return basicChecks && (loopsEnabled() || !hasLoops());
}


NodeProgram &
NodeProgramGraphModel::getParent(void) const
{
    return _parent;
}
void NodeProgramGraphModel::addConnection(QtNodes::ConnectionId const connectionId)
{
    _connectivity.insert(connectionId);

    sendConnectionCreation(connectionId);

    QVariant const portDataToPropagate = portData(connectionId.outNodeId,
                                                  QtNodes::PortType::Out,
                                                  connectionId.outPortIndex,
                                                  QtNodes::PortRole::Data);

    setPortData(connectionId.inNodeId,
                QtNodes::PortType::In,
                connectionId.inPortIndex,
                portDataToPropagate,
                QtNodes::PortRole::Data);
}

void NodeProgramGraphModel::sendConnectionCreation(QtNodes::ConnectionId const connectionId)
{
    Q_EMIT connectionCreated(connectionId);

    auto iti = _models.find(connectionId.inNodeId);
    auto ito = _models.find(connectionId.outNodeId);
    if (iti != _models.end() && ito != _models.end()) {
        auto &modeli = iti->second;
        auto &modelo = ito->second;
        modeli->inputConnectionCreated(connectionId);
        modelo->outputConnectionCreated(connectionId);
    }
}

void NodeProgramGraphModel::sendConnectionDeletion(QtNodes::ConnectionId const connectionId)
{
    Q_EMIT connectionDeleted(connectionId);

    auto iti = _models.find(connectionId.inNodeId);
    auto ito = _models.find(connectionId.outNodeId);
    if (iti != _models.end() && ito != _models.end()) {
        auto &modeli = iti->second;
        auto &modelo = ito->second;
        modeli->inputConnectionDeleted(connectionId);
        modelo->outputConnectionDeleted(connectionId);
    }
}

bool NodeProgramGraphModel::nodeExists(QtNodes::NodeId const nodeId) const
{
    return (_models.find(nodeId) != _models.end());
}

QVariant NodeProgramGraphModel::nodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role) const
{
    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
        return result;

    auto &model = it->second;

    switch (role) {
    case QtNodes::NodeRole::Type:
        result = model->name();
        break;

    case QtNodes::NodeRole::Position:
        result = _nodeGeometryData[nodeId].pos;
        break;

    case QtNodes::NodeRole::Size:
        result = _nodeGeometryData[nodeId].size;
        break;

    case QtNodes::NodeRole::CaptionVisible:
        result = model->captionVisible();
        break;

    case QtNodes::NodeRole::Caption:
        result = model->caption();
        break;

    case QtNodes::NodeRole::Style: {
        auto style = model->nodeStyle();
        result = style.toJson().toVariantMap();
    } break;

    case QtNodes::NodeRole::InternalData: {
        QJsonObject nodeJson;

        nodeJson["internal-data"] = model->save();

        result = nodeJson.toVariantMap();
        break;
    }

    case QtNodes::NodeRole::InPortCount:
        result = model->nPorts(QtNodes::PortType::In);
        break;

    case QtNodes::NodeRole::OutPortCount:
        result = model->nPorts(QtNodes::PortType::Out);
        break;

    case QtNodes::NodeRole::Widget: {
        auto *w = model->embeddedWidget();
        result = QVariant::fromValue(w);
    } break;

    case QtNodes::NodeRole::ValidationState: {
        auto validationState = model->validationState();
        result = QVariant::fromValue(validationState);
    } break;

    case QtNodes::NodeRole::LabelVisible: {
        const auto labelVisibleIt = _labelsVisible.find(nodeId);
        result = (labelVisibleIt != _labelsVisible.end()) ? labelVisibleIt->second
                                                          : model->labelVisible();
    } break;

    case QtNodes::NodeRole::Label: {
        const auto labelIt = _labels.find(nodeId);
        result = (labelIt != _labels.end()) ? labelIt->second : model->label();
    } break;

    case QtNodes::NodeRole::LabelEditable:
        result = model->labelEditable();
        break;

    case QtNodes::NodeRole::ProcessingStatus: {
        auto processingStatus = model->processingStatus();
        result = QVariant::fromValue(processingStatus);
    } break;

    case QtNodes::NodeRole::ProgressValue:
        result = model->progressValue();
        break;
    }

    return result;
}

QtNodes::NodeFlags NodeProgramGraphModel::nodeFlags(QtNodes::NodeId nodeId) const
{
    auto it = _models.find(nodeId);

    if (it != _models.end() && it->second->resizable())
        return QtNodes::NodeFlag::Resizable;

    return QtNodes::NodeFlag::NoFlags;
}

bool NodeProgramGraphModel::setNodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role, QVariant value)
{
    Q_UNUSED(nodeId);
    Q_UNUSED(role);
    Q_UNUSED(value);

    bool result = false;

    switch (role) {
    case QtNodes::NodeRole::Type:
        break;
    case QtNodes::NodeRole::Position: {
        _nodeGeometryData[nodeId].pos = value.value<QPointF>();

        Q_EMIT nodePositionUpdated(nodeId);

        result = true;
    } break;

    case QtNodes::NodeRole::Size: {
        _nodeGeometryData[nodeId].size = value.value<QSize>();
        result = true;
    } break;

    case QtNodes::NodeRole::CaptionVisible:
        break;

    case QtNodes::NodeRole::Caption:
        break;

    case QtNodes::NodeRole::Style:
        break;

    case QtNodes::NodeRole::InternalData:
        break;

    case QtNodes::NodeRole::InPortCount:
        break;

    case QtNodes::NodeRole::OutPortCount:
        break;

    case QtNodes::NodeRole::Widget:
        break;

    case QtNodes::NodeRole::ValidationState: {
        if (value.canConvert<QtNodes::NodeValidationState>()) {
            auto state = value.value<QtNodes::NodeValidationState>();
            if (auto node = delegateModel<QtNodes::NodeDelegateModel>(nodeId); node != nullptr) {
                node->setValidationState(state);
            }
        }
        Q_EMIT nodeUpdated(nodeId);
    } break;

    case QtNodes::NodeRole::ProcessingStatus: {
        if (value.canConvert<QtNodes::NodeProcessingStatus>()) {
            auto status = value.value<QtNodes::NodeProcessingStatus>();
            if (auto node = delegateModel<QtNodes::NodeDelegateModel>(nodeId); node != nullptr) {
                node->setNodeProcessingStatus(status);
            }
        }
        Q_EMIT nodeUpdated(nodeId);
    } break;

    case QtNodes::NodeRole::LabelVisible: {
        _labelsVisible[nodeId] = value.toBool();
        Q_EMIT nodeUpdated(nodeId);
        result = true;
    } break;

    case QtNodes::NodeRole::Label: {
        _labels[nodeId] = value.toString();
        Q_EMIT nodeUpdated(nodeId);
        result = true;
    } break;

    case QtNodes::NodeRole::LabelEditable:
        break;

    case QtNodes::NodeRole::ProgressValue:
        break;
    }

    return result;
}

QVariant NodeProgramGraphModel::portData(QtNodes::NodeId nodeId,
                                      QtNodes::PortType portType,
                                      QtNodes::PortIndex portIndex,
                                      QtNodes::PortRole role) const
{
    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
        return result;

    auto &model = it->second;

    switch (role) {
    case QtNodes::PortRole::Data:
        if (portType == QtNodes::PortType::Out) {
            result = QVariant::fromValue(model->outData(portIndex));
        }
        break;

    case QtNodes::PortRole::DataType:
        result = QVariant::fromValue(model->dataType(portType, portIndex));
        break;

    case QtNodes::PortRole::ConnectionPolicyRole:
        result = QVariant::fromValue(model->portConnectionPolicy(portType, portIndex));
        break;

    case QtNodes::PortRole::CaptionVisible:
        result = model->portCaptionVisible(portType, portIndex);
        break;

    case QtNodes::PortRole::Caption:
        result = model->portCaption(portType, portIndex);

        break;
    }

    return result;
}

bool NodeProgramGraphModel::setPortData(
    QtNodes::NodeId nodeId, QtNodes::PortType portType, QtNodes::PortIndex portIndex, QVariant const &value, QtNodes::PortRole role)
{
    Q_UNUSED(nodeId);

    QVariant result;

    auto it = _models.find(nodeId);
    if (it == _models.end())
        return false;

    auto &model = it->second;

    switch (role) {
    case QtNodes::PortRole::Data:
        if (portType == QtNodes::PortType::In) {
            if (model->frozen())
                return false;

            model->setInData(value.value<std::shared_ptr<QtNodes::NodeData>>(), portIndex);

            // Triggers repainting on the scene.
            Q_EMIT inPortDataWasSet(nodeId, portType, portIndex);
        }
        break;

    default:
        break;
    }

    return false;
}

bool NodeProgramGraphModel::deleteConnection(QtNodes::ConnectionId const connectionId)
{
    bool disconnected = false;

    auto it = _connectivity.find(connectionId);

    if (it != _connectivity.end()) {
        disconnected = true;

        _connectivity.erase(it);
    }

    if (disconnected) {
        sendConnectionDeletion(connectionId);

        propagateEmptyDataTo(getNodeId(QtNodes::PortType::In, connectionId),
                             getPortIndex(QtNodes::PortType::In, connectionId));
    }

    return disconnected;
}

bool NodeProgramGraphModel::deleteNode(QtNodes::NodeId const nodeId)
{
    // Delete connections to this node first.
    auto connectionIds = allConnectionIds(nodeId);
    for (auto &cId : connectionIds) {
        deleteConnection(cId);
    }

    _nodeGeometryData.erase(nodeId);
    _labels.erase(nodeId);
    _labelsVisible.erase(nodeId);
    _models.erase(nodeId);

    Q_EMIT nodeDeleted(nodeId);

    return true;
}

QJsonObject NodeProgramGraphModel::saveNode(QtNodes::NodeId const nodeId) const
{
    QJsonObject nodeJson;

    const auto modelIt = _models.find(nodeId);
    if (modelIt == _models.end()) {
        return nodeJson;
    }

    const auto &model = modelIt->second;

    nodeJson["id"] = static_cast<qint64>(nodeId);
    nodeJson["internal-data"] = model->save();

    const auto labelIt = _labels.find(nodeId);
    nodeJson["label"] = (labelIt != _labels.end()) ? labelIt->second : model->label();

    const auto labelVisibleIt = _labelsVisible.find(nodeId);
    nodeJson["labelVisible"] = (labelVisibleIt != _labelsVisible.end()) ? labelVisibleIt->second
                                                                        : model->labelVisible();

    {
        QPointF const pos = nodeData(nodeId, QtNodes::NodeRole::Position).value<QPointF>();
        QJsonObject posJson;
        posJson["x"] = pos.x();
        posJson["y"] = pos.y();
        nodeJson["position"] = posJson;
    }

    return nodeJson;
}

QJsonObject NodeProgramGraphModel::save() const
{
    QJsonObject sceneJson;

    QJsonArray nodesJsonArray;
    for (const auto nodeId : allNodeIds()) {
        nodesJsonArray.append(saveNode(nodeId));
    }
    sceneJson["nodes"] = nodesJsonArray;

    QJsonArray connJsonArray;
    for (const auto &cid : _connectivity) {
        connJsonArray.append(toJson(cid));
    }
    sceneJson["connections"] = connJsonArray;

    // Take the 'transpose' of this map:
    auto groupMap = getGroups();
    QJsonArray groupJsonArray;
    for (const auto & gid : groupMap) {
	QJsonObject groupObj;

	QJsonValue gidJson((qint64)gid.first);
	groupObj["id"] = gidJson;
	QJsonArray nodesJsonArray;
	for (const auto & nodeid : gid.second) {
	    QJsonValue nidJson((qint64)nodeid);
	    nodesJsonArray.append(nidJson);
	}
	groupObj["nodes"] = nodesJsonArray;
	
	groupJsonArray.append(groupObj);
    }
    
    sceneJson["groups"] = groupJsonArray;
    
    return sceneJson;
}

void NodeProgramGraphModel::loadNode(QJsonObject const &nodeJson)
{
    // Possibility of the id clash when reading it from json and not generating a
    // new value.
    // 1. When restoring a scene from a file.
    // Conflict is not possible because the scene must be cleared by the time of
    // loading.
    // 2. When undoing the deletion command.  Conflict is not possible
    // because all the new ids were created past the removed nodes.
    QtNodes::NodeId restoredNodeId = nodeJson["id"].toInt();

    _nextNodeId = std::max(_nextNodeId, restoredNodeId + 1);

    QJsonObject const internalDataJson = nodeJson["internal-data"].toObject();

    QString delegateModelName = internalDataJson["model-name"].toString();

    std::unique_ptr<QtNodes::NodeDelegateModel> model = _registry->create(delegateModelName, *this);

    if (model) {
        connect(model.get(),
                &QtNodes::NodeDelegateModel::dataUpdated,
                [restoredNodeId, this](QtNodes::PortIndex const portIndex) {
                    onOutPortDataUpdated(restoredNodeId, portIndex);
                });

        connect(model.get(),
                &QtNodes::NodeDelegateModel::portsAboutToBeDeleted,
                this,
                [restoredNodeId,
                 this](QtNodes::PortType const portType, QtNodes::PortIndex const first, QtNodes::PortIndex const last) {
                    portsAboutToBeDeleted(restoredNodeId, portType, first, last);
                });

        connect(model.get(),
                &QtNodes::NodeDelegateModel::portsDeleted,
                this,
                &NodeProgramGraphModel::portsDeleted);

        connect(model.get(),
                &QtNodes::NodeDelegateModel::portsAboutToBeInserted,
                this,
                [restoredNodeId,
                 this](QtNodes::PortType const portType, QtNodes::PortIndex const first, QtNodes::PortIndex const last) {
                    portsAboutToBeInserted(restoredNodeId, portType, first, last);
                });

        connect(model.get(),
                &QtNodes::NodeDelegateModel::portsInserted,
                this,
                &NodeProgramGraphModel::portsInserted);

        connect(model.get(), &QtNodes::NodeDelegateModel::requestNodeUpdate, this, [restoredNodeId, this]() {
            Q_EMIT nodeUpdated(restoredNodeId);
        });

        _models[restoredNodeId] = std::move(model);

        Q_EMIT nodeCreated(restoredNodeId);

        QJsonObject posJson = nodeJson["position"].toObject();
        QPointF const pos(posJson["x"].toDouble(), posJson["y"].toDouble());
        setNodeData(restoredNodeId, QtNodes::NodeRole::Position, pos);

        auto *restoredModel = _models[restoredNodeId].get();
        _labels[restoredNodeId] = nodeJson["label"].toString(restoredModel->label());
        _labelsVisible[restoredNodeId] = nodeJson.contains("labelVisible")
                                             ? nodeJson["labelVisible"].toBool()
                                             : restoredModel->labelVisible();

        restoredModel->load(internalDataJson);
    } else {
        throw std::logic_error(std::string("No registered model with name ")
                               + delegateModelName.toLocal8Bit().data());
    }
}

void NodeProgramGraphModel::load(QJsonObject const &jsonDocument)
{
    QJsonArray nodesJsonArray = jsonDocument["nodes"].toArray();

    for (QJsonValueRef nodeJson : nodesJsonArray) {
        loadNode(nodeJson.toObject());
    }

    QJsonArray connectionJsonArray = jsonDocument["connections"].toArray();

    for (QJsonValueRef connection : connectionJsonArray) {
        QJsonObject connJson = connection.toObject();

        QtNodes::ConnectionId connId = QtNodes::fromJson(connJson);

        // Restore the connection
        addConnection(connId);
    }

    QJsonArray groupJsonArray = jsonDocument["groups"].toArray();
    for (QJsonValueRef groupRef : groupJsonArray) {
	QJsonObject group = groupRef.toObject();
	QtNodes::GroupId groupId = group["id"].toInt();
	QJsonArray nodeListJsonArray = group["nodes"].toArray();
	for (QJsonValueRef node : nodeListJsonArray) {
	    QtNodes::NodeId nodeId = node.toInt();
	    fprintf(stderr, "Loaded group %d %d\n", groupId, nodeId);
	    setNodeGroup(nodeId, groupId);
	}
    }
    
}

void NodeProgramGraphModel::onOutPortDataUpdated(QtNodes::NodeId const nodeId, QtNodes::PortIndex const portIndex)
{
    std::unordered_set<QtNodes::ConnectionId> const &connected = connections(nodeId,
                                                                    QtNodes::PortType::Out,
                                                                    portIndex);

    QVariant const portDataToPropagate = portData(nodeId, QtNodes::PortType::Out, portIndex, QtNodes::PortRole::Data);

    for (const auto &cn : connected) {
        setPortData(cn.inNodeId, QtNodes::PortType::In, cn.inPortIndex, portDataToPropagate, QtNodes::PortRole::Data);
    }
}

void NodeProgramGraphModel::propagateEmptyDataTo(QtNodes::NodeId const nodeId, QtNodes::PortIndex const portIndex)
{
    QVariant emptyData{};

    setPortData(nodeId, QtNodes::PortType::In, portIndex, emptyData, QtNodes::PortRole::Data);
}

void NodeProgramGraphModel::setNodeGroup(QtNodes::NodeId const nodeId, QtNodes::GroupId const groupId)
{
    _groups[nodeId] = groupId;
}

void
NodeProgramGraphModel::unsetNodeGroup(QtNodes::NodeId const nodeId)
{
    _groups.erase(nodeId);
}

std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>>
NodeProgramGraphModel::getGroups() const
{
    std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>> groupMap;
    for (const auto & gid : _groups) {
	groupMap[gid.second].push_back(gid.first);
    }
    return groupMap;
}
