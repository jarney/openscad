#include <stdio.h>
#include <QString>
#include <QtNodes/Definitions>

#include "nodes/OpenSCADBuiltinModel.hpp"
#include "nodes/OpenSCADEvaluator.hpp"

using namespace JNodes::core;

static void
findOutputNodes(
    const NodeGraph & model,
    std::vector<QtNodes::NodeId> & outputNodes
    )
{
    for (auto nodeId : model.allNodeIds()) {
	OpenSCADBuiltinModel *delegate = model.delegateModel<OpenSCADBuiltinModel>(nodeId);
	// Output nodes must have one input and no outputs.
	if ((delegate->nPorts(QtNodes::PortType::In) == 1) &&
	    (delegate->nPorts(QtNodes::PortType::Out) == 0)) {
	    outputNodes.push_back(nodeId);
	}
    }
}

static std::vector<QtNodes::ConnectionId> inputConnections(const NodeGraph & model, QtNodes::NodeId nodeId)
{
    auto connections = model.allConnectionIds(nodeId);
    std::vector<QtNodes::ConnectionId> input_connections;
    for (auto connection : connections) {
	if (nodeId == connection.outNodeId) continue;
	input_connections.push_back(connection);
    }
    return input_connections;
}

static std::vector<QtNodes::NodeId> connectedNodes(const std::vector<QtNodes::ConnectionId> & connections)
{
    std::vector<QtNodes::NodeId> connected_nodes;
    for (auto connection : connections) {
	if (std::find(connected_nodes.begin(), connected_nodes.end(), connection.outNodeId) == connected_nodes.end()) {
	    connected_nodes.push_back(connection.outNodeId);
	}
    }
    return connected_nodes;
}

// We make the assumption here that the graph is cycle-free.
// If there are cycles, this will blow up.
static void processNode(
    const NodeGraph & model,
    std::set<QtNodes::NodeId> & processed_nodes,    // Set of nodes that has already been processed.
    std::map<QtNodes::NodeId, PortFunctionData> & all_node_data,
    QtNodes::NodeId nodeId,                       // Node to process.
    int depth                                     // Exit early if we reach maximum stack depth.
    )
{
    // If we have already processed a node, then we have also
    // already processed all of its predecessors because we disallow
    // recursion or cycles in graphs.
    if (processed_nodes.count(nodeId) > 0) {
	return;
    }
    fprintf(stderr, "Processing node %d\n", nodeId);
    processed_nodes.insert(nodeId);

    std::vector<QtNodes::ConnectionId> input_connections = inputConnections(model, nodeId);
    std::vector<QtNodes::NodeId> connected_nodes = connectedNodes(input_connections);

    for (auto pred : connected_nodes) {
	processNode(model, processed_nodes, all_node_data, pred, depth+1);
    }
    OpenSCADBuiltinModel *nodeData = model.delegateModel<OpenSCADBuiltinModel>(nodeId);

    PortFunctionData input_data;
    fprintf(stderr, "Processing input connections %ld\n", input_connections.size());
    for (auto & connection : input_connections) {
	OpenSCADBuiltinModel *upstreamNode = model.delegateModel<OpenSCADBuiltinModel>(connection.outNodeId);
	std::string outputPortName = upstreamNode->outputPortName(connection.outPortIndex);
	
	std::string inputPortName = nodeData->inputPortName(connection.inPortIndex);
	const PortFunctionData & connected_node_data = all_node_data[connection.outNodeId];
	if (connected_node_data.hasValue(outputPortName)) {
	    std::string outputValue = connected_node_data.getValue(outputPortName, "");
	    input_data.setValue(inputPortName, outputValue);
	    fprintf(stderr, "Node %d:%s => %d:%s = %s\n", connection.outNodeId, outputPortName.c_str(), nodeId, inputPortName.c_str(), outputValue.c_str());
	}
    }
    // Create a new entry and pass it by reference.
    auto & node_data = all_node_data[nodeId];
    nodeData->process(input_data, node_data);
}


std::string evaluateToSCAD(const NodeGraph & model)
{
//    virtual std::unordered_set<NodeId> allNodeIds() const = 0;
    std::vector<QtNodes::NodeId> outputNodes;

    // First, find all of the 'output' nodes.
    findOutputNodes(model, outputNodes);

    // Here's now this needs to work.
    // Each node gets 'process' called on it
    // once with all of its upstream data available.
    // If an upstream data is not available, it has not yet been processed.

    // We start with a map of node and output data.
    // If the node is not on the map, we place it on the map
    // with no data.  Once a node has all of its data,
    // we call 'process' on it and put the result in its data.
    // If we find a node that has missing data, we put it on the open list
    // and process it.
    
//    std::unordered_set<ConnectionId> connections(NodeId nodeId,
//                                                 PortType portType,
//                                                 PortIndex portIndex) const override;

    std::set<QtNodes::NodeId> processed_nodes;
    std::map<QtNodes::NodeId, PortFunctionData> all_node_data;

    std::string output;
    for (auto nodeId : outputNodes) {
	// For each output node, wire up all of the input ports upstream.
	// If a port has no connection, use a default data for that port.
	// Continue recursively until there are not more ancestor nodes.
	processNode(model, processed_nodes, all_node_data, nodeId, 0);
	output += all_node_data[nodeId].getValue("out", "");
    }


    return output;
}
