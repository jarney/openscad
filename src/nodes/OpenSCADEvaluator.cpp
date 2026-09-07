#include <stdio.h>
#include <QString>
#include <QtNodes/Definitions>
#include "OpenSCADModels.hpp"
#include "OpenSCADEvaluator.hpp"

static void
findOutputNodes(
    const QtNodes::DataFlowGraphModel & model,
    std::vector<QtNodes::NodeId> & outputNodes
    )
{
    for (auto nodeId : model.allNodeIds()) {
	QString nodeType = model.nodeData(nodeId, QtNodes::NodeRole::Type).value<QString>();


	if (nodeType == "output") {
	    // First, verify that this really is an output node.
	    auto connections = model.connections(nodeId, QtNodes::PortType::In, 0);
	    if (connections.size() == 0) {
		fprintf(stderr, "No connection to output node\n");
	    }
	    else if (connections.size() > 1) {
		fprintf(stderr, "Only one geometry node must be connected to output\n");
	    }
	    else {
		outputNodes.push_back(nodeId);
	    }
	
	}
    }
}

static std::vector<QtNodes::NodeId> connectedNodes(const QtNodes::DataFlowGraphModel & model, QtNodes::NodeId nodeId)
{
    auto connections = model.allConnectionIds(nodeId);
    std::vector<QtNodes::NodeId> connected_nodes;
    for (auto connection : connections) {
	if (nodeId == connection.outNodeId) continue;
	if (std::find(connected_nodes.begin(), connected_nodes.end(), connection.outNodeId) == connected_nodes.end()) {
	    connected_nodes.push_back(connection.outNodeId);
	}
    }
    return connected_nodes;
}

// We make the assumption here that the graph is cycle-free.
// If there are cycles, this will blow up.
static std::string processNode(const QtNodes::DataFlowGraphModel & model, QtNodes::NodeId nodeId, int depth)
{
    std::vector<QtNodes::NodeId> connected_nodes = connectedNodes(model, nodeId);
    std::vector<std::string> input_data;
    for (auto pred : connected_nodes) {
	input_data.push_back(processNode(model, pred, depth+1));
    }
	
    BaseSCADModel *nodeData = model.delegateModel<BaseSCADModel>(nodeId);
    
    return nodeData->process(input_data);
}


void evaluateToSCAD(const QtNodes::DataFlowGraphModel & model)
{
//    virtual std::unordered_set<NodeId> allNodeIds() const = 0;
    std::vector<QtNodes::NodeId> outputNodes;

    // First, find all of the 'output' nodes.
    findOutputNodes(model, outputNodes);

    
    
//    std::unordered_set<ConnectionId> connections(NodeId nodeId,
//                                                 PortType portType,
//                                                 PortIndex portIndex) const override;
    for (auto nodeId : outputNodes) {
	// Output nodes should have only one
	// input connection at index 0.
	auto out = processNode(model, nodeId, 0);
	fprintf(stderr, "%s\n", out.c_str());
    }

    // Next, for each output node, wire up all of the input ports upstream.
    // If a port has no connection, use a default data for that port.
    
    // Continue recursively until there are not more ancestor nodes.
    
}
