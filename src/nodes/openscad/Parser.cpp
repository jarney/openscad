#include "Parser.h"
#include <variant>
#include <fstream>
#include <iostream>
#include "node--js/SerializerError.hpp"
#include "node--js/xml/SerializerXML.hpp"

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

    const NodeType *nodeType = currentGraph->getNodeType(moduleInstantiation->name());
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
    childNode.setPosition(std::make_pair(-depth * 400, -i * 400));
    
    currentGraph->newEdge(
	childNode.getId(), childNode.getType().getOutputs().getName(0),
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
    SerializerErrorReporter & err,
    NodeModule & nodeModule,
    SourceFile *sourceFile,
    const std::shared_ptr<const Context>& context
    )
{

    ContextHandle<FileContext> file_context{Context::create<FileContext>(context, sourceFile)};
    //////////// Initialization

    moduleFactory["sphere"] = std::make_shared<ModuleNodeFactorySphere>();
    moduleFactory["cube"] = moduleFactory["sphere"];
    moduleFactory["for"] = moduleFactory["sphere"];

    // At this point, we need to load the
    // openscad.xml to load the type definitions for the
    // builtins.
    ModuleLoader & loader = nodeModule.getModuleLoader();

    const NodeModule *openscad_module = loader.loadModule("org.ensor.nodejs.openscad", err);

    ////////////
    NodeGraph *main = nodeModule.addGraph("main");
    main->addScope(openscad_module);
    
    const NodeType *outputType = main->getNodeType("output");
    if (outputType == nullptr) {
	fprintf(stderr, "Invalid output port, not found\n");
	return;
    }
    ConnectionData defaultData;
    Node & outputNode = main->newNode(
	*outputType,
	"output",
	defaultData
	);

    // A source file is just a single large scope.
    processLocalScope(
	nodeModule,
	main,
	outputNode.getId(),
	outputNode.getType().getInputs().getName(0), // Assignments
	outputNode.getType().getInputs().getName(1), // Function Definitions
	outputNode.getType().getInputs().getName(2), // Module Definitions
	outputNode.getType().getInputs().getName(3), // Module Instantiations
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
    const NodeType *nodeType = currentGraph->getNodeType("assign");
    ConnectionData defaultData;
    Node & assignmentNode = currentGraph->newNode(
	*nodeType,
	"assign",
	defaultData
	);
    assignmentNode.setPosition(std::make_pair(-depth * 400, 0));

    // The output of an assignment needs to link up
    // with the output node's assignments port.
    currentGraph->newEdge(
	assignmentNode.getId(), assignmentNode.getType().getOutputs().getName(0),
	parentNode, parentAssignments
	);
    
    assignmentNode.getData().setValue("variable_name", assignment->getName().c_str());

    processExpression(nodeModule, currentGraph, assignmentNode.getId(), assignmentNode.getType().getInputs().getName(0), assignment->getExpr(), context, depth+1);
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
    std::map<UnaryOp::Op, std::string> operators{{UnaryOp::Op::Not, "not"},
					   {UnaryOp::Op::Negate, "negate"},
					   {UnaryOp::Op::BinaryNot, "tilde"}};
    const auto op_it = operators.find(operation->op);
    if (op_it == operators.end()) {
	throw std::string("Invalid literal type found parsing openscad file\n");
    }

    const NodeType *nodeType = currentGraph->getNodeType(op_it->second);
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
    childNode.setPosition(std::make_pair(-depth * 400, 0));

    currentGraph->newEdge(
	childNode.getId(), childNode.getType().getOutputs().getName(0),
	parentNode, parentPort
	);

    processExpression(nodeModule, currentGraph, childNode.getId(), childNode.getType().getInputs().getName(0), operation->expr, context, depth+1);
    
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
	{BinaryOp::Op::LogicalAnd, "and"},
	{BinaryOp::Op::LogicalOr, "or"},
	{BinaryOp::Op::Exponent, "exponentiate"},
	{BinaryOp::Op::Multiply, "multiply"},
	{BinaryOp::Op::Divide, "divide"},
	{BinaryOp::Op::Modulo, "modulo"},
	{BinaryOp::Op::Plus, "add"},
	{BinaryOp::Op::Minus, "subtract"},
	{BinaryOp::Op::ShiftLeft, "binary_shl"},
	{BinaryOp::Op::ShiftRight, "binary_shr"},
	{BinaryOp::Op::BinaryAnd, "binary_and"},
	{BinaryOp::Op::BinaryOr, "binary_or"},
	{BinaryOp::Op::Less, "lt"},
	{BinaryOp::Op::LessEqual, "leq"},
	{BinaryOp::Op::Greater, "gt"},
	{BinaryOp::Op::GreaterEqual, "geq"},
	{BinaryOp::Op::Equal, "eq"},
	{BinaryOp::Op::NotEqual, "neq"}
    };
    
    const auto op_it = operators.find(operation->op);
    if (op_it == operators.end()) {
	throw std::string("Invalid literal type found parsing openscad file\n");
    }

    const NodeType *nodeType = currentGraph->getNodeType(op_it->second);
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
    childNode.setPosition(std::make_pair(-depth * 400, 0));

    currentGraph->newEdge(
	childNode.getId(), childNode.getType().getOutputs().getName(0),
	parentNode, parentPort
	);
    
    processExpression(nodeModule, currentGraph, childNode.getId(), childNode.getType().getInputs().getName(0), operation->left, context, depth+1);
    processExpression(nodeModule, currentGraph, childNode.getId(), childNode.getType().getInputs().getName(1), operation->right, context, depth+1);
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
	std::string nodeTypeName = literal->toBool() ? "true" : "false";
	const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
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
	std::string nodeTypeName = "const_string";
	const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
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
	    std::string nodeTypeName = "const_int";
	    const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
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
	    std::string nodeTypeName = "const_float";
	    const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
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
	std::string nodeTypeName = "undef";
	const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
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

    childNode->setPosition(std::make_pair(-depth * 400, 0));

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
    std::string nodeTypeName = "variable";
    const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
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
    
    childNode->setPosition(std::make_pair(-depth * 400, 0));
    
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
    std::string nodeTypeName  = functionCall->name;
    const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
	return;
    }

    Node *childNode = &currentGraph->newNode(
	*nodeType,
	nodeTypeName
	);
    childNode->setPosition(std::make_pair(-depth * 400, 0));
    
    currentGraph->newEdge(
	childNode->getId(), childNode->getType().getOutputs().getName(0),
	parentNode, parentPort
	);
    
#if 0
    Parameters parameters = Parameters::parse(
	Arguments(moduleInstantiation->arguments, context),
	moduleInstantiation->location(),
	sphere_required,
	sphere_optional);
						      
    processExpression(nodeModule, currentGraph, childNode, 0, operation->left, context, depth+1);
    processExpression(nodeModule, currentGraph, childNode, 1, operation->right, context, depth+1);
#endif
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
    const NodeType *nodeType = currentGraph->getNodeType("function-call");
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
    functionCallNode->setPosition(std::make_pair(-depth * 400, 0));
    functionCallNode->setOverrideInputs(true);
    
    currentGraph->newEdge(
	functionCallNode->getId(), functionCallNode->getType().getOutputs().getName(0),
	parentNode, parentPort
	);
    NamedPorts & functionInputs = functionCallNode->getOverrideInputs();

    for (const std::shared_ptr<Assignment> & assignment : function->parameters) {
	std::unique_ptr<NodePort> argumentPort = std::make_unique<NodePort>("variable", assignment->getName(), NodePort::ConnectionPolicy::One);
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
    
#if 0
    std::string name;
  AssignmentList parameters;
  std::shared_ptr<Expression> expr;
#endif
  fprintf(stderr, "Handling function call\n");
  
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
    // Create the node type (this is like a function prototype)
#if 0
    std::unique_ptr<NodeType> nodeType = std::make_unique<NodeType>();
    nodeType->setId(functionDefinition->name);
    nodeType->setVisibility(NodeType::Visibility::PRIVATE);
    nodeType->setType(NodeType::Type::GRAPH);
    for (const auto & assignment : functionDefinition->parameters) {
	std::unique_ptr<NodePort> port = std::make_unique<NodePort>("variable", assignment->getName(), NodePort::ConnectionPolicy::One);
	nodeType->getInputs().addPort(assignment->getName(), std::move(port));
    }
    std::unique_ptr<NodePort> outputPort = std::make_unique<NodePort>("variable", "out", NodePort::ConnectionPolicy::One);
    nodeType->getOutputs().addPort("out", std::move(outputPort));
    
    // Add this type to the 'currentGraph' scope
    // because we might want to call the function
    // in the same scope where it was defined (or below);
    //currentGraph->addScope(nodeType.get());
    fprintf(stderr, "Registering node type %p in module %p\n",
	    nodeType.get(), &nodeModule);
    
    nodeModule.addNodeType(std::move(nodeType));
#endif

    // Create the graph (this is like the function body)
    NodeGraph *functionGraph = nodeModule.addGraph(functionDefinition->name);
    functionGraph->copyScope(currentGraph);

    const NodeType *functionOutputNodeType = functionGraph->getNodeType("function_output");
    Node & functionOutputNode = functionGraph->newNode(*functionOutputNodeType, "function_output", ConnectionData());
    processExpression(nodeModule,
		      functionGraph,
		      functionOutputNode.getId(),
		      functionOutputNodeType->getInputs().getName(0),
		      functionDefinition->expr,
		      context,
		      depth+1);

    // Create the function definition in the parent's graph.
    const NodeType *functionDefinitionNodeType = functionGraph->getNodeType("function");
    ConnectionData functionDefinitionData;
    functionDefinitionData.setValue("graph", functionDefinition->name);
    Node & functionDefinitionNode = currentGraph->newNode(*functionDefinitionNodeType, "function", functionDefinitionData);
    functionDefinitionNode.setPosition(std::make_pair(-depth * 400, 0));
    functionDefinitionNode.setOverrideInputs(true);

    currentGraph->newEdge(
	functionDefinitionNode.getId(), functionDefinitionNodeType->getOutputs().getName(0),
	parentNode, parentFunctionDefinitions
	);

    NamedPorts & functionDefinitionDefaults = functionDefinitionNode.getOverrideInputs();
    
    for (const auto & assignment : functionDefinition->parameters) {
	std::unique_ptr<NodePort> port = std::make_unique<NodePort>("variable", assignment->getName(), NodePort::ConnectionPolicy::One);
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
