#include <stdio.h>
#include <fstream>

#include <QtCore/QFileInfo>
#include <QApplication>

#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADEvaluator.hpp"
#include "nodes/NodeProgram.hpp"
#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgramSerializer.hpp"

#include "core/SourceFile.h"
#include "core/LocalScope.h"
#include "core/ModuleInstantiation.h"
#include "core/Assignment.h"
#include "core/Expression.h"

#include "openscad.h"

//extern bool parse(class SourceFile *& file, const std::string& text, const std::string& filename,
//                  const std::string& mainFile, int debug);

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

    auto localScope = sourceFile->scope;
    auto mi = localScope->moduleInstantiations.at(0);
    fprintf(stderr, "Module instantiation name %s\n", mi->name().c_str());
    auto as = mi->arguments.at(0);
    Expression *expr = as->getExpr().get();
    
    Lookup *lit = dynamic_cast<Lookup*>(expr);
    if (lit) {
	fprintf(stderr, "It is a literal %s\n", lit->get_name().c_str());
    }
    else {
	fprintf(stderr, "It is not a literal\n");
    }
    
    const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
    serializer.write(program, std::cout);

    return 0;
}
