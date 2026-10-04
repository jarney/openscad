#include <stdio.h>
#include <fstream>
#include <iostream>

#include <QtCore/QFileInfo>
#include <QApplication>

#include "node--js/NodeModule.hpp"
#include "node--js/Processor.hpp"
#include "node--js/xml/Serializer.hpp"
#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::core;

int main_process(int argc, char *argv[])
{
    if (argc != 2) {
	fprintf(stderr, "Usage: process filename\n");
	return 1;
    }
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "File %s does not exist\n", argv[1]);
	fprintf(stderr, "Usage: process filename\n");
	return 2;
    }

    NodeModule program;
    auto & serializer = NodeJS::xml::Serializer::instance();
    std::string filename(argv[1]);
    std::ifstream exampleInputFile(filename);

    // The linker/class loader needs to be invoked here so we can actually include
    // the correct paths for dependent graphs/node types.
    
    SerializerErrorReporterStream err(std::cerr);
    if (!serializer.read(program, exampleInputFile, err)) {
	fprintf(stderr, "Could not read file %s\n", argv[1]);
	return 4;
    }

    Processor processor;

    // We need to register the 'native' node types here.
    NodeJS::openscad::Builtins::registerProcessors(processor);
    
    ConnectionData input;
    ConnectionData output;

    NodeGraph *graph = program.getGraph("main");
    if (!graph) {
	fprintf(stderr, "Invalid graph\n");
	return 3;
    }
    processor.processGraph(*graph, input, output);
    
    fprintf(stderr, "We really processed a graph: %s\n", output.getValue("Geometry").c_str());
    
    return 0;
}
