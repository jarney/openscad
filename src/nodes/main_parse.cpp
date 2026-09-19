#include <stdio.h>
#include <fstream>

#include <QtCore/QFileInfo>

#include "nodes/NodeFactoryRegistry.hpp"
#include "nodes/NodeProgram.hpp"
#include "nodes/NodeType.hpp"
#include "nodes/NodeProgramSerializer.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/NodeProgramSerializerOpenSCAD.hpp"

using namespace JNodes::core;
using namespace JNodes::openscad;

int main_parse(int argc, char *argv[])
{
    if (argc != 2) {
	fprintf(stderr, "Usage: process filename\n");
	return 1;
    }
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "Cannot process file, it does not exist\n");
	return 2;
    }

    std::shared_ptr<NodeFactoryRegistry> registry = JNodes::openscad::Builtins::registerDataModels();
    NodeProgram program(registry);
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "File %s does not exist\n", argv[1]);
	fprintf(stderr, "Usage: process filename\n");
	return 3;
    }

    std::string fname(argv[1]);
    std::ifstream input_stream(fname);
    
    const NodeProgramSerializer & fromSCAD = NodeProgramSerializerOpenSCAD::instance();
    if (fromSCAD.read(program, input_stream)) {
	fprintf(stderr, "Could not read file %s\n", argv[1]);
	return 4;
    }

    const NodeProgramSerializer & toJson = NodeProgramSerializerJSON::instance();
    toJson.write(program, std::cout);

    return 0;
}

