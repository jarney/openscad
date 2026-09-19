#include <stdio.h>
#include <fstream>

#include <QtCore/QFileInfo>
#include <QApplication>

#include "nodes/NodeFactoryRegistry.hpp"
#include "nodes/NodeProgram.hpp"
#include "nodes/NodeType.hpp"
#include "nodes/NodeProgramSerializer.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/NodeProgramSerializerOpenSCAD.hpp"

using namespace JNodes::core;
using namespace JNodes::openscad;

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
    
    std::shared_ptr<NodeFactoryRegistry> registry = JNodes::openscad::Builtins::registerDataModels();
    NodeProgram program(registry);
    
    const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
    std::string filename(argv[1]);
    std::ifstream exampleInputFile(filename);
    if (serializer.read(program, exampleInputFile)) {
	fprintf(stderr, "Could not read file %s\n", argv[1]);
	return 4;
    }
    
    NodeGraph *graph = program.getGraph("main");
    if (!graph) {
	fprintf(stderr, "File %s does not contain a 'main' graph\n", argv[1]);
	return 3;
    }

    NodeProgramSerializerOpenSCAD::instance().write(program, std::cout);
    std::cout << std::endl;

    return 0;
}
