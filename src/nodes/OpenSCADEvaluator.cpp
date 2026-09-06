#include <stdio.h>
#include <QString>
#include <QtNodes/Definitions>
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
		fprintf(stderr, "Processing node %d\n", nodeId);
		outputNodes.push_back(nodeId);
	    }
	
	}
    }
}

static void processNode(const QtNodes::DataFlowGraphModel & model, QtNodes::NodeId nodeId)
{
	auto connections = model.connections(nodeId, QtNodes::PortType::In, 0);

	QString nodeType = model.nodeData(nodeId, QtNodes::NodeRole::Type).value<QString>();
}


void evaluateToSCAD(const QtNodes::DataFlowGraphModel & model)
{
    fprintf(stderr, "Performing scene evaluation to OpenSCAD\n");

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
	processNode(model, nodeId);
    }

    // Next, for each output node, wire up all of the input ports upstream.
    // If a port has no connection, use a default data for that port.
    
    // Continue recursively until there are not more ancestor nodes.
    
}
