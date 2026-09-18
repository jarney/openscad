#include <stdio.h>
#include <QString>
#include <QtNodes/Definitions>

#include "nodes/Node.hpp"
#include "nodes/openscad/NodeProgramSerializerOpenSCAD.hpp"

using namespace JNodes::core;
using namespace JNodes::openscad;

static void
findOutputNodes(
    const NodeGraph & graph,
    std::vector<QtNodes::NodeId> & outputNodes
    )
{
    for (auto nodeId : graph.allNodeIds()) {
	Node *node = graph.delegateModel<Node>(nodeId);
	// Output nodes must have one input and no outputs.
	if ((node->nPorts(QtNodes::PortType::In) == 1) &&
	    (node->nPorts(QtNodes::PortType::Out) == 0)) {
	    outputNodes.push_back(nodeId);
	}
    }
}

static std::vector<QtNodes::ConnectionId> inputConnections(const NodeGraph & graph, QtNodes::NodeId nodeId)
{
    auto connections = graph.allConnectionIds(nodeId);
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
    const NodeGraph & graph,
    std::set<QtNodes::NodeId> & processed_nodes,    // Set of nodes that has already been processed.
    std::map<QtNodes::NodeId, NodePortData> & all_node_data,
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

    std::vector<QtNodes::ConnectionId> input_connections = inputConnections(graph, nodeId);
    std::vector<QtNodes::NodeId> connected_nodes = connectedNodes(input_connections);

    for (auto pred : connected_nodes) {
	processNode(graph, processed_nodes, all_node_data, pred, depth+1);
    }
    Node *node = graph.delegateModel<Node>(nodeId);

    NodePortData input_data;
    fprintf(stderr, "Processing input connections %ld\n", input_connections.size());
    for (auto & connection : input_connections) {
	Node *upstreamNode = graph.delegateModel<Node>(connection.outNodeId);
	std::string outputPortName = upstreamNode->outputPortName(connection.outPortIndex);
	
	std::string inputPortName = node->inputPortName(connection.inPortIndex);
	const NodePortData & connected_node_data = all_node_data[connection.outNodeId];
	if (connected_node_data.hasValue(outputPortName)) {
	    std::string outputValue = connected_node_data.getValue(outputPortName, "");
	    input_data.setValue(inputPortName, outputValue);
	    fprintf(stderr, "Node %d:%s => %d:%s = %s\n", connection.outNodeId, outputPortName.c_str(), nodeId, inputPortName.c_str(), outputValue.c_str());
	}
    }
    // Create a new entry and pass it by reference.
    auto & node_data = all_node_data[nodeId];
    node->process(input_data, node_data);
}


std::string
NodeProgramSerializerOpenSCAD::toString(const NodeGraph & graph)
{
//    virtual std::unordered_set<NodeId> allNodeIds() const = 0;
    std::vector<QtNodes::NodeId> outputNodes;

    // First, find all of the 'output' nodes.
    findOutputNodes(graph, outputNodes);

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
    std::map<QtNodes::NodeId, NodePortData> all_node_data;

    std::string output;
    for (auto nodeId : outputNodes) {
	// For each output node, wire up all of the input ports upstream.
	// If a port has no connection, use a default data for that port.
	// Continue recursively until there are not more ancestor nodes.
	processNode(graph, processed_nodes, all_node_data, nodeId, 0);
	output += all_node_data[nodeId].getValue("out", "");
    }


    return output;
}


NodeProgramSerializerOpenSCAD::NodeProgramSerializerOpenSCAD()
{}

const NodeProgramSerializer &
NodeProgramSerializerOpenSCAD::instance()
{
    static NodeProgramSerializerOpenSCAD instance;
    return instance;
}
    

/**
 * Writes the given node program to
 * the given stream using the serialization
 * method of JSON output.
 */
void
NodeProgramSerializerOpenSCAD::write(const NodeProgram &program, std::ostream & output_stream) const
{
    const NodeGraph *graph = program.getGraph("main");
    if (!graph) {
	fprintf(stderr, "Cannot find main\n");
	return;
    }
    std::string scadOutput = toString(*graph);
    output_stream << scadOutput;
}
/**
 * Reads the input stream and fills in the (assumed empty)
 * node program based on the file content.
 */
void
NodeProgramSerializerOpenSCAD::read(NodeProgram & program, std::istream & input_stream) const
{
    fprintf(stderr, "Nothing done, this doesn't work yet.  The parse entrypoint will go here...\n");
}
