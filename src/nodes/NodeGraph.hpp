#pragma once

#include <QtNodes/AbstractGraphModel>
#include <QtNodes/ConnectionIdUtils>
#include "NodeFactoryRegistry.hpp"
#include <QtNodes/internal/Serializable.hpp>
#include <QtNodes/StyleCollection>
#include <QtNodes/NodeGroup>

//#include "Export.hpp"

#include <QJsonObject>

#include <memory>
#include <unordered_map>
#include <QString>

namespace JNodes {
    namespace core {

class NodeProgram;

class NODE_EDITOR_PUBLIC NodeGraph
    : public QtNodes::AbstractGraphModel
    , public QtNodes::Serializable
{
    Q_OBJECT

public:
    struct NodeGeometryData
    {
        QSize size;
        QPointF pos;
    };

public:
    NodeGraph(std::shared_ptr<NodeFactoryRegistry> registry, NodeProgram & parent);

    std::shared_ptr<NodeFactoryRegistry> dataModelRegistry() { return _registry; }

public:
    std::unordered_set<QtNodes::NodeId> allNodeIds() const override;

    std::unordered_set<QtNodes::ConnectionId> allConnectionIds(QtNodes::NodeId const nodeId) const override;

    std::unordered_set<QtNodes::ConnectionId> connections(QtNodes::NodeId nodeId,
                                                 QtNodes::PortType portType,
                                                 QtNodes::PortIndex portIndex) const override;

    bool connectionExists(QtNodes::ConnectionId const connectionId) const override;

    QtNodes::NodeId addNode(QString const nodeType) override;
    QtNodes::NodeId addNode(std::unique_ptr<QtNodes::NodeDelegateModel> model);

    bool connectionPossible(QtNodes::ConnectionId const connectionId) const override;

    /**
     * Returns true if an output port of the given type is allowed to connect
     * to an input port of another type.  This is useful when you want to allow
     * certain types of 'casting' operations where more than one data type may
     * be accepted.
     */
    bool dataTypeConnectionAllowed(const QtNodes::NodeDataType & outType, const QtNodes::NodeDataType & inType) const;

    void addConnection(QtNodes::ConnectionId const connectionId) override;

    bool nodeExists(QtNodes::NodeId const nodeId) const override;

    QVariant nodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role) const override;

    QtNodes::NodeFlags nodeFlags(QtNodes::NodeId nodeId) const override;

    bool setNodeData(QtNodes::NodeId nodeId, QtNodes::NodeRole role, QVariant value) override;

    QVariant portData(QtNodes::NodeId nodeId,
                      QtNodes::PortType portType,
                      QtNodes::PortIndex portIndex,
                      QtNodes::PortRole role) const override;

    bool setPortData(QtNodes::NodeId nodeId,
                     QtNodes::PortType portType,
                     QtNodes::PortIndex portIndex,
                     QVariant const &value,
                     QtNodes::PortRole role = QtNodes::PortRole::Data) override;

    bool deleteConnection(QtNodes::ConnectionId const connectionId) override;

    bool deleteNode(QtNodes::NodeId const nodeId) override;

    QJsonObject saveNode(QtNodes::NodeId const) const override;
    QJsonObject saveGroup(
	const std::pair<QtNodes::GroupId, std::vector<QtNodes::NodeId>> group
	) const;

    void loadNode(QJsonObject const &nodeJson) override;

    // From Serializable
    QJsonObject save() const override;

    // From Serializable
    void load(QJsonObject const &json) override;

    /**
     * Fetches the NodeDelegateModel for the given `nodeId` and tries to cast the
     * stored pointer to the given type
     */
    template<typename NodeDelegateModelType>
    NodeDelegateModelType *delegateModel(QtNodes::NodeId const nodeId) const
    {
        auto it = _models.find(nodeId);
        if (it == _models.end())
            return nullptr;

        auto model = dynamic_cast<NodeDelegateModelType *>(it->second.get());

        return model;
    }

    /// Loops do not make any sense in uni-direction data propagation
    bool loopsEnabled() const override { return false; }

    NodeProgram & getParent(void) const;
    
    /**
     * This method is used to indicate that the given node should be a part
     * of the given node group.  Node groups are not heirarchal, so
     * if a node is a member of one group, it is not a member of another.
     */
    void setNodeGroup(QtNodes::NodeId const nodeId, QtNodes::GroupId const groupId);
    
    /**
     * This method is used to indicate that the given node should no longer
     * be a part of any node group.
     */
    void unsetNodeGroup(QtNodes::NodeId const nodeId);

    /**
     * Returns the map of groups.
     */
    std::map<QtNodes::GroupId, std::vector<QtNodes::NodeId>> getGroups() const;
    
Q_SIGNALS:
    void inPortDataWasSet(QtNodes::NodeId const, QtNodes::PortType const, QtNodes::PortIndex const);

private:
    QtNodes::NodeId newNodeId() override { return _nextNodeId++; }

    void sendConnectionCreation(QtNodes::ConnectionId const connectionId);

    void sendConnectionDeletion(QtNodes::ConnectionId const connectionId);

private Q_SLOTS:
    /**
     * Fuction is called in three cases:
     *
     * - By underlying NodeDelegateModel when a node has new data to propagate.
     *   @see NodeGraph::addNode
     * - When a new connection is created.
     *   @see NodeGraph::addConnection
     * - When a node restored from JSON an needs to send data downstream.
     *   @see NodeGraph::loadNode
     */
    void onOutPortDataUpdated(QtNodes::NodeId const nodeId, QtNodes::PortIndex const portIndex);

    /// Function is called after detaching a connection.
    void propagateEmptyDataTo(QtNodes::NodeId const nodeId, QtNodes::PortIndex const portIndex);

private:
    std::shared_ptr<NodeFactoryRegistry> _registry;

    QtNodes::NodeId _nextNodeId;

    std::unordered_map<QtNodes::NodeId, std::unique_ptr<QtNodes::NodeDelegateModel>> _models;

    std::unordered_set<QtNodes::ConnectionId> _connectivity;

    mutable std::unordered_map<QtNodes::NodeId, NodeGeometryData> _nodeGeometryData;

    std::unordered_map<QtNodes::NodeId, QString> _labels;
    std::unordered_map<QtNodes::NodeId, bool> _labelsVisible;

    std::map<QtNodes::NodeId, QtNodes::GroupId> _groups;

    NodeProgram & _parent;
};

    } // End core
} // End JNodes
