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

int main_process(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    if (argc != 2) {
	fprintf(stderr, "Usage: process filename\n");
	return 1;
    }
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "File %s does not exist\n", argv[1]);
	fprintf(stderr, "Usage: process filename\n");
	return 2;
    }
    
    std::shared_ptr<NodeProgramModelRegistry> registry = JNodes::openscad::Builtins::registerDataModels();
    NodeProgram program(registry);
    
    const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
    std::string filename(argv[1]);
    std::ifstream exampleInputFile(filename);
    serializer.read(program, exampleInputFile);
    
    NodeProgramGraphModel *graph = program.getGraph("main");
    if (!graph) {
	fprintf(stderr, "File %s does not contain a 'main' graph\n", argv[1]);
	return 3;
    }
    
    std::cout << evaluateToSCAD(*graph);

    return 0;
}
