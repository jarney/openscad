#include <stdio.h>
#include <fstream>
#include <iostream>

#include <QtCore/QFileInfo>

#include "openscad.h"
//extern bool parse(class SourceFile *& file, const std::string& text, const std::string& filename,
//                  const std::string& mainFile, int debug);

#if 0
#include "nodes/NodeFactoryRegistry.hpp"
#include "nodes/NodeProgram.hpp"
#include "nodes/NodeType.hpp"
#include "nodes/NodeProgramSerializer.hpp"
#include "nodes/openscad/Builtins.hpp"
#endif

#include "node--js/xml/Serializer.hpp"
#include "nodes/openscad/Parser.h"

#include "core/Builtins.h"

using namespace NodeJS::core;

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
    NodeModule node_module;
    
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
	
	processSourceFile(node_module, sourceFile, *builtin_context);
    }

    SerializerErrorReporterStream err(std::cerr);
    
    const auto & xmlSerializer = NodeJS::xml::Serializer::instance();
    xmlSerializer.write(node_module, std::cout, err);
    
#if 0
    const NodeProgramSerializer & fromSCAD = JNodes::openscad::NodeProgramSerializerOpenSCAD::instance();
    if (fromSCAD.read(program, input_stream)) {
	fprintf(stderr, "Could not read file %s\n", argv[1]);
	return 4;
    }

    const NodeProgramSerializer & toJson = NodeProgramSerializerJSON::instance();
    toJson.write(program, std::cout);
#endif

    return 0;
}

