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
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_union_process);
    return model;
}
void
OpenSCADBuiltins::f_op_union_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("union() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Difference
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_op_difference()
{
    auto model = std::make_unique<NodeModelType>("difference", "Difference", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::One);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_difference_process);
    return model;
}
void
OpenSCADBuiltins::f_op_difference_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("difference() {\n");
    out += std::string("    {\n");
    out +=     input.getValue("a", "{}");
    out += std::string("    }");
    out += std::string("    {\n");
    out +=     input.getValue("b", "{}");
    out += std::string("    }");
    out += std::string("}");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Intersection
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_op_intersection()
{
    auto model = std::make_unique<NodeModelType>("intersection", "Intersection", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_intersection_process);
    return model;
}
void
OpenSCADBuiltins::f_op_intersection_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("intersection() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}


