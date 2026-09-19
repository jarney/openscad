
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

    ContextHandle<FileContext> file_context{Context::create<FileContext>(context, sourceFile)};
    //////////// Initialization

    moduleFactory["sphere"] = std::make_shared<ModuleNodeFactorySphere>();
    moduleFactory["cube"] = moduleFactory["sphere"];
    moduleFactory["for"] = moduleFactory["sphere"];
    
    ////////////
    NodeGraph *main = program.newGraph("main");
    QtNodes::NodeId outputNode = main->addNode("output");

    // A source file is just a single large scope.
    processLocalScope(program, main, outputNode, sourceFile->scope, *file_context, 0);
}

void
processAssignment(
    NodeProgram & program,
    NodeGraph *currentGraph,
    const std::shared_ptr<Assignment> & assignment,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    QtNodes::NodeId assignmentNode = currentGraph->addNode("assign");
    QPointF pos(-depth * 400, 0);
    currentGraph->setNodeData(assignmentNode, QtNodes::NodeRole::Position, pos);
    Node *node = currentGraph->getNode(assignmentNode);
    node->setValue("variable", assignment->getName().c_str());

    processExpression(program, currentGraph, assignmentNode, assignment->getExpr(), context, depth+1);
}

void
processExpressionLiteral(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    const Literal *literal,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    QtNodes::NodeId childNode;
    if (literal->isBool()) {
	childNode = currentGraph->addNode(literal->toBool() ? "true" : "false");
    }
    else if (literal->isString()) {
	childNode = currentGraph->addNode("const_string");
	Node *node = currentGraph->getNode(childNode);
	node->setValue("value", literal->toString());
    }
    else if (literal->isDouble()) {
	double lit_double = literal->toDouble();
	long lit_long = (long)lit_double;
	double lit_recast = (double)lit_long;
	if (abs(lit_double - lit_recast) < 1e-9) {
	    childNode = currentGraph->addNode("const_int");
	    Node *node = currentGraph->getNode(childNode);
	    node->setValue("value", std::to_string(lit_long));
	}
	else {
	    childNode = currentGraph->addNode("const_float");
	    Node *node = currentGraph->getNode(childNode);
	    node->setValue("value", std::to_string(lit_double));
	}
    }
    else if (literal->isUndefined()) {
	childNode = currentGraph->addNode("undef");
    }
    else {
	throw std::string("Invalid literal type found parsing openscad file\n");
    }
    
    QPointF pos(-depth * 400, 0);
    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);
    
    QtNodes::ConnectionId connection;
    connection.inNodeId = parentNode;
    connection.inPortIndex = 0;
    connection.outNodeId = childNode;
    connection.outPortIndex = 0;
    currentGraph->addConnection(connection);    
}

void
processExpression(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    const std::shared_ptr<Expression> & expression,
    const std::shared_ptr<const Context>& context,
    int depth
    )
{
    // TODO: Switch based on expression type and recursively handle expressions by their type...
    Expression *e = expression.get();
    if (dynamic_cast<UnaryOp*>(e)) {
    }
    else if (dynamic_cast<BinaryOp*>(e)) {
    }
    else if (dynamic_cast<TernaryOp*>(e)) {
    }
    else if (dynamic_cast<ArrayLookup*>(e)) {
    }
    else if (dynamic_cast<ArrayLookup*>(e)) {
    }
    else if (dynamic_cast<Literal*>(e)) {
	processExpressionLiteral(program, currentGraph, parentNode, dynamic_cast<Literal*>(e), context, depth);
    }
    else if (dynamic_cast<Vector*>(e)) {
    }
    else if (dynamic_cast<Lookup*>(e)) {
    }
    else if (dynamic_cast<MemberLookup*>(e)) {
    }
    else if (dynamic_cast<FunctionCall*>(e)) {
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
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
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
