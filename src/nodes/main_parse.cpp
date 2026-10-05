#include <stdio.h>
#include <fstream>
#include <iostream>

#include <QtCore/QFileInfo>

#include "openscad.h"

#include "node--js/xml/SerializerXML.hpp"
#include "nodes/openscad/Parser.h"

#include "core/Builtins.h"

using namespace NodeJS::core;
using namespace NodeJS::xml;

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

    //std::shared_ptr<NodeFactoryRegistry> registry = JNodes::openscad::Builtins::registerDataModels();
    ModuleLoaderNodeJSPath loader;
    loader.setNODEJS_PATH("../submodules/node--js/test-data");
    NodeModule & node_module = *loader.newModule("anonymous");
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "File %s does not exist\n", argv[1]);
	fprintf(stderr, "Usage: process filename\n");
	return 3;
    }

    // Register the OpenSCAD builtins
    Builtins::initialize();

    std::string fname(argv[1]);
    std::ifstream input_stream(fname);
    {
	std::string fulltext(std::istreambuf_iterator<char>(input_stream), {});

	SourceFile *sourceFile;
	std::string fname("none");
	
	sourceFile = parse(
	    sourceFile,
	    fulltext,
	    fname,
	    fname,
	    false) ? sourceFile : nullptr;
	if (!sourceFile) {
	    fprintf(stderr, "Unsuccessful parse\n");
	    return -1;
	}
	EvaluationSession session{sourceFile->getFullpath()};
	ContextHandle<BuiltinContext> builtin_context{Context::create<BuiltinContext>(&session)};
	
	SerializerErrorReporterStream err(std::cerr);
	processSourceFile(err, node_module, sourceFile, *builtin_context);
    }

    SerializerErrorReporterStream err(std::cerr);
    
    const auto & xmlSerializer = SerializerXML::instance();
    xmlSerializer.write(node_module, std::cout, err);
    
    return 0;
}

