#include "nodes/gui/GraphModelAdapter.hpp"

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/ConnectionIdUtils>
#include <QtNodes/NodeData>
#include <QtNodes/StyleCollection>

#include <QPoint>
#include <QSize>

#include <unordered_set>
#include <stack>

using namespace NodeJS::core;
using namespace NodeJS::gui;

GraphModelAdapter::GraphModelAdapter(NodeJS::core::NodeGraph & aGraph)
    : mGraph(aGraph)
{
    mID_nextNew = 0;
    for (const auto & it : mGraph.getNodes()) {
	mID_toGraph.insert(std::make_pair(mID_nextNew, it.first));
	mID_fromGraph.insert(std::make_pair(it.first, mID_nextNew));
	mID_nextNew++;
    }
}

QtNodes::NodeId
GraphModelAdapter::newNodeId()
{
    while (true) {
	if (mID_toGraph.count(mID_nextNew) == 0) {
	    return mID_nextNew++;
	}
	mID_nextNew++;
    }
}

std::unordered_set<QtNodes::NodeId>
GraphModelAdapter::allNodeIds() const
{
    std::unordered_set<QtNodes::NodeId> allNodes;
    for (const auto & it : mID_toGraph) {
	allNodes.insert(it.first);
    }
    return allNodes;
}


/**
 * A collection of all input and output connections for the given `nodeId`.
 */
std::unordered_set<QtNodes::ConnectionId>
GraphModelAdapter::allConnectionIds(QtNodes::NodeId const nodeId) const
{
    std::unordered_set<QtNodes::ConnectionId> allConnections;

    // Node does not exist, this is a problem.
    const auto & nodeIt = mID_toGraph.find(nodeId);
    if (nodeIt == mID_toGraph.end()) {
	fprintf(stderr, "THIS SHOULD NEVER HAPPEN, WE'RE MISSING A NODE IN THE ADAPTER'S MAP %d\n", nodeId);
	return allConnections;
    }
    std::string nodeIdGraph = nodeIt->second;
    
    const auto & edges = mGraph.getEdges();
    for (const auto & edgeIt : edges) {
	
	QtNodes::ConnectionId connection;
	const Edge *edge = edgeIt.second.get();
	
	const Node *toNode = mGraph.getNode(edge->toNode);
	const Node *fromNode = mGraph.getNode(edge->fromNode);
	if (toNode == nullptr || fromNode == nullptr) {
	    fprintf(stderr, "THIS SHOULD NEVER HAPPEN, NODE NOT EXISTING IN GRAPH (to node OR FROM NODE) %s %p or %s %p\n",
		    edge->toNode.c_str(), toNode,
		    edge->fromNode.c_str(), fromNode);
	    continue;
	}
	if (edge->fromNode == nodeIdGraph) {
	    const auto & toNodeIt = mID_fromGraph.find(edge->toNode);
	    if (toNodeIt == mID_fromGraph.end()) {
		fprintf(stderr, "THIS SHOULD NEVER HAPPEN, NODE NOT EXISTING IN ADAPTER'S MAP (to node) %s\n", edge->toNode.c_str());
		continue;
	    }
	    connection.outNodeId = nodeId;
	    connection.inNodeId = toNodeIt->second;
	}
	else if (edge->toNode == nodeIdGraph) {
	    const auto & fromNodeIt = mID_fromGraph.find(edge->fromNode);
	    if (fromNodeIt == mID_fromGraph.end()) {
		fprintf(stderr, "THIS SHOULD NEVER HAPPEN, NODE NOT EXISTING IN ADAPTER'S MAP (from node) %s\n", edge->fromNode.c_str());
		continue;
	    }
	    connection.outNodeId = fromNodeIt->second;
	    connection.inNodeId = nodeId;
	}
	connection.outPortIndex = fromNode->getOutputs().getPortIndex(edge->fromPort);
	connection.inPortIndex = toNode->getInputs().getPortIndex(edge->toPort);
	allConnections.insert(connection);
    }

    return allConnections;
}


std::unordered_set<QtNodes::ConnectionId>
GraphModelAdapter::connections(
    QtNodes::NodeId nodeId,
    QtNodes::PortType portType,
    QtNodes::PortIndex index) const
{
    std::unordered_set<QtNodes::ConnectionId> selectedConnections;

    const auto allConnections = allConnectionIds(nodeId);
    for (const auto & connection : allConnections) {

	// Skip if this isn't our node.
	if (nodeId != connection.outNodeId &&
	    nodeId != connection.inNodeId) {
	    continue;
	}
	if (portType == QtNodes::PortType::In && index == connection.inPortIndex) {
	    selectedConnections.insert(connection);
	}
	else if (portType == QtNodes::PortType::Out && index == connection.outPortIndex) {
	    selectedConnections.insert(connection);
	}
    }
    return selectedConnections;
}

	    
bool
GraphModelAdapter::connectionExists(QtNodes::ConnectionId const connectionId) const
{
    const auto allConnections = allConnectionIds(connectionId.inNodeId);
    for (const auto & connection : allConnections) {
	if (connection == connectionId) return true;
    }
    return false;
}


QtNodes::NodeId
GraphModelAdapter::addNode(QString const nodeTypeName)
{
    // If we can't resolve the node type,
    // we cannot create the node.
    const NodeType *nodeType = mGraph.getNodeType(nodeTypeName.toStdString());
    if (nodeType == nullptr) {
	return QtNodes::InvalidNodeId;
    }
    
    Node &newNode = mGraph.newNode(*nodeType, nodeType->getId());
    
    // Now that we have the node, we need to keep track of the IDs so we can map correctly.
    QtNodes::NodeId adapterNodeId = newNodeId();
    mID_toGraph.insert(std::make_pair(adapterNodeId, newNode.getId()));
    mID_fromGraph.insert(std::make_pair(newNode.getId(), adapterNodeId));

    return adapterNodeId;
}

// This is internal, we don't need to expose it.
// We may at some point, allow 'typecasting'
// node data, but this might not actually be a good idea.
static bool
dataTypeConnectionAllowed(const QtNodes::NodeDataType & outType, const QtNodes::NodeDataType & inType)
{
    return outType.id == inType.id;
}

bool
GraphModelAdapter::connectionPossible(QtNodes::ConnectionId const connectionId) const
{
    // Check if nodes exist
    if (!nodeExists(connectionId.outNodeId) || !nodeExists(connectionId.inNodeId)) {
        return false;
    }
    
    // Check port bounds, i.e. that we do not connect non-existing port numbers
    auto checkPortBounds = [&](QtNodes::PortType const portType) {
	QtNodes::NodeId const nodeId = QtNodes::getNodeId(portType, connectionId);
        auto portCountRole = (portType == QtNodes::PortType::Out) ? QtNodes::NodeRole::OutPortCount
                                                         : QtNodes::NodeRole::InPortCount;

        std::size_t const portCount = nodeData(nodeId, portCountRole).toUInt();

        return QtNodes::getPortIndex(portType, connectionId) < portCount;
    };

    auto getDataType = [&](QtNodes::PortType const portType) {
        return portData(QtNodes::getNodeId(portType, connectionId),
                        portType,
                        QtNodes::getPortIndex(portType, connectionId),
                        QtNodes::PortRole::DataType)
            .value<QtNodes::NodeDataType>();
    };

    auto portVacant = [&](QtNodes::PortType const portType) {
        QtNodes::NodeId const nodeId = QtNodes::getNodeId(portType, connectionId);
        QtNodes::PortIndex const portIndex = QtNodes::getPortIndex(portType, connectionId);
        auto const connected = connections(nodeId, portType, portIndex);

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
                auto const &outConnectionIds = connections(id, QtNodes::PortType::Out, index);

                for (auto cid : outConnectionIds) {
                    filo.push(cid.inNodeId);
                }
            }
        }

        return false;
    };

    return basicChecks && (loopsEnabled() || !hasLoops());
}

bool
GraphModelAdapter::nodeExists(QtNodes::NodeId const nodeId) const
{
    return (mID_toGraph.find(nodeId) == mID_toGraph.end());
}

QVariant GraphModelAdapter::nodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role) const
{
    QVariant result;

    const auto & graphNodeIdIt = mID_toGraph.find(nodeId);
    if (graphNodeIdIt == mID_toGraph.end()) {
	return result;
    }

    const Node *node = mGraph.getNode(graphNodeIdIt->second);
    if (node == nullptr) {
        return result;
    }

    switch (role) {
    case QtNodes::NodeRole::Type: {
        result = QString::fromStdString(node->getId());
        break;
    }
    case QtNodes::NodeRole::Position: {
	const std::pair<float, float> &graphPos = node->getPosition();
        QPointF pos(graphPos.first, graphPos.second);
        result = pos;
        break;
    }
    case QtNodes::NodeRole::Size: {
	const std::pair<int, int> &graphSize = node->getSize();
        QSize size(graphSize.first, graphSize.second);
        result = size;
        break;
    }
#if 0
    case QtNodes::NodeRole::CaptionVisible:
	//TODO: push down to the node.
        result = model->captionVisible();
        break;

    case QtNodes::NodeRole::Caption:
	//TODO: push down to the node.
        result = model->caption();
        break;
#endif
    case QtNodes::NodeRole::Style: {
	// We will probably never have a reason to change this.
        result = QtNodes::StyleCollection::nodeStyle().toJson().toVariantMap();
    } break;
    case QtNodes::NodeRole::InternalData: {
	// This has no purpose and should be removed.
        break;
    }
    case QtNodes::NodeRole::InPortCount:
        result = QVariant::fromValue((int)node->getInputs().getCount());
        break;

    case QtNodes::NodeRole::OutPortCount:
        result = QVariant::fromValue((int)node->getOutputs().getCount());
        break;
	
    case QtNodes::NodeRole::Widget: {
	// TODO: Get these from some editor registration system
	// because this doesn't belong as a part of the graph,
	// it is actually a part of the editor/IDE.
        //auto *w = model->embeddedWidget();
        //result = QVariant::fromValue(w);
    } break;
    case QtNodes::NodeRole::ValidationState: {
	// State is meaningless for our application right now unless
	// we do some additional error checking and validation.
        result = QVariant::fromValue(QtNodes::NodeValidationState::State::Valid);
    } break;
    case QtNodes::NodeRole::ProcessingStatus: {
	// State is meaningless for our application right now unless
	// we do some additional error checking and validation.
        result = QVariant::fromValue(QtNodes::NodeProcessingStatus::NoStatus);
    } break;
    case QtNodes::NodeRole::ProgressValue:
        result = QString();
        break;

#if 0

    case QtNodes::NodeRole::LabelVisible: {
        auto const labelVisibleIt = _labelsVisible.find(nodeId);
        result = (labelVisibleIt != _labelsVisible.end()) ? labelVisibleIt->second
                                                          : model->labelVisible();
    } break;

    case QtNodes::NodeRole::Label: {
        auto const labelIt = _labels.find(nodeId);
        result = (labelIt != _labels.end()) ? labelIt->second : model->label();
    } break;

    case QtNodes::NodeRole::LabelEditable:
        result = model->labelEditable();
        break;

#endif
    }

    return result;
}

std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>>
GraphModelAdapter::getGroups() const
{
    std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>> groups;
    QtNodes::GroupId groupId = 0;

    for (const auto & graphGroupIt : mGraph.getGroups()) {
	for (const auto & nodeIt : graphGroupIt.second->getNodes()) {
	    const auto & nodeIdIt = mID_fromGraph.find(nodeIt);
	    if (nodeIdIt == mID_fromGraph.end()) {
		continue;
	    }
	    groups[groupId].push_back(nodeIdIt->second);
	}
    }
    
    return groups;
}


bool
GraphModelAdapter::deleteNode(QtNodes::NodeId const nodeId)
{
    const auto & it = mID_toGraph.find(nodeId);
    if (it == mID_toGraph.end()) {
	return false;
    }
    NodeId graphNodeId = it->second;
    mGraph.removeNode(graphNodeId);

    return true;
}

void
GraphModelAdapter::addConnection(QtNodes::ConnectionId const connectionId)
{
    const auto & fromNodeIt = mID_toGraph.find(connectionId.outNodeId);
    const auto & toNodeIt = mID_toGraph.find(connectionId.inNodeId);
    if (fromNodeIt == mID_toGraph.end() ||
	toNodeIt == mID_toGraph.end()) {
	fprintf(stderr, "NODES COULD NOT BE RESOLVED WHEN CREATING CONNECTION\n");
	return;
    }
    
    NodeId fromNodeId = fromNodeIt->second;
    NodeId toNodeId = toNodeIt->second;

    Node *fromNode = mGraph.getNode(fromNodeId);
    Node *toNode = mGraph.getNode(toNodeId);

    std::optional<EdgeId> optEdge = mGraph.newEdge(
	fromNodeId,
	fromNode->getOutputs().getName(connectionId.outPortIndex),
	toNodeId,
	toNode->getInputs().getName(connectionId.inPortIndex)
	);
}

bool
GraphModelAdapter::deleteConnection(QtNodes::ConnectionId const connectionId)
{
    const auto & fromNodeIt = mID_toGraph.find(connectionId.outNodeId);
    const auto & toNodeIt = mID_toGraph.find(connectionId.inNodeId);
    if (fromNodeIt == mID_toGraph.end() ||
	toNodeIt == mID_toGraph.end()) {
	fprintf(stderr, "NODES COULD NOT BE RESOLVED WHEN CREATING CONNECTION\n");
	return false;
    }
    
    NodeId fromNodeId = fromNodeIt->second;
    NodeId toNodeId = toNodeIt->second;

    Node *fromNode = mGraph.getNode(fromNodeId);
    Node *toNode = mGraph.getNode(toNodeId);

    mGraph.removeEdge(
	fromNodeId,
	fromNode->getOutputs().getName(connectionId.outPortIndex),
	toNodeId,
	toNode->getInputs().getName(connectionId.inPortIndex)
	);
    return true;
}

