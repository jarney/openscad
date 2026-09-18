#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_LIST.getName()

////////////////////////////////////////
// List Index
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_index()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_index", "List Index", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "list"), "list");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "index"), "index");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "out"), "out");
    model->setProcessor(f_list_index_process);
    return model;
}

void
Builtins::f_list_index_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    std::string out;
    out += input.getValue("list", "[]");
    out += std::string("[") +  input.getValue("index", "0") + std::string("]");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// List get xyz
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_get_xyz()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_get_xyz", "Get XYZ", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "list"), "list");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "x"), "x");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "y"), "y");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "z"), "z");
    model->setProcessor(f_list_get_xyz_process);
    return model;
}

void
Builtins::f_list_get_xyz_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    std::string x = input.getValue("list", "[0,0,0]") + std::string("[0]");
    std::string y = input.getValue("list", "[0,0,0]") + std::string("[1]");
    std::string z = input.getValue("list", "[0,0,0]") + std::string("[2]");
    output.setValue("x", x);
    output.setValue("y", y);
    output.setValue("z", z);
}

////////////////////////////////////////
// List set xyz
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_set_xyz()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_set_xyz", "Set XYZ", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "x"), "x");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "y"), "y");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "z"), "z");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "list"), "list");
    model->setProcessor(f_list_set_xyz_process);
    return model;
}

void
Builtins::f_list_set_xyz_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("x", "undef") + std::string(",");
    out += input.getValue("y", "undef") + std::string(",");
    out += input.getValue("z", "undef");
    out += std::string("]");
    output.setValue("list", out);
}

////////////////////////////////////////
// List get xyz
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_get_xy()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_get_xy", "Get XY", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "list"), "list");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "x"), "x");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "y"), "y");
    model->setProcessor(f_list_get_xy_process);
    return model;
}

void
Builtins::f_list_get_xy_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    std::string x = input.getValue("list", "[0,0]") + std::string("[0]");
    std::string y = input.getValue("list", "[0,0]") + std::string("[1]");
    output.setValue("x", x);
    output.setValue("y", y);
}

////////////////////////////////////////
// List set xy
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_set_xy()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_set_xy", "Set XY", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "x"), "x");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "y"), "y");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "list"), "list");
    model->setProcessor(f_list_set_xy_process);
    return model;
}

void
Builtins::f_list_set_xy_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("x", "undef") + std::string(",");
    out += input.getValue("y", "undef");
    out += std::string("]");
    output.setValue("list", out);
}

////////////////////////////////////////
// List get xyz
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_get_rgba()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_get_rgba", "Get RGBA", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "list"), "list");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "g"), "g");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "b"), "b");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "a"), "a");
    model->setProcessor(f_list_get_rgba_process);
    return model;
}

void
Builtins::f_list_get_rgba_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    std::string r = input.getValue("list", "[1,1,1,0]") + std::string("[0]");
    std::string g = input.getValue("list", "[1,1,1,0]") + std::string("[1]");
    std::string b = input.getValue("list", "[1,1,1,0]") + std::string("[2]");
    std::string a = input.getValue("list", "[1,1,1,0]") + std::string("[3]");
    output.setValue("r", r);
    output.setValue("g", g);
    output.setValue("b", b);
    output.setValue("a", a);
}

////////////////////////////////////////
// List set rgba
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_set_rgba()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_set_xyz", "Set XYZ", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "g"), "g");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "b"), "b");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "a"), "a");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "list"), "list");
    model->setProcessor(f_list_set_xyz_process);
    return model;
}

void
Builtins::f_list_set_rgba_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("r", "1") + std::string(",");
    out += input.getValue("g", "1") + std::string(",");
    out += input.getValue("b", "1") + std::string(",");
    out += input.getValue("a", "0");
    out += std::string("]");
    output.setValue("list", out);
    
}

////////////////////////////////////////
// List set range
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_list_set_range()
{
    auto model = std::make_unique<NodeModelType>("__builtin_list_set_range", "Range", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "start"), "start");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "end"), "end");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "increment"), "increment");
    model->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "range"), "range");
    model->setProcessor(f_list_set_range_process);
    return model;
}

void
Builtins::f_list_set_range_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out;
    out += std::string("[");
    out += input.getValue("start", "0");
    out += std::string(":") + input.getValue("end", "1");
    if (input.hasValue("increment")) {
	out += std::string(":") + input.getValue("increment", "1");
    }
    out += std::string("]");
    output.setValue("range", out);
}
