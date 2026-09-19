#include <stdio.h>
#include <QString>
#include <QtNodes/Definitions>

#include "nodes/Node.hpp"
#include "nodes/openscad/NodeProgramSerializerOpenSCAD.hpp"

#include "openscad.h"

#include "core/SourceFile.h"
#include "core/LocalScope.h"
#include "core/ModuleInstantiation.h"
#include "core/Assignment.h"
#include "core/Expression.h"
#include "core/Parameters.h"
#include "core/Context.h"
#include "core/BuiltinContext.h"

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

static std::vector<QtNodes::ConnectionId>
inputConnections(const NodeGraph & graph, QtNodes::NodeId nodeId)
{
    auto connections = graph.allConnectionIds(nodeId);
    std::vector<QtNodes::ConnectionId> input_connections;
    for (auto connection : connections) {
	if (nodeId == connection.outNodeId) continue;
	input_connections.push_back(connection);
    }
    return input_connections;
}

static std::vector<QtNodes::NodeId>
connectedNodes(const std::vector<QtNodes::ConnectionId> & connections)
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
    processed_nodes.insert(nodeId);

    std::vector<QtNodes::ConnectionId> input_connections = inputConnections(graph, nodeId);
    std::vector<QtNodes::NodeId> connected_nodes = connectedNodes(input_connections);

    for (auto pred : connected_nodes) {
	processNode(graph, processed_nodes, all_node_data, pred, depth+1);
    }
    Node *node = graph.delegateModel<Node>(nodeId);

    NodePortData input_data;
    for (auto & connection : input_connections) {
	Node *upstreamNode = graph.delegateModel<Node>(connection.outNodeId);
	std::string outputPortName = upstreamNode->outputPortName(connection.outPortIndex);
	
	std::string inputPortName = node->inputPortName(connection.inPortIndex);
	const NodePortData & connected_node_data = all_node_data[connection.outNodeId];
	if (connected_node_data.hasValue(outputPortName)) {
	    std::string outputValue = connected_node_data.getValue(outputPortName, "");
	    input_data.setValue(inputPortName, outputValue);
	    //fprintf(stderr, "Node %d:%s => %d:%s = %s\n", connection.outNodeId, outputPortName.c_str(), nodeId, inputPortName.c_str(), outputValue.c_str());
	}
    }
    // Create a new entry and pass it by reference.
    auto & node_data = all_node_data[nodeId];
    node->process(input_data, node_data);
}


std::string
NodeProgramSerializerOpenSCAD::toString(const NodeGraph & graph)
{
    std::vector<QtNodes::NodeId> outputNodes;

    // First, find all of the 'output' nodes.
    findOutputNodes(graph, outputNodes);

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

void processSourceFile(
    NodeProgram & program,
    SourceFile *sourceFile,
    const std::shared_ptr<const Context>& context
    );

void processLocalScope(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<LocalScope> localScope,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void processModuleInstantiation(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    const std::shared_ptr<const Context>& context,
    int depth,
    int i
    );


/**
 * Reads the input stream and fills in the (assumed empty)
 * node program based on the file content.
 */
int
NodeProgramSerializerOpenSCAD::read(NodeProgram & program, std::istream & input_stream) const
{
    std::string fulltext(std::istreambuf_iterator<char>(input_stream), {});

    SourceFile *sourceFile;
    std::string fname("none");
    
    sourceFile = parse(
	sourceFile,
	fulltext,
	fname,
	fname,
	false) ? sourceFile : nullptr;
    if (!sourceFile) {
	fprintf(stderr, "Unsuccessful parse\n");
	return -1;
    }
    fprintf(stderr, "Got a valid parse tree\n");

    EvaluationSession session{sourceFile->getFullpath()};
    ContextHandle<BuiltinContext> builtin_context{Context::create<BuiltinContext>(&session)};
    
    processSourceFile(program, sourceFile, *builtin_context);

    return 0;
}

/*************************************************************/
class ModuleInstantiationASTHandler {
public:
    ModuleInstantiationASTHandler() = default;
    ~ModuleInstantiationASTHandler() = default;

    virtual void handle(
	NodeProgram & program,
	NodeGraph *currentGraph,
	QtNodes::NodeId parentNode,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const = 0;
    
};

class ModuleNodeFactorySphere : public ModuleInstantiationASTHandler {
public:
    virtual void handle(
	NodeProgram & program,
	NodeGraph *currentGraph,
	QtNodes::NodeId parentNode,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const;
};

void
ModuleNodeFactorySphere::handle(
	NodeProgram & program,
	NodeGraph *currentGraph,
	QtNodes::NodeId parentNode,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const
{
    // Create a new node and connect it to
    // our parent node with the output of the module's node
    // connected to the input of our parent's node.
    QtNodes::NodeId childNode = currentGraph->addNode(QString::fromStdString(moduleInstantiation->name()));
    QPointF pos(-depth * 400, -i * 400);
    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);

    QtNodes::ConnectionId connection;
    connection.inNodeId = parentNode;
    connection.inPortIndex = 0;
    connection.outNodeId = childNode;
    connection.outPortIndex = 0;
    currentGraph->addConnection(connection);

    // First, we parse the arguments to get the
    // list of parameters to the module.
//  static Parameters parse(Arguments arguments, const Location& loc,
//                          const std::vector<std::string>& required_parameters,
//                          const std::vector<std::string>& optional_parameters = {});
    const std::vector<std::string> sphere_required{"r"};
    const std::vector<std::string> sphere_optional{"d"};
    
    Parameters parameters = Parameters::parse(
	Arguments(moduleInstantiation->arguments, context),
	moduleInstantiation->location(),
	sphere_required,
	sphere_optional);
    fprintf(stderr, "Parsed parametrs\n");
    
    
    // Next, we handle the suff in the curly-braces
    // that is the body of the node.
    processLocalScope(program, currentGraph, childNode, moduleInstantiation->scope, context, depth);
	
}

static std::map<std::string, std::shared_ptr<ModuleInstantiationASTHandler>> moduleFactory;

void
processSourceFile(
    NodeProgram & program,
    SourceFile *sourceFile,
    const std::shared_ptr<const Context>& context
    )
{

    //////////// Initialization

    moduleFactory["sphere"] = std::make_shared<ModuleNodeFactorySphere>();
    moduleFactory["cube"] = moduleFactory["sphere"];
    moduleFactory["for"] = moduleFactory["sphere"];
    
    ////////////
    NodeGraph *main = program.newGraph("main");
    QtNodes::NodeId outputNode = main->addNode("output");

    // A source file is just a single large scope.
    processLocalScope(program, main, outputNode, sourceFile->scope, context, 0);
}

void
processLocalScope(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<LocalScope> localScope,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{

    // First process any variable assignments in this scope.

    // Next, process any function definitions

    // Next, process any module definitions

    // We probably want to push the context after we do this so we can perform
    // lookups in terms of the new context.

    
    // Finally instantiate any modules
    int i = 0;
    for (const auto moduleInstantiation : localScope->moduleInstantiations) {
	processModuleInstantiation(program, currentGraph, parentNode, moduleInstantiation, context, depth+1, i);
	i++;
    }
}

void
processModuleInstantiation(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    const std::shared_ptr<const Context>& context,
    int depth,
    int i
    )
{
    const auto it = moduleFactory.find(moduleInstantiation->name());
    if (it != moduleFactory.end()) {
	it->second->handle(program, currentGraph, parentNode, moduleInstantiation, context, depth, i);
    }
    else {
	fprintf(stderr, "Un-handled module instantiation %s\n", moduleInstantiation->name().c_str());
    }
    
#if 0
    auto as = mi->arguments.at(0);
    Expression *expr = as->getExpr().get();
    
    Lookup *lit = dynamic_cast<Lookup*>(expr);
    if (lit) {
	fprintf(stderr, "It is a literal %s\n", lit->get_name().c_str());
    }
    else {
	fprintf(stderr, "It is not a literal\n");
    }
#endif
}
