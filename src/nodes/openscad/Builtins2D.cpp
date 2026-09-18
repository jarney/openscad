#include "nodes/OpenSCADBuiltinFactory.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_2D.getName()

////////////////////////////////////////
// Circle
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_2d_circle()
{
    auto model = std::make_unique<NodeModelType>("circle", "Circle", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_2d_circle_process);
    return model;
}

void
OpenSCADBuiltins::f_2d_circle_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("circle(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}
////////////////////////////////////////
// Square
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_2d_square()
{
    auto model = std::make_unique<NodeModelType>("square", "Square", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "size"), "size");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "center"), "center");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_2d_square_process);
    return model;
}

void
OpenSCADBuiltins::f_2d_square_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "size");
    conditionalArg(args, input, model, "center");
    std::string out = std::string("square(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Polygon
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_2d_polygon()
{
    auto model = std::make_unique<NodeModelType>("polygon", "Polygon", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "points"), "points");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "paths"), "paths");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_2d_polygon_process);
    return model;
}

void
OpenSCADBuiltins::f_2d_polygon_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "points", "[]");
    conditionalArg(args, input, model, "paths");
    conditionalArg(args, input, model, "convexity");
    std::string out = std::string("polygon(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Text
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_2d_text()
{
    auto model = std::make_unique<NodeModelType>("text", "Projection", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "text"), "text");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "size"), "size");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "font"), "font");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "direction"), "direction");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "language"), "language");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "script"), "script");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "halign"), "halign");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "valign"), "valign");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "spacing"), "spacing");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "em"), "em");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_2d_text_process);
    return model;
}

void
OpenSCADBuiltins::f_2d_text_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "text");
    conditionalArg(args, input, model, "size");
    conditionalArg(args, input, model, "font");
    conditionalArg(args, input, model, "direction");
    conditionalArg(args, input, model, "language");
    conditionalArg(args, input, model, "script");
    conditionalArg(args, input, model, "halign");
    conditionalArg(args, input, model, "valign");
    conditionalArg(args, input, model, "spacing");
    conditionalArg(args, input, model, "em");
    std::string arguments = joinArguments(args);
    std::string out = std::string("text(") + arguments + std::string(")");

    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Projection
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_2d_projection()
{
    auto model = std::make_unique<NodeModelType>("projection", "Projection", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "cut"), "cut");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_2d_projection_process);
    return model;
}

void
OpenSCADBuiltins::f_2d_projection_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "cut");
    std::string out = std::string("projection(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

