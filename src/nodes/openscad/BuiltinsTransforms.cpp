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
    auto model = std::make_unique<NodeType>("translate", "Translate", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_translate_process);
    return model;
}
void
Builtins::f_xform_translate_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "v");
    
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
    auto model = std::make_unique<NodeType>("rotate", "Rotate", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "a"), "a");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_rotate_process);
    return model;
}
void
Builtins::f_xform_rotate_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "a");
    conditionalArg(args, input, model, "v");
    
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
    auto model = std::make_unique<NodeType>("scale", "Scale", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_scale_process);
    return model;
}
void
Builtins::f_xform_scale_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "v");

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
    auto model = std::make_unique<NodeType>("resize", "Resize", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "newsize"), "newsize");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "auto"), "auto");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_resize_process);
    return model;
}
void
Builtins::f_xform_resize_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    std::vector<std::string> args;
    conditionalArg(args, input, model, "newsize");
    conditionalArg(args, input, model, "auto");
    conditionalArg(args, input, model, "convexity");

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
    auto model = std::make_unique<NodeType>("mirror", "Mirror", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_mirror_process);
    return model;
}
void
Builtins::f_xform_mirror_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "v");
    
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
    auto model = std::make_unique<NodeType>("multmatrix", "Multiply by Matrix", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "m"), "m");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_multmatrix_process);
    return model;
}
void
Builtins::f_xform_multmatrix_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "m");
    
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
    auto model = std::make_unique<NodeType>("color", "Color", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "c"), "color");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "alpha"), "alpha");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_color_process);
    return model;
}

void
Builtins::f_xform_color_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "v");
    
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
    auto model = std::make_unique<NodeType>("offset", "Offset", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "delta"), "delta");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "chamfer"), "chamfer");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_color_process);
    return model;
}

void
Builtins::f_xform_offset_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "delta");
    conditionalArg(args, input, model, "chamfer");
    
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
    auto model = std::make_unique<NodeType>("hull", "Convex Hull", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_hull_process);
    return model;
}
void
Builtins::f_xform_hull_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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
    auto model = std::make_unique<NodeType>("fill", "Fill Solid", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_fill_process);
    return model;
}
void
Builtins::f_xform_fill_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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
    auto model = std::make_unique<NodeType>("minkowski", "Minkowski", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_minkowski_process);
    return model;
}
void
Builtins::f_xform_minkowski_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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

