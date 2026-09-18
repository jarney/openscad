#include <stdio.h>
#include <unistd.h>

#include "core/Builtins.h"

#include <QtCore/QFileInfo>
#include "nodes/NodeFactoryRegistry.hpp"
#include "nodes/NodeGraph.hpp"
#include "nodes/NodeType.hpp"

#include "nodes/openscad/Builtins.hpp"

#include "nodes/nodes.hpp"

using namespace JNodes::core;

void dumpRegistry(std::shared_ptr<NodeFactoryRegistry> registry);
void dumpBuiltins(void);
void dumpNotImplemented(std::shared_ptr<NodeFactoryRegistry> registry);

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

    std::shared_ptr<NodeFactoryRegistry> registry = JNodes::openscad::Builtins::registerDataModels();

    // Register builtins...
    Builtins::initialize();
//    dumpRegistry(registry);
//    dumpBuiltins();

    dumpNotImplemented(registry);

    return 0;
}

void dumpBuiltins(void)
{
    for (const auto & it : Builtins::instance().getModules()) {
	fprintf(stderr, "Builtin module %s\n", it.first.c_str());
    }
}

void dumpNotImplemented(std::shared_ptr<NodeFactoryRegistry> registry)
{
    const NodeFactoryRegistry::RegisteredModelCreatorsMap & registered
	= registry->getModels();

    for (const auto & builtin_it : Builtins::instance().getModules()) {
	QString registeredName = QString::fromStdString(builtin_it.first);
	const auto & it = registered.find(registeredName);
	if (it == registered.end()) {
	    fprintf(stderr, "NO %s (module)\n", builtin_it.first.c_str());
	}
	else {
//	    fprintf(stderr, "YES %s\n", builtin_it.first.c_str());
	}

    }
    for (const auto & builtin_it : Builtins::instance().getFunctions()) {
	QString registeredName = QString::fromStdString(builtin_it.first);
	const auto & it = registered.find(registeredName);
	if (it == registered.end()) {
	    fprintf(stderr, "NO %s (function)\n", builtin_it.first.c_str());
	}
	else {
//	    fprintf(stderr, "YES %s\n", builtin_it.first.c_str());
	}

    }
}

void dumpRegistry(std::shared_ptr<NodeFactoryRegistry> registry)
{
    for (const auto & it : registry->getModels()) {
	fprintf(stderr, "%s : %s\n",
		it.first.toStdString().c_str(),
		it.second->getCategory().c_str());
    }
    
}
