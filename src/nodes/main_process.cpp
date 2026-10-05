#include <stdio.h>
#include <fstream>
#include <iostream>

#include <QtCore/QFileInfo>
#include <QApplication>

#include "node--js/NodeModule.hpp"
#include "node--js/Processor.hpp"
#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "node--js/engines/openscad/Builtins.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

class NodeProcessorOutput : public NodeProcessor {
public:
    virtual void process(
	const Node & node,
	const ConnectionData & fromData,
	ConnectionData & toData
	);
};

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

    ModuleLoaderNodeJSPath loader;
    loader.setNODEJS_PATH("../submodules/node--js/test-data");
    NodeModule & nodeModule = *loader.newModule("anonymous");
    
    auto & serializer = SerializerXML::instance();
    std::string filename(argv[1]);
    std::ifstream exampleInputFile(filename);

    // The linker/class loader needs to be invoked here so we can actually include
    // the correct paths for dependent graphs/node types.
    
    SerializerErrorReporterStream err(std::cerr);
    if (!serializer.read(nodeModule, exampleInputFile, err)) {
	fprintf(stderr, "Could not read file %s\n", argv[1]);
	return 4;
    }

    Processor processor;

    // We need to register the 'native' node types here.
    NodeJS::openscad::Builtins::registerProcessors(processor);
    processor.setNativeImpl("output", std::make_unique<NodeProcessorOutput>());
    
    ConnectionData input;
    ConnectionData output;

    NodeGraph *graph = nodeModule.getGraph("main");
    if (!graph) {
	fprintf(stderr, "Invalid graph\n");
	return 3;
    }
    processor.processGraph(*graph, input, output);
    
    return 0;
}

void
NodeProcessorOutput::process(
    const Node & node,
    const ConnectionData & input,
    ConnectionData & output
    )    
{
    fprintf(stderr, "Assignments: %s\n", input.getValue("assignments").c_str());
    fprintf(stderr, "Module Instantiations %s\n", input.getValue("geometry").c_str());
}
