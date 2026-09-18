#include <stdio.h>
#include <unistd.h>

#include "core/Builtins.h"

#include <QtCore/QFileInfo>
#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/NodeProgramGraphModel.hpp"
#include "nodes/NodeModelType.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "OpenSCADEvaluator.hpp"

#include "nodes/nodes.hpp"

void dumpRegistry(std::shared_ptr<NodeProgramModelRegistry> registry);
void dumpBuiltins(void);
void dumpNotImplemented(std::shared_ptr<NodeProgramModelRegistry> registry);

int main_audit_namespace(int argc, char *argv[])
{
//    QApplication app(argc, argv);
    
    if (argc != 2) {
	fprintf(stderr, "Usage: audit-namespace filename\n");
	return 1;
    }
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "Cannot process file, it does not exist\n");
	return 2;
    }

    std::shared_ptr<NodeProgramModelRegistry> registry = JNodes::openscad::Builtins::registerDataModels();

    // Register builtins...
    Builtins::initialize();
//    dumpRegistry(registry);
//    dumpBuiltins();

    dumpNotImplemented(registry);

    return 0;
#if 0
    NodeProgramGraphModel dataFlowGraphModel(registry);

    QJsonObject object = oscd_loadJson(argv[1]);
    dataFlowGraphModel.load(object);
    
    evaluateToSCAD(dataFlowGraphModel);
#endif
    return 0;
}

void dumpBuiltins(void)
{
    for (const auto & it : Builtins::instance().getModules()) {
	fprintf(stderr, "Builtin module %s\n", it.first.c_str());
    }
}

void dumpNotImplemented(std::shared_ptr<NodeProgramModelRegistry> registry)
{
    const NodeProgramModelRegistry::RegisteredModelCreatorsMap & registered
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

void dumpRegistry(std::shared_ptr<NodeProgramModelRegistry> registry)
{
    for (const auto & it : registry->getModels()) {
	fprintf(stderr, "%s : %s\n",
		it.first.toStdString().c_str(),
		it.second->getCategory().c_str());
    }
    
}
