#include <stdio.h>
#include <unistd.h>
#include <iostream>

#include "core/Builtins.h"
#include <QtCore/QFileInfo>

#include "node--js/NodeModule.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "nodes/nodes.hpp"

using namespace NodeJS::core;
using namespace NodeJS::xml;

void dumpRegistry(const NodeModule *openscad_module);
void dumpBuiltins(void);
void dumpNotImplemented(const NodeModule *openscad_module);

int main_audit_namespace(int argc, char *argv[])
{
    if (argc != 2) {
	fprintf(stderr, "Usage: audit-namespace filename\n");
	return 1;
    }
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "Cannot process file, it does not exist\n");
	return 2;
    }

    SerializerErrorReporterStream err(std::cerr);
    
    ModuleLoaderNodeJSPath loader;
    loader.setNODEJS_PATH("../submodules/node--js/test-data");
    const NodeModule *openscad_module = loader.loadModule("org.ensor.nodejs.openscad", err);

    // Register builtins...
    Builtins::initialize();
//    dumpRegistry(registry);
//    dumpBuiltins();

    dumpNotImplemented(openscad_module);

    return 0;
}

void dumpBuiltins(void)
{
    for (const auto & it : Builtins::instance().getModules()) {
	fprintf(stderr, "Builtin module %s\n", it.first.c_str());
    }
}

void dumpNotImplemented(const NodeModule *openscad_module)
{
    for (const auto & builtin_it : Builtins::instance().getModules()) {
	if (!openscad_module->hasNodeType(builtin_it.first)) {
	    fprintf(stderr, "NO %s (module)\n", builtin_it.first.c_str());
	}
	else {
//	    fprintf(stderr, "YES %s\n", builtin_it.first.c_str());
	}

    }
    for (const auto & builtin_it : Builtins::instance().getFunctions()) {
	if (!openscad_module->hasNodeType(builtin_it.first)) {
	    fprintf(stderr, "NO %s (function)\n", builtin_it.first.c_str());
	}
	else {
//	    fprintf(stderr, "YES %s\n", builtin_it.first.c_str());
	}

    }
}

void dumpRegistry(const NodeModule *openscad_module)
{
    for (const auto & it : openscad_module->getNodeTypes()) {
	fprintf(stderr, "%s\n",
		it.first.c_str()
	    );
    }
}
