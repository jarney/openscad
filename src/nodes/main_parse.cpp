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
    
    const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
    serializer.write(program, std::cout);

    return 0;
}
