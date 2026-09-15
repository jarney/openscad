#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

NONARY_NODE(const, true, "True", "Constants", "value", DATA_VARIABLE)
{
    _output.setValue("value", std::string("true"));
}
NONARY_NODE(const, false, "False", "Constants", "value", DATA_VARIABLE)
{
    _output.setValue("value", std::string("false"));
}
NONARY_NODE(const, undef, "Undefined", "Constants", "value", DATA_VARIABLE)
{
    _output.setValue("value", std::string("undef"));
}
NONARY_NODE(const, pi, "PI", "Constants", "value", DATA_VARIABLE)
{
    _output.setValue("value", std::string("PI"));
}

// These are not macros because we want to put widgets in here
// to contain and edit their data.
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_int()
{
    auto model = std::make_unique<NodeModelType>("const_int", "Integer", "Constants");
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
    auto model = std::make_unique<NodeModelType>("const_float", "Float", "Constants");
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
    auto model = std::make_unique<NodeModelType>("const_string", "String", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_string_process);
    return model;
}
void
OpenSCADBuiltins::f_const_string_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("\"constant string\""));
}

