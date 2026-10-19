#include "Parser.h"
#include <variant>
#include <fstream>
#include <iostream>
#include "node--js/SerializerError.hpp"
#include "node--js/xml/SerializerXML.hpp"
#include "nodes/gui/GraphModelAdapter.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

/*************************************************************/
class ModuleInstantiationASTHandler {
public:
    ModuleInstantiationASTHandler() = default;
    ~ModuleInstantiationASTHandler() = default;

    virtual void handle(
	NodeModule & nodeModule,
	NodeGraph *currentGraph,
	NodeId parentNode,
	PortId parentAssignments,
	PortId parentFunctionDefinitions,
	PortId parentModuleDefinitions,
	PortId parentModuleInstantiation,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const = 0;
    
};

class ModuleNodeFactorySphere : public ModuleInstantiationASTHandler {
public:
    virtual void handle(
	NodeModule & nodeModule,
	NodeGraph *currentGraph,
	NodeId parentNode,
	PortId parentAssignments,
	PortId parentFunctionDefinitions,
	PortId parentModuleDefinitions,
	PortId parentModuleInstantiation,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const;
};

static void
setNodePosition(Node & node, float x, float y)
{
    ConnectionData & data = node.getMetadata().getMetadata(
	NodeJS::gui::METADATA_NAMESPACE_GRAPH_EDITOR
	);
    data.setValue("pos-x", std::to_string(x));
    data.setValue("pos-y", std::to_string(y));
}


void
ModuleNodeFactorySphere::handle(
	NodeModule & nodeModule,
	NodeGraph *currentGraph,
	NodeId parentNode,
	PortId parentAssignments,
	PortId parentFunctionDefinitions,
	PortId parentModuleDefinitions,
	PortId parentModuleInstantiation,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const
{
    // Create a new node and connect it to
    // our parent node with the output of the module's node
    // connected to the input of our parent's node.

    const NodeType *nodeType = nodeModule.getNodeType(std::string("org.ensor.nodejs.openscad.") + moduleInstantiation->name());
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", moduleInstantiation->name().c_str());
	return;
    }
    ConnectionData defaultData;
    Node & childNode = currentGraph->newNode(
	*nodeType,
	moduleInstantiation->name(),
	defaultData
	);
    
    setNodePosition(childNode, -depth * 400, -i * 400);

    currentGraph->newEdge(
	childNode.getId(), childNode.getOutputs().getName(0),
	parentNode, parentModuleInstantiation
	);

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
    processLocalScope(
	nodeModule,
	currentGraph,
	childNode.getId(),
	parentAssignments,
	parentFunctionDefinitions,
	parentModuleDefinitions,
	childNode.getType().getInputs().getName(3), // Module Instantiations
	moduleInstantiation->scope,
	context,
	depth);
	
}

static std::map<std::string, std::shared_ptr<ModuleInstantiationASTHandler>> moduleFactory;

void
processSourceFile(
    std::string filename,
    SerializerErrorReporter & err,
    NodeModule & nodeModule,
    SourceFile *sourceFile,
    const std::shared_ptr<const Context>& context
    )
{

    // Derive the package name from the input filename.
    std::string package;
    size_t ext_offset = filename.find_last_of('.');
    if (ext_offset != std::string::npos) {
	package = filename.substr(0, ext_offset);
    }
    else {
	package = filename;
    }
    nodeModule.setPackage(package);
    
    ContextHandle<FileContext> file_context{Context::create<FileContext>(context, sourceFile)};
    //////////// Initialization

    moduleFactory["sphere"] = std::make_shared<ModuleNodeFactorySphere>();
    moduleFactory["cube"] = moduleFactory["sphere"];
    moduleFactory["circle"] = moduleFactory["sphere"];
    moduleFactory["square"] = moduleFactory["sphere"];
    moduleFactory["for"] = moduleFactory["sphere"];

    // At this point, we need to load the
    // openscad.xml to load the type definitions for the
    // builtins.
    ModuleLoader & loader = nodeModule.getModuleLoader();

    const NodeModule *core_module = loader.loadModule("org.ensor.nodejs.core", err);
    if (!core_module) {
	// TODO: We should report errors better in this parser.
	fprintf(stderr, "Cannot load nodejs core module\n");
	return;
    }
    nodeModule.addImportModule(core_module);
    
    const NodeModule *openscad_module = loader.loadModule("org.ensor.nodejs.openscad", err);
    if (!openscad_module) {
	// TODO: We should report errors better in this parser.
	fprintf(stderr, "Cannot load openscad module\n");
	return;
    }
    nodeModule.addImportModule(openscad_module);

    ////////////
    NodeGraph *main = nodeModule.addGraph("main");

    // Add the mandatory input and ouptut nodes.
    const NodeType *outputType = nodeModule.getNodeType("org.ensor.nodejs.core.output");
    if (outputType == nullptr) {
	fprintf(stderr, "Invalid: node type org.ensor.nodejs.openscad.output not found\n");
	return;
    }
    ConnectionData defaultData;
    Node & outputNode = main->newNode(
	*outputType,
	"output",
	defaultData
	);

    // Add the ports to capture the data:
    outputNode.setOverrideInputs(true);
    NamedPorts & outputInputs = outputNode.getOverrideInputs();
    outputInputs.addPort("assignments", std::make_unique<NodePort>("org.ensor.nodejs.openscad.assignment", "Assignments", NodePort::ConnectionPolicy::Multiple));
    outputInputs.addPort("function-definitions", std::make_unique<NodePort>("org.ensor.nodejs.openscad.function-definition", "Functions", NodePort::ConnectionPolicy::Multiple));
    outputInputs.addPort("module-definitions", std::make_unique<NodePort>("org.ensor.nodejs.openscad.module-definition", "Modules", NodePort::ConnectionPolicy::Multiple));
    outputInputs.addPort("module-instantiations", std::make_unique<NodePort>("org.ensor.nodejs.openscad.module-instantiation", "Geometry", NodePort::ConnectionPolicy::Multiple));
    
    const NodeType *inputType = nodeModule.getNodeType("org.ensor.nodejs.core.input");
    if (inputType == nullptr) {
	fprintf(stderr, "Invalid: node type org.ensor.nodejs.openscad.input not found\n");
	return;
    }
    Node & inputNode = main->newNode(
	*inputType,
	"input",
	defaultData
	);

    // A source file is just a single large scope.
    processLocalScope(
	nodeModule,
	main,
	outputNode.getId(),
	outputNode.getInputs().getName(0), // Assignments
	outputNode.getInputs().getName(1), // Function Definitions
	outputNode.getInputs().getName(2), // Module Definitions
	outputNode.getInputs().getName(3), // Module Instantiations
	sourceFile->scope,
	*file_context,
	0);
}

void
processAssignment(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentAssignments,
    const std::shared_ptr<Assignment> & assignment,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    const NodeType *nodeType = nodeModule.getNodeType("org.ensor.nodejs.openscad.assign");
    ConnectionData defaultData;
    Node & assignmentNode = currentGraph->newNode(
	*nodeType,
	"assign",
	defaultData
	);
    setNodePosition(assignmentNode, -depth * 400, 0);

    // The output of an assignment needs to link up
    // with the output node's assignments port.
    currentGraph->newEdge(
	assignmentNode.getId(), assignmentNode.getType().getOutputs().getName(0),
	parentNode, parentAssignments
	);
    
    assignmentNode.getData().setValue("variable_name", assignment->getName().c_str());

    processExpression(nodeModule, currentGraph, assignmentNode.getId(), assignmentNode.getInputs().getName(0), assignment->getExpr(), context, depth+1);
}

void
processExpressionUnaryOp(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const UnaryOp *operation,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    std::map<UnaryOp::Op, std::string> operators{{UnaryOp::Op::Not, "org.ensor.nodejs.openscad.not"},
					   {UnaryOp::Op::Negate, "org.ensor.nodejs.openscad.negate"},
					   {UnaryOp::Op::BinaryNot, "org.ensor.nodejs.openscad.tilde"}};
    const auto op_it = operators.find(operation->op);
    if (op_it == operators.end()) {
	throw std::string("Invalid literal type found parsing openscad file\n");
    }

    const NodeType *nodeType = nodeModule.getNodeType(op_it->second);
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", op_it->second.c_str());
	return;
    }
    ConnectionData defaultData;
    Node & childNode = currentGraph->newNode(
	*nodeType,
	op_it->second,
	defaultData
	);
    setNodePosition(childNode, -depth * 400, 0);

    currentGraph->newEdge(
	childNode.getId(), childNode.getType().getOutputs().getName(0),
	parentNode, parentPort
	);

    processExpression(nodeModule, currentGraph, childNode.getId(), childNode.getInputs().getName(0), operation->expr, context, depth+1);
    
}

void
processExpressionBinaryOp(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const BinaryOp *operation,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    
    std::map<BinaryOp::Op, std::string> operators{
	{BinaryOp::Op::LogicalAnd, "org.ensor.nodejs.openscad.and"},
	{BinaryOp::Op::LogicalOr, "org.ensor.nodejs.openscad.or"},
	{BinaryOp::Op::Exponent, "org.ensor.nodejs.openscad.exponentiate"},
	{BinaryOp::Op::Multiply, "org.ensor.nodejs.openscad.multiply"},
	{BinaryOp::Op::Divide, "org.ensor.nodejs.openscad.divide"},
	{BinaryOp::Op::Modulo, "org.ensor.nodejs.openscad.modulo"},
	{BinaryOp::Op::Plus, "org.ensor.nodejs.openscad.add"},
	{BinaryOp::Op::Minus, "org.ensor.nodejs.openscad.subtract"},
	{BinaryOp::Op::ShiftLeft, "org.ensor.nodejs.openscad.binary_shl"},
	{BinaryOp::Op::ShiftRight, "org.ensor.nodejs.openscad.binary_shr"},
	{BinaryOp::Op::BinaryAnd, "org.ensor.nodejs.openscad.binary_and"},
	{BinaryOp::Op::BinaryOr, "org.ensor.nodejs.openscad.binary_or"},
	{BinaryOp::Op::Less, "org.ensor.nodejs.openscad.lt"},
	{BinaryOp::Op::LessEqual, "org.ensor.nodejs.openscad.leq"},
	{BinaryOp::Op::Greater, "org.ensor.nodejs.openscad.gt"},
	{BinaryOp::Op::GreaterEqual, "org.ensor.nodejs.openscad.geq"},
	{BinaryOp::Op::Equal, "org.ensor.nodejs.openscad.eq"},
	{BinaryOp::Op::NotEqual, "org.ensor.nodejs.openscad.neq"}
    };
    
    const auto op_it = operators.find(operation->op);
    if (op_it == operators.end()) {
	throw std::string("Invalid literal type found parsing openscad file\n");
    }

    const NodeType *nodeType = nodeModule.getNodeType(op_it->second);
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", op_it->second.c_str());
	return;
    }
    ConnectionData defaultData;
    Node & childNode = currentGraph->newNode(
	*nodeType,
	op_it->second,
	defaultData
	);
    setNodePosition(childNode, -depth * 400, 0);

    currentGraph->newEdge(
	childNode.getId(), childNode.getType().getOutputs().getName(0),
	parentNode, parentPort
	);
    
    processExpression(nodeModule, currentGraph, childNode.getId(), childNode.getInputs().getName(0), operation->left, context, depth+1);
    processExpression(nodeModule, currentGraph, childNode.getId(), childNode.getInputs().getName(1), operation->right, context, depth+1);
}

void
processExpressionLiteral(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const Literal *literal,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    Node *childNode;
    if (literal->isBool()) {
	std::string nodeTypeName = literal->toBool() ? "org.ensor.nodejs.openscad.true" : "org.ensor.nodejs.openscad.false";
	const NodeType *nodeType = nodeModule.getNodeType(nodeTypeName);
	if (nodeType == nullptr) {
	    fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
	    return;
	}
	ConnectionData defaultData;
	childNode = &currentGraph->newNode(
	    *nodeType,
	    nodeTypeName,
	    defaultData
	    );
    }
    else if (literal->isString()) {
	std::string nodeTypeName = "org.ensor.nodejs.openscad.const_string";
	const NodeType *nodeType = nodeModule.getNodeType(nodeTypeName);
	if (nodeType == nullptr) {
	    fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
	    return;
	}
	ConnectionData defaultData;
	defaultData.setValue("value", literal->toString());
	childNode = &currentGraph->newNode(
	    *nodeType,
	    nodeTypeName,
	    defaultData);
    }
    else if (literal->isDouble()) {
	double lit_double = literal->toDouble();
	long lit_long = (long)lit_double;
	double lit_recast = (double)lit_long;
	if (abs(lit_double - lit_recast) < 1e-9) {
	    std::string nodeTypeName = "org.ensor.nodejs.openscad.const_int";
	    const NodeType *nodeType = nodeModule.getNodeType(nodeTypeName);
	    if (nodeType == nullptr) {
		fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
		return;
	    }
	    fprintf(stderr, "Handling lit_long %ld\n", lit_long);
	    ConnectionData defaultData;
	    defaultData.setValue("value", std::to_string(lit_long));
	    childNode = &currentGraph->newNode(
		*nodeType,
		nodeTypeName,
		defaultData);
	    
	}
	else {
	    std::string nodeTypeName = "org.ensor.nodejs.openscad.const_float";
	    const NodeType *nodeType = nodeModule.getNodeType(nodeTypeName);
	    if (nodeType == nullptr) {
		fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
		return;
	    }
	    ConnectionData defaultData;
	    defaultData.setValue("value", std::to_string(lit_double));
	    childNode = &currentGraph->newNode(
		*nodeType,
		nodeTypeName,
		defaultData);
	}
    }
    else if (literal->isUndefined()) {
	std::string nodeTypeName = "org.ensor.nodejs.openscad.undef";
	const NodeType *nodeType = nodeModule.getNodeType(nodeTypeName);
	if (nodeType == nullptr) {
	    fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
	    return;
	}
	ConnectionData defaultData;
	childNode = &currentGraph->newNode(
	    *nodeType,
	    nodeTypeName,
	    defaultData);
    }
    else {
	throw std::string("Invalid literal type found parsing openscad file\n");
    }

    setNodePosition(*childNode, -depth * 400, 0);

    currentGraph->newEdge(
	childNode->getId(), childNode->getType().getOutputs().getName(0),
	parentNode, parentPort
	);
    
}

void
processExpressionLookup(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const Lookup *lookup,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    std::string nodeTypeName = "org.ensor.nodejs.openscad.variable";
    const NodeType *nodeType = nodeModule.getNodeType(nodeTypeName);
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
	return;
    }
    ConnectionData defaultData;
    defaultData.setValue("variable_name", lookup->get_name());
    Node *childNode = &currentGraph->newNode(
	*nodeType,
	nodeTypeName,
	defaultData);
    
    setNodePosition(*childNode, -depth * 400, 0);
    
    currentGraph->newEdge(
	childNode->getId(), childNode->getType().getOutputs().getName(0),
	parentNode, parentPort
	);
}

void
processExpressionBuiltinFunctionCall(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const FunctionCall *functionCall,
    const BuiltinFunction *builtinFunction,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    // Map between the OpenSCAD function's
    // arguments and the node's inputs,
    // calling evaluations for the evaluations
    // as needed.
    
    // TODO:
    // * Look up the function's node in the node graph so we have the
    //   names of the inputs along with their indices.
    // * Look up the function's entry in the context so we know what
    //   arguments to parse for.
    std::string nodeTypeName  = std::string("org.ensor.nodejs.openscad.") + functionCall->name;
    const NodeType *nodeType = nodeModule.getNodeType(nodeTypeName);
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
	return;
    }

    Node *functionCallNode = &currentGraph->newNode(
	*nodeType,
	nodeTypeName
	);
    setNodePosition(*functionCallNode, -depth * 400, 0);
    
    currentGraph->newEdge(
	functionCallNode->getId(), functionCallNode->getType().getOutputs().getName(0),
	parentNode, parentPort
	);

    int i = 0;
    for (const std::shared_ptr<Assignment> & assignment : functionCall->arguments) {
	fprintf(stderr, "Setting argument %s\n", assignment->getName().c_str());
	// Now, for each of the arguments,
	// we need to evaluate the expression it corresponds to
	processExpression(
	    nodeModule,
	    currentGraph,
	    functionCallNode->getId(),
	    functionCallNode->getInputs().getName(i),
	    assignment->getExpr(),
	    context,
	    depth + 1
	    );
	i++;
    }
}

void
processExpressionCallableUserFunction(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const FunctionCall *functionCall,
    const CallableUserFunction & callableUserFunction,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    const NodeType *nodeType = nodeModule.getNodeType("org.ensor.nodejs.openscad.function-call");
    if (nodeType == nullptr) {
	fprintf(stderr, "Error: no such node type function-call\n");
	return;
    }

    const UserFunction *function = callableUserFunction.function;
    
    ConnectionData nodeData;
    nodeData.setValue("function-name", function->name);
    
    Node *functionCallNode = &currentGraph->newNode(
	*nodeType,
	function->name,
	nodeData
	);
    setNodePosition(*functionCallNode, -depth * 400, 0);
    functionCallNode->setOverrideInputs(true);
    
    currentGraph->newEdge(
	functionCallNode->getId(), functionCallNode->getType().getOutputs().getName(0),
	parentNode, parentPort
	);
    NamedPorts & functionInputs = functionCallNode->getOverrideInputs();

    for (const std::shared_ptr<Assignment> & assignment : function->parameters) {
	std::unique_ptr<NodePort> argumentPort = std::make_unique<NodePort>("org.ensor.nodejs.openscad.variable", assignment->getName(), NodePort::ConnectionPolicy::One);
	functionInputs.addPort(assignment->getName(), std::move(argumentPort));
    }

    for (const std::shared_ptr<Assignment> & assignment : functionCall->arguments) {
	fprintf(stderr, "Setting argument %s\n", assignment->getName().c_str());
	// Now, for each of the arguments,
	// we need to evaluate the expression it corresponds to
	processExpression(
	    nodeModule,
	    currentGraph,
	    functionCallNode->getId(),
	    assignment->getName(),
	    assignment->getExpr(),
	    context,
	    depth + 1
	    );
    }
    
}


void
processExpressionFunctionCall(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const FunctionCall *functionCall,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    boost::optional<CallableFunction> scad_function;
    
    scad_function = context->lookup_function(functionCall->name, functionCall->location());
    if (!scad_function) {
	fprintf(stderr, "No such function %s\n", functionCall->name.c_str());
	throw std::string("Invalid function %s\n", functionCall->name.c_str());
    }
    else if (std::holds_alternative<const BuiltinFunction *>(*scad_function)) {
	fprintf(stderr, "Builtin function %s\n", functionCall->name.c_str());
	const BuiltinFunction *builtinFunction = std::get<const BuiltinFunction *>(*scad_function);
	processExpressionBuiltinFunctionCall(
	    nodeModule,
	    currentGraph,
	    parentNode,
	    parentPort,
	    functionCall,
	    builtinFunction,
	    context,
	    depth+1
	    );
    }
    else if (std::holds_alternative<CallableUserFunction>(*scad_function)) {
	const CallableUserFunction & callableUserFunction = std::get<CallableUserFunction>(*scad_function);
	processExpressionCallableUserFunction(
	    nodeModule,
	    currentGraph,
	    parentNode,
	    parentPort,
	    functionCall,
	    callableUserFunction,
	    context,
	    depth+1
	    );
    }
    else if (std::holds_alternative<Value>(*scad_function)) {
	fprintf(stderr, "Value %s\n", functionCall->name.c_str());
	throw std::string("Callable values not yet supported\n");
    }
    else if (std::holds_alternative<const Value*>(*scad_function)) {
	fprintf(stderr, "Value pointer %s\n", functionCall->name.c_str());
	throw std::string("Value pointers not yet supported\n");
    }
    else {
	throw std::string("Unknown variant of callable function\n");
    }

    
}

void
processExpression(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const std::shared_ptr<Expression> & expression,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    // TODO: Switch based on expression type and recursively handle expressions by their type...
    Expression *e = expression.get();
    if (dynamic_cast<UnaryOp*>(e)) {
	processExpressionUnaryOp(nodeModule, currentGraph, parentNode, parentPort, dynamic_cast<UnaryOp*>(e), context, depth);
    }
    else if (dynamic_cast<BinaryOp*>(e)) {
	processExpressionBinaryOp(nodeModule, currentGraph, parentNode, parentPort, dynamic_cast<BinaryOp*>(e), context, depth);
    }
    else if (dynamic_cast<TernaryOp*>(e)) {
    }
    else if (dynamic_cast<ArrayLookup*>(e)) {
    }
    else if (dynamic_cast<Literal*>(e)) {
	processExpressionLiteral(nodeModule, currentGraph, parentNode, parentPort, dynamic_cast<Literal*>(e), context, depth);
    }
    else if (dynamic_cast<Vector*>(e)) {
	// Vectors are going to be hard because we don't have a 'varargs' version
	// of a node, so we'll have to do it with a 'list append' node.
	// We'll do it with a 2,3,many approach since 1,2,3,4 are the most common by far.
    }
    else if (dynamic_cast<Lookup*>(e)) {
	processExpressionLookup(nodeModule, currentGraph, parentNode, parentPort, dynamic_cast<Lookup*>(e), context, depth);
    }
    else if (dynamic_cast<MemberLookup*>(e)) {
    }
    else if (dynamic_cast<FunctionCall*>(e)) {
	processExpressionFunctionCall(nodeModule, currentGraph, parentNode, parentPort, dynamic_cast<FunctionCall*>(e), context, depth);
    }
    else if (dynamic_cast<FunctionDefinition*>(e)) {
    }
    else if (dynamic_cast<Assert*>(e)) {
    }
    else if (dynamic_cast<Echo*>(e)) {
    }
    else if (dynamic_cast<Let*>(e)) {
    }
    else if (dynamic_cast<LcIf*>(e)) {
    }
    else if (dynamic_cast<LcFor*>(e)) {
    }
    else if (dynamic_cast<LcForC*>(e)) {
    }
    else if (dynamic_cast<LcEach*>(e)) {
    }
    else if (dynamic_cast<LcLet*>(e)) {
    }
    
}


void
processFunctionDefinition(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentFunctionDefinitions,
    std::shared_ptr<UserFunction> functionDefinition,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{

    // Create the graph (this is like the function body)
    NodeGraph *functionGraph = nodeModule.addGraph(functionDefinition->name);

    const NodeType *functionOutputNodeType = nodeModule.getNodeType("org.ensor.nodejs.openscad.function_output");
    Node & functionOutputNode = functionGraph->newNode(*functionOutputNodeType, "function_output", ConnectionData());
    processExpression(nodeModule,
		      functionGraph,
		      functionOutputNode.getId(),
		      functionOutputNodeType->getInputs().getName(0),
		      functionDefinition->expr,
		      context,
		      depth+1);

    // Create the function definition in the parent's graph.
    const NodeType *functionDefinitionNodeType = nodeModule.getNodeType("org.ensor.nodejs.openscad.function");
    ConnectionData functionDefinitionData;
    functionDefinitionData.setValue("graph", functionDefinition->name);
    Node & functionDefinitionNode = currentGraph->newNode(*functionDefinitionNodeType, "function", functionDefinitionData);
    setNodePosition(functionDefinitionNode, -depth * 400, 0);
    functionDefinitionNode.setOverrideInputs(true);

    currentGraph->newEdge(
	functionDefinitionNode.getId(), functionDefinitionNodeType->getOutputs().getName(0),
	parentNode, parentFunctionDefinitions
	);

    NamedPorts & functionDefinitionDefaults = functionDefinitionNode.getOverrideInputs();
    
    for (const auto & assignment : functionDefinition->parameters) {
	std::unique_ptr<NodePort> port = std::make_unique<NodePort>("org.ensor.nodejs.openscad.variable", assignment->getName(), NodePort::ConnectionPolicy::One);
	functionDefinitionDefaults.addPort(assignment->getName(), std::move(port));

	processExpression(
	    nodeModule,
	    currentGraph,
	    functionDefinitionNode.getId(),
	    assignment->getName(),
	    assignment->getExpr(),
	    context,
	    depth+1
	    );

	}

}

void
processModuleDefinition(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentFunctionDefinitions,
    std::shared_ptr<UserModule> moduleDefinition,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
}


void
processLocalScope(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentAssignments,
    PortId parentFunctionDefinitions,
    PortId parentModuleDefinitions,
    PortId parentModuleInstantiations,
    std::shared_ptr<LocalScope> localScope,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    // Order is important here.  Functions get defined first,
    // then modules, and finally we can define the variables and
    // instantiate modules.

    fprintf(stderr, "Assignments %s\n", parentAssignments.c_str());
    
    // Next, process any function definitions
    for (const auto functionDefinition : localScope->getUserFunctions()) {
	processFunctionDefinition(
	    nodeModule,
	    currentGraph,
	    parentNode,
	    parentFunctionDefinitions,
	    functionDefinition.second,
	    context, depth + 1);
    }

    // Next, process any module definitions
    for (const auto moduleDefinition : localScope->getUserModules()) {
	processModuleDefinition(
	    nodeModule,
	    currentGraph,
	    parentNode,
	    parentFunctionDefinitions,
	    moduleDefinition.second,
	    context, depth + 1);
    }
	    

    // First process any variable assignments in this scope.
    for (const auto assignment : localScope->assignments) {
	processAssignment(
	    nodeModule,
	    currentGraph,
	    parentNode,
	    parentAssignments,
	    assignment,
	    context, depth + 1);
    }

    // Finally instantiate any modules
    int i = 0;
    for (const auto moduleInstantiation : localScope->moduleInstantiations) {
	processModuleInstantiation(
	    nodeModule,
	    currentGraph,
	    parentNode,
	    parentAssignments,
	    parentFunctionDefinitions,
	    parentModuleDefinitions,
	    parentModuleInstantiations,
	    moduleInstantiation,
	    context,
	    depth+1,
	    i);
	i++;
    }
}

void
processModuleInstantiation(
    NodeModule & nodeModule,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentAssignments,
    PortId parentFunctionDefinitions,
    PortId parentModuleDefinitions,
    PortId parentModuleInstantiations,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    const std::shared_ptr<const Context>& context,
    int depth,
    int i
    )
{
    const auto it = moduleFactory.find(moduleInstantiation->name());
    if (it != moduleFactory.end()) {
	it->second->handle(
	    nodeModule,
	    currentGraph,
	    parentNode,
	    parentAssignments,
	    parentFunctionDefinitions,
	    parentModuleDefinitions,
	    parentModuleInstantiations,
	    moduleInstantiation,
	    context,
	    depth,
	    i);
    }
    else {
	fprintf(stderr, "Un-handled module instantiation %s\n", moduleInstantiation->name().c_str());
    }
}
