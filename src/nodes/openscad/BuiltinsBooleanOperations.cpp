#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_BOOLEAN_OPS.getName()

////////////////////////////////////////
// Union
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_op_union()
{
    auto model = std::make_unique<NodeModelType>("union", "Union", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_union_process);
    return model;
}
void
Builtins::f_op_union_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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
Builtins::RegistryItemPtr
Builtins::f_op_difference()
{
    auto model = std::make_unique<NodeModelType>("difference", "Difference", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::One);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "b"), "b", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_difference_process);
    return model;
}
void
Builtins::f_op_difference_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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
Builtins::RegistryItemPtr
Builtins::f_op_intersection()
{
    auto model = std::make_unique<NodeModelType>("intersection", "Intersection", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_intersection_process);
    return model;
}
void
Builtins::f_op_intersection_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("intersection() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}


