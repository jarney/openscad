#include <stdio.h>
#include <fstream>

#include <QtCore/QFileInfo>
#include <QApplication>

#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/NodeProgram.hpp"
#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgramSerializer.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/OpenSCADEvaluator.hpp"

#include "core/SourceFile.h"
#include "core/LocalScope.h"
#include "core/ModuleInstantiation.h"
#include "core/Assignment.h"
#include "core/Expression.h"

#include "openscad.h"

void processSourceFile(NodeProgram & program, SourceFile *sourceFile);

void processLocalScope(
    NodeProgram & program,
    NodeProgramGraphModel *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<LocalScope> localScope,
    int depth
    );

void processModuleInstantiation(
    NodeProgram & program,
    NodeProgramGraphModel *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    int depth,
    int i
    );

int main_parse(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    if (argc != 2) {
	fprintf(stderr, "Usage: process filename\n");
	return 1;
    }
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "Cannot process file, it does not exist\n");
	return 2;
    }

    std::shared_ptr<NodeProgramModelRegistry> registry = OpenSCADBuiltins::registerDataModels();
    NodeProgram program(registry);
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "File %s does not exist\n", argv[1]);
	fprintf(stderr, "Usage: process filename\n");
	return 3;
    }

    std::string fname(argv[1]);
    std::ifstream input_stream(fname);
    std::string fulltext(std::istreambuf_iterator<char>(input_stream), {});

    SourceFile *sourceFile;
    sourceFile = parse(sourceFile, fulltext, fname, fname, false) ? sourceFile : nullptr;
    if (!sourceFile) {
	fprintf(stderr, "Unsuccessful parse\n");
	return -1;
    }
    fprintf(stderr, "Got a valid parse tree\n");

    processSourceFile(program, sourceFile);
    
    const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
    serializer.write(program, std::cout);

    return 0;
}

class ModuleInstantiationASTHandler {
public:
    ModuleInstantiationASTHandler() = default;
    ~ModuleInstantiationASTHandler() = default;

    virtual void handle(
	NodeProgram & program,
	NodeProgramGraphModel *currentGraph,
	QtNodes::NodeId parentNode,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	int depth,
	int i
	) const = 0;
    
};

class ModuleNodeFactorySphere : public ModuleInstantiationASTHandler {
public:
    virtual void handle(
	NodeProgram & program,
	NodeProgramGraphModel *currentGraph,
	QtNodes::NodeId parentNode,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	int depth,
	int i
	) const;
};

void
ModuleNodeFactorySphere::handle(
	NodeProgram & program,
	NodeProgramGraphModel *currentGraph,
	QtNodes::NodeId parentNode,
	std::shared_ptr<ModuleInstantiation> moduleInstantiation,
	int depth,
	int i
	) const
{
    fprintf(stderr, "Handling module instantiation\n");
}

static std::map<std::string, ModuleInstantiationASTHandler *> moduleFactory;


void processSourceFile(NodeProgram & program, SourceFile *sourceFile)
{

    //////////// Initialization

    moduleFactory["sphere"] = new ModuleNodeFactorySphere();
    moduleFactory["cube"] = new ModuleNodeFactorySphere();
    
    ////////////
    NodeProgramGraphModel *main = program.newGraph("main");
    QtNodes::NodeId outputNode = main->addNode("output");

    processLocalScope(program, main, outputNode, sourceFile->scope, 0);
}

void
processLocalScope(
    NodeProgram & program,
    NodeProgramGraphModel *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<LocalScope> localScope,
    int depth
    )
{
    int i = 0;
    for (const auto moduleInstantiation : localScope->moduleInstantiations) {
	processModuleInstantiation(program, currentGraph, parentNode, moduleInstantiation, depth+1, i);
	i++;
    }
}

void
processModuleInstantiation(
    NodeProgram & program,
    NodeProgramGraphModel *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    int depth,
    int i
    )
{
    fprintf(stderr, "Module instantiation name %s\n", moduleInstantiation->name().c_str());

    const auto it = moduleFactory.find(moduleInstantiation->name());
    if (it != moduleFactory.end()) {
	it->second->handle(program, currentGraph, parentNode, moduleInstantiation, depth, i);
    }
    
    QtNodes::NodeId childNode = currentGraph->addNode(QString::fromStdString(moduleInstantiation->name()));
    QPointF pos(-depth * 400, -i * 400);
    currentGraph->setNodeData(childNode, QtNodes::NodeRole::Position, pos);

    QtNodes::ConnectionId connection;
    connection.inNodeId = parentNode;
    connection.inPortIndex = 0;
    connection.outNodeId = childNode;
    connection.outPortIndex = 0;
    currentGraph->addConnection(connection);
    
    processLocalScope(program, currentGraph, childNode, moduleInstantiation->scope, depth);
	
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
