#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_CONST.getName()

NONARY_NODE(const, true, _OPENSCAD_NODE_CATEGORY, "True", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("true"));
}
NONARY_NODE(const, false, _OPENSCAD_NODE_CATEGORY, "False", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("false"));
}
NONARY_NODE(const, undef, _OPENSCAD_NODE_CATEGORY, "Undefined", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("undef"));
}
NONARY_NODE(const, pi, _OPENSCAD_NODE_CATEGORY, "PI", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("PI"));
}

// These are not macros because we want to put widgets in here
// to contain and edit their data.
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_int()
{
    auto model = std::make_unique<NodeModelType>("const_int", "Integer", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_float_process);
    return model;
}
void
OpenSCADBuiltins::f_const_int_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("15"));
}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_float()
{
    auto model = std::make_unique<NodeModelType>("const_float", "Float", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_float_process);
    return model;
}
void
OpenSCADBuiltins::f_const_float_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("15.0"));
}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_string()
{
    auto model = std::make_unique<NodeModelType>("const_string", "String", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_string_process);
    return model;
}
void
OpenSCADBuiltins::f_const_string_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("\"constant string\""));
}

