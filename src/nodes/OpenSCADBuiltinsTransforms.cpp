#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_xform_translate()
{
    auto model = std::make_unique<NodeModelType>("translate", "Translate", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_translate_process);
    return model;
}
void
OpenSCADBuiltins::f_xform_translate_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("translate(");
    out += std::string("v=") + input.getValue("vector", "[0,0,0]");
    out += std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_xform_mirror()
{
    auto model = std::make_unique<NodeModelType>("mirror", "Mirror", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_mirror_process);
    return model;
}
void
OpenSCADBuiltins::f_xform_mirror_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("mirror(");
    out += std::string("v=") + input.getValue("vector", "[0,0,0]");
    out += std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_xform_scale()
{
    auto model = std::make_unique<NodeModelType>("scale", "Scale", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_scale_process);
    return model;
}
void
OpenSCADBuiltins::f_xform_scale_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("scale(");
    out += std::string("v=") + input.getValue("vector", "[0,0,0]");
    out += std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_xform_resize()
{
    auto model = std::make_unique<NodeModelType>("resize", "Resize", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_resize_process);
    return model;
}
void
OpenSCADBuiltins::f_xform_resize_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("resize(");
    out += std::string("v=") + input.getValue("vector", "[0,0,0]");
    out += std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}


OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_util_color()
{
    auto model = std::make_unique<NodeModelType>("color", "Color", "Utilities");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "c"), "color");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "alpha"), "alpha");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_util_color_process);
    return model;
}

void
OpenSCADBuiltins::f_util_color_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("color(");
    if (input.hasValue("color")) {
	out += std::string("c=") + input.getValue("color", "15.0") + ", ";
    }
    else if (input.hasValue("alpha")) {
	out += std::string("alpha=") + input.getValue("alpha", "30.0");
    }
    out += std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_op_hull()
{
    auto model = std::make_unique<NodeModelType>("hull", "Convex Hull", "Operations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_hull_process);
    return model;
}
void
OpenSCADBuiltins::f_op_hull_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("hull() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        input.getValue("b", "{}") +
	std::string("    }") + 
        std::string("});"));
}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_op_minkowski()
{
    auto model = std::make_unique<NodeModelType>("minkowski", "Minkowski", "Operations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_minkowski_process);
    return model;
}
void
OpenSCADBuiltins::f_op_minkowski_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("minkowski() {\n") +
	std::string("    {\n") + 
        input.getValue("a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        input.getValue("b", "{}") +
	std::string("    }") + 
        std::string("});"));
}

