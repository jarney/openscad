#include "nodes/OpenSCADBuiltinFactory.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_OTHER.getName()

UNARY_NODE(other, echo, _OPENSCAD_NODE_CATEGORY, "Echo", "value", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("value(") + input.getValue("value", "1") + std::string(")"));
}
////////////////////////////////////////
// Render
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_other_render()
{
    auto model = std::make_unique<NodeModelType>("render", "Render", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_other_render_process);
    return model;
}

void
Builtins::f_other_render_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "convexity");
    std::string out;
    out += std::string("render(") + joinArguments(args) + std::string(") {\n");
    out +=     input.getValue("Geometry", "{}");
    out += "}";
    output.setValue("Geometry", out);
    
}
////////////////////////////////////////
// Children
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_other_children()
{
    auto model = std::make_unique<NodeModelType>("children", "Children", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "index"), "index");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_other_children_process);
    return model;
}

void
Builtins::f_other_children_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "index");
    
    std::string out;
    out += std::string("children(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Assert
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_other_assert()
{
    auto model = std::make_unique<NodeModelType>("assert", "Assert", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "condition"), "condition");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "message"), "message");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_other_assert_process);
    return model;
}

void
Builtins::f_other_assert_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "condition");
    conditionalArg(args, input, model, "message");
    
    std::string out;
    out += std::string("assert(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}
