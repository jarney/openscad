#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_2D.getName()

////////////////////////////////////////
// Circle
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_2d_circle()
{
    auto nodeType = std::make_unique<NodeType>("circle", "Circle", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_2d_circle_process);
    return nodeType;
}

void
Builtins::f_2d_circle_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("circle(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}
////////////////////////////////////////
// Square
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_2d_square()
{
    auto nodeType = std::make_unique<NodeType>("square", "Square", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "size"), "size");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "center"), "center");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_2d_square_process);
    return nodeType;
}

void
Builtins::f_2d_square_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "size");
    conditionalArg(args, input, node, "center");
    std::string out = std::string("square(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Polygon
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_2d_polygon()
{
    auto nodeType = std::make_unique<NodeType>("polygon", "Polygon", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "points"), "points");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "paths"), "paths");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_2d_polygon_process);
    return nodeType;
}

void
Builtins::f_2d_polygon_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "points", "[]");
    conditionalArg(args, input, node, "paths");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("polygon(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Text
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_2d_text()
{
    auto nodeType = std::make_unique<NodeType>("text", "Projection", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "text"), "text");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "size"), "size");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "font"), "font");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "direction"), "direction");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "language"), "language");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "script"), "script");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "halign"), "halign");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "valign"), "valign");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "spacing"), "spacing");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "em"), "em");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_2d_text_process);
    return nodeType;
}

void
Builtins::f_2d_text_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "text");
    conditionalArg(args, input, node, "size");
    conditionalArg(args, input, node, "font");
    conditionalArg(args, input, node, "direction");
    conditionalArg(args, input, node, "language");
    conditionalArg(args, input, node, "script");
    conditionalArg(args, input, node, "halign");
    conditionalArg(args, input, node, "valign");
    conditionalArg(args, input, node, "spacing");
    conditionalArg(args, input, node, "em");
    std::string arguments = joinArguments(args);
    std::string out = std::string("text(") + arguments + std::string(")");

    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Projection
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_2d_projection()
{
    auto nodeType = std::make_unique<NodeType>("projection", "Projection", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "cut"), "cut");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_2d_projection_process);
    return nodeType;
}

void
Builtins::f_2d_projection_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "cut");
    std::string out = std::string("projection(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}

