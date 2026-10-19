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
    ModuleLoader loader;
    std::unique_ptr<ModuleLoaderNodeJSPath> plugin = std::make_unique<ModuleLoaderNodeJSPath>();
    plugin->setNODEJS_PATH("../submodules/node--js/test-data/xml/Serializer:.");
    loader.setPlugin(std::move(plugin));
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

    // Derive the package name from the input filename.
    std::string package;
    size_t ext_offset = fname.find_last_of('.');
    if (ext_offset != std::string::npos) {
	package = fname.substr(0, ext_offset);
    }
    else {
	package = fname;
    }
    node_module.setPackage(package);

    std::ofstream out(node_module.getPackage() + ".xml", std::ios::binary | std::ios::trunc);
    const auto & xmlSerializer = SerializerXML::instance();
    xmlSerializer.write(node_module.getPackage(), node_module, out, err);
    out.close();
    
    return 0;
}

