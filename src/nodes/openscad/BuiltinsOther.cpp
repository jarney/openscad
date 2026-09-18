#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

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
    auto nodeType = std::make_unique<NodeType>("render", "Render", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_other_render_process);
    return nodeType;
}

void
Builtins::f_other_render_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "convexity");
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
    auto nodeType = std::make_unique<NodeType>("children", "Children", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "index"), "index");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_other_children_process);
    return nodeType;
}

void
Builtins::f_other_children_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "index");
    
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
    auto nodeType = std::make_unique<NodeType>("assert", "Assert", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "condition"), "condition");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "message"), "message");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_other_assert_process);
    return nodeType;
}

void
Builtins::f_other_assert_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "condition");
    conditionalArg(args, input, node, "message");
    
    std::string out;
    out += std::string("assert(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
    
}
