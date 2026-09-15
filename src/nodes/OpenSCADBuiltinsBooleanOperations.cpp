#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#define _OPENSCAD_NODE_CATEGORY "Boolean Operations"

////////////////////////////////////////
// Union
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_op_union()
{
    auto model = std::make_unique<NodeModelType>("union", "Union", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_union_process);
    return model;
}
void
OpenSCADBuiltins::f_op_union_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("union() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        input.getValue("b", "{}") +
	std::string("    }") + 
        std::string("});"));
}

////////////////////////////////////////
// Difference
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_op_difference()
{
    auto model = std::make_unique<NodeModelType>("difference", "Difference", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_difference_process);
    return model;
}
void
OpenSCADBuiltins::f_op_difference_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("difference() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        input.getValue("b", "{}") +
	std::string("    }") + 
        std::string("});"));
}

////////////////////////////////////////
// Intersection
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_op_intersection()
{
    auto model = std::make_unique<NodeModelType>("intersection", "Intersection", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_intersection_process);
    return model;
}
void
OpenSCADBuiltins::f_op_intersection_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("intersection() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        input.getValue("b", "{}") +
	std::string("    }") + 
        std::string("});"));
}


