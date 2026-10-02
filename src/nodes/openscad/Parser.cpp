#include "Parser.h"
#include <variant>
#include <fstream>
#include <iostream>
#include "node--js/SerializerError.hpp"
#include "node--js/xml/Serializer.hpp"

using namespace NodeJS::core;

/*************************************************************/
class ModuleInstantiationASTHandler {
public:
    ModuleInstantiationASTHandler() = default;
    ~ModuleInstantiationASTHandler() = default;

    virtual void handle(
	NodeModule & program,
	NodeGraph *currentGraph,
	NodeId parentNode,
	PortId parentPort,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const = 0;
    
};

class ModuleNodeFactorySphere : public ModuleInstantiationASTHandler {
public:
    virtual void handle(
	NodeModule & program,
	NodeGraph *currentGraph,
	NodeId parentNode,
	PortId parentPort,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const;
};

void
ModuleNodeFactorySphere::handle(
	NodeModule & program,
	NodeGraph *currentGraph,
	NodeId parentNode,
	PortId parentPort,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	const std::shared_ptr<const Context>& context,
	int depth,
	int i
	) const
{
    // Create a new node and connect it to
    // our parent node with the output of the module's node
    // connected to the input of our parent's node.
//    NodeId childNode = currentGraph->addNode(moduleInstantiation->name());

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
    
//    QPointF pos(-depth * 400, -i * 400);
//    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);

//    QtNodes::ConnectionId connection;
//    connection.inNodeId = parentNode;
//    connection.inPortIndex = parentPort;
//    connection.outNodeId = childNode.getId();
//    connection.outPortIndex = 0;
//    currentGraph->addConnection(connection);
    currentGraph->newEdge(parentNode, parentPort,
			  childNode.getId(), "output");

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
    processLocalScope(program, currentGraph, childNode.getId(), childNode.getType().getInputPortName(0), moduleInstantiation->scope, context, depth);
	
}

static std::map<std::string, std::shared_ptr<ModuleInstantiationASTHandler>> moduleFactory;

void
processSourceFile(
    NodeModule & program,
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
    std::unique_ptr<NodeModule> openscad_module = std::make_unique<NodeModule>();
    std::ifstream in("../submodules/node--js/doc/openscad.xml");
    SerializerErrorReporterStream err(std::cerr);

    const auto & ser = NodeJS::xml::Serializer::instance();
    
    bool rc = ser.read(
	*openscad_module,
	in,
	err);
    
    ////////////
    NodeGraph *main = program.addGraph("main");
    main->addScope(std::move(openscad_module));
    
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
    processLocalScope(program, main, outputNode.getId(), outputNode.getType().getInputPortName(0), sourceFile->scope, *file_context, 0);
}

void
processAssignment(
    NodeModule & program,
    NodeGraph *currentGraph,
    const std::shared_ptr<Assignment> & assignment,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
//    NodeId assignmentNode = currentGraph->addNode("assign");

    const NodeType *nodeType = currentGraph->getNodeType("assign");
    ConnectionData defaultData;
    Node & assignmentNode = currentGraph->newNode(
	*nodeType,
	"assign",
	defaultData
	);
    assignmentNode.setPosition(std::make_pair(-depth * 400, 0));
    
//    currentGraph->setNodeData(assignmentNode, QtNodes::NodeRole::Position, pos);
//    Node *node = currentGraph->getNode(assignmentNode);
    assignmentNode.getData().setValue("variable_name", assignment->getName().c_str());

    processExpression(program, currentGraph, assignmentNode.getId(), assignmentNode.getType().getInputPortName(0), assignment->getExpr(), context, depth+1);
}

void
processExpressionUnaryOp(
    NodeModule & program,
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

//    NodeId childNode = currentGraph->addNode(op_it->second);
//
//    QPointF pos(-depth * 400, 0);
//    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);
    
//    QtNodes::ConnectionId connection;
//    connection.inNodeId = parentNode;
//    connection.inPortIndex = parentPort;
//    connection.outNodeId = childNode;
//    connection.outPortIndex = 0;
//    currentGraph->addConnection(connection);
    currentGraph->newEdge(parentNode, parentPort,
			  childNode.getId(), childNode.getType().getOutputPortName(0));

    processExpression(program, currentGraph, childNode.getId(), childNode.getType().getInputPortName(0), operation->expr, context, depth+1);
    
}

void
processExpressionBinaryOp(
    NodeModule & program,
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
	{BinaryOp::Op::LessEqual, "le"},
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
	parentNode, parentPort,
	childNode.getId(), childNode.getType().getOutputPortName(0)
	);
    
//    NodeId childNode = currentGraph->addNode(op_it->second);
//
//    QPointF pos(-depth * 400, 0);
//    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);
//    
//    QtNodes::ConnectionId connection;
//    connection.inNodeId = parentNode;
//    connection.inPortIndex = parentPort;
//    connection.outNodeId = childNode;
//    connection.outPortIndex = 0;
//    currentGraph->addConnection(connection);

    processExpression(program, currentGraph, childNode.getId(), childNode.getType().getInputPortName(0), operation->left, context, depth+1);
    processExpression(program, currentGraph, childNode.getId(), childNode.getType().getInputPortName(1), operation->right, context, depth+1);
}

void
processExpressionLiteral(
    NodeModule & program,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const Literal *literal,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
//    NodeId childNode;
    Node *childNode;
    if (literal->isBool()) {
	//childNode = currentGraph->addNode(nodeType
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
	//childNode = currentGraph->addNode("const_string");
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
	    //childNode = currentGraph->addNode("const_int");
	    //Node *node = currentGraph->getNode(childNode);
	    //node->setValue("value", std::to_string(lit_long));
	    std::string nodeTypeName = "const_int";
	    const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
	    if (nodeType == nullptr) {
		fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
		return;
	    }
	    ConnectionData defaultData;
	    defaultData.setValue("value", std::to_string(lit_long));
	    childNode = &currentGraph->newNode(
		*nodeType,
		nodeTypeName,
		defaultData);
	    
	}
	else {
	    //childNode = currentGraph->addNode("const_float");
	    //Node *node = currentGraph->getNode(childNode);
	    //node->setValue("value", std::to_string(lit_double));
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
	//childNode = currentGraph->addNode("undef");
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
    //QPointF pos(-depth * 400, 0);
    //currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);
    
    //QtNodes::ConnectionId connection;
    //connection.inNodeId = parentNode;
    //connection.inPortIndex = parentPort;
    //connection.outNodeId = childNode;
    //connection.outPortIndex = 0;
    //currentGraph->addConnection(connection);
    currentGraph->newEdge(
	parentNode, parentPort,
	childNode->getId(), childNode->getType().getInputPortName(0)
	);
    
}

void
processExpressionLookup(
    NodeModule & program,
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
	parentNode, parentPort,
	childNode->getId(), childNode->getType().getInputPortName(0)
	);
#if 0
    NodeId childNode;

    childNode = currentGraph->addNode("variable");
    Node *node = currentGraph->getNode(childNode);
    node->setValue("variable_name", lookup->get_name());
    
    QPointF pos(-depth * 400, 0);
    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);
    
    QtNodes::ConnectionId connection;
    connection.inNodeId = parentNode;
    connection.inPortIndex = parentPort;
    connection.outNodeId = childNode;
    connection.outPortIndex = 0;
    currentGraph->addConnection(connection);
#endif
}

void
processExpressionBuiltinFunctionCall(
    NodeModule & program,
    NodeGraph *currentGraph,
    NodeId parentNode,
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
#if 0
    Parameters parameters = Parameters::parse(
	Arguments(moduleInstantiation->arguments, context),
	moduleInstantiation->location(),
	sphere_required,
	sphere_optional);
						      
    processExpression(program, currentGraph, childNode, 0, operation->left, context, depth+1);
    processExpression(program, currentGraph, childNode, 1, operation->right, context, depth+1);
#endif
}

void
processExpressionFunctionCall(
    NodeModule & program,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    const FunctionCall *functionCall,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    std::string nodeTypeName  = functionCall->name;
    const NodeType *nodeType = currentGraph->getNodeType(nodeTypeName);
    if (nodeType == nullptr) {
	fprintf(stderr, "Error loading node of type %s\n", nodeTypeName.c_str());
	return;
    }
    ConnectionData defaultData;
    Node *childNode = &currentGraph->newNode(
	*nodeType,
	nodeTypeName,
	defaultData);
    
    childNode->setPosition(std::make_pair(-depth * 400, 0));
    
    currentGraph->newEdge(
	parentNode, parentPort,
	childNode->getId(), childNode->getType().getInputPortName(0)
	);
#if 0
    NodeId childNode = currentGraph->addNode(functionCall->name);
						      
    QPointF pos(-depth * 400, 0);
    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);
    
    QtNodes::ConnectionId connection;
    connection.inNodeId = parentNode;
    connection.inPortIndex = parentPort;
    connection.outNodeId = childNode;
    connection.outPortIndex = 0;
    currentGraph->addConnection(connection);
#endif
    
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
	    program,
	    currentGraph,
	    childNode->getId(),
	    builtinFunction,
	    context,
	    depth+1
	    );
    }
    else if (std::holds_alternative<CallableUserFunction>(*scad_function)) {
	fprintf(stderr, "Callable user function %s\n", functionCall->name.c_str());
	throw std::string("Callable user functions not yet supported\n");
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
    NodeModule & program,
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
	processExpressionUnaryOp(program, currentGraph, parentNode, parentPort, dynamic_cast<UnaryOp*>(e), context, depth);
    }
    else if (dynamic_cast<BinaryOp*>(e)) {
	processExpressionBinaryOp(program, currentGraph, parentNode, parentPort, dynamic_cast<BinaryOp*>(e), context, depth);
    }
    else if (dynamic_cast<TernaryOp*>(e)) {
    }
    else if (dynamic_cast<ArrayLookup*>(e)) {
    }
    else if (dynamic_cast<Literal*>(e)) {
	processExpressionLiteral(program, currentGraph, parentNode, parentPort, dynamic_cast<Literal*>(e), context, depth);
    }
    else if (dynamic_cast<Vector*>(e)) {
    }
    else if (dynamic_cast<Lookup*>(e)) {
	processExpressionLookup(program, currentGraph, parentNode, parentPort, dynamic_cast<Lookup*>(e), context, depth);
    }
    else if (dynamic_cast<MemberLookup*>(e)) {
    }
    else if (dynamic_cast<FunctionCall*>(e)) {
	processExpressionFunctionCall(program, currentGraph, parentNode, parentPort, dynamic_cast<FunctionCall*>(e), context, depth);
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
processLocalScope(
    NodeModule & program,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    std::shared_ptr<LocalScope> localScope,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{

    // First process any variable assignments in this scope.
    for (const auto assignment : localScope->assignments) {
	processAssignment(program, currentGraph, assignment, context, depth+1);
    }

    // Next, process any function definitions

    // Next, process any module definitions

    // We probably want to push the context after we do this so we can perform
    // lookups in terms of the new context.

    
    // Finally instantiate any modules
    int i = 0;
    for (const auto moduleInstantiation : localScope->moduleInstantiations) {
	processModuleInstantiation(program, currentGraph, parentNode, parentPort, moduleInstantiation, context, depth+1, i);
	i++;
    }
}

void
processModuleInstantiation(
    NodeModule & program,
    NodeGraph *currentGraph,
    NodeId parentNode,
    PortId parentPort,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    const std::shared_ptr<const Context>& context,
    int depth,
    int i
    )
{
    const auto it = moduleFactory.find(moduleInstantiation->name());
    if (it != moduleFactory.end()) {
	it->second->handle(program, currentGraph, parentNode, parentPort, moduleInstantiation, context, depth, i);
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
