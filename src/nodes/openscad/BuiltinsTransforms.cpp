#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_XFORM.getName()

////////////////////////////////////////
// Translate
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_translate()
{
    auto nodeType = std::make_unique<NodeType>("translate", "Translate", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_translate_process);
    return nodeType;
}
void
Builtins::f_xform_translate_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "v");
    
    std::string out;
    out += std::string("translate(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Rotate
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_rotate()
{
    auto nodeType = std::make_unique<NodeType>("rotate", "Rotate", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "a"), "a");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_rotate_process);
    return nodeType;
}
void
Builtins::f_xform_rotate_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "a");
    conditionalArg(args, input, node, "v");
    
    std::string out;
    out += std::string("rotate(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}
////////////////////////////////////////
// Scale
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_scale()
{
    auto nodeType = std::make_unique<NodeType>("scale", "Scale", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_scale_process);
    return nodeType;
}
void
Builtins::f_xform_scale_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "v");

    std::string out = std::string();
    out += std::string("scale(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Resize
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_resize()
{
    auto nodeType = std::make_unique<NodeType>("resize", "Resize", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "newsize"), "newsize");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "auto"), "auto");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_resize_process);
    return nodeType;
}
void
Builtins::f_xform_resize_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out = std::string();
    std::vector<std::string> args;
    conditionalArg(args, input, node, "newsize");
    conditionalArg(args, input, node, "auto");
    conditionalArg(args, input, node, "convexity");

    out += std::string("resize(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Mirror
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_mirror()
{
    auto nodeType = std::make_unique<NodeType>("mirror", "Mirror", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_mirror_process);
    return nodeType;
}
void
Builtins::f_xform_mirror_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "v");
    
    std::string out;
    out += std::string("mirror(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Multmatrix
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_multmatrix()
{
    auto nodeType = std::make_unique<NodeType>("multmatrix", "Multiply by Matrix", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "m"), "m");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_multmatrix_process);
    return nodeType;
}
void
Builtins::f_xform_multmatrix_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "m");
    
    std::string out;
    out += std::string("multmatrix(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Color
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_color()
{
    auto nodeType = std::make_unique<NodeType>("color", "Color", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "c"), "color");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "alpha"), "alpha");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_color_process);
    return nodeType;
}

void
Builtins::f_xform_color_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "v");
    
    std::string out = std::string();
    out += std::string("color(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Offset
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_offset()
{
    auto nodeType = std::make_unique<NodeType>("offset", "Offset", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "delta"), "delta");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "chamfer"), "chamfer");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_color_process);
    return nodeType;
}

void
Builtins::f_xform_offset_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "delta");
    conditionalArg(args, input, node, "chamfer");
    
    std::string out = std::string();
    out += std::string("offset(") + joinArguments(args) + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Convex Hull
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_hull()
{
    auto nodeType = std::make_unique<NodeType>("hull", "Convex Hull", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_hull_process);
    return nodeType;
}
void
Builtins::f_xform_hull_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("hull() {\n");
    out += std::string("    {\n");
    out += input.getValue("a", "{}");
    out += std::string("    }");
    out += std::string("})");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Fill
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_fill()
{
    auto nodeType = std::make_unique<NodeType>("fill", "Fill Solid", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_fill_process);
    return nodeType;
}
void
Builtins::f_xform_fill_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("Geometry", std::string("fill() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
        std::string("})"));
}

////////////////////////////////////////
// Minkowski
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_xform_minkowski()
{
    auto nodeType = std::make_unique<NodeType>("minkowski", "Minkowski", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_xform_minkowski_process);
    return nodeType;
}
void
Builtins::f_xform_minkowski_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("Geometry", std::string("minkowski() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        input.getValue("b", "{}") +
	std::string("    }") + 
        std::string("})"));
}

