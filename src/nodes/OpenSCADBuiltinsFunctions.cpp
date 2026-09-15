#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

UNARY_FUNCTION_BODY(math, str, "To String", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("str(") + _input.getValue("x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, chr, "To Char", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("chr(") + _input.getValue("x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ord, "To Ordinal", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ord(") + _input.getValue("x", "true") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, concat, "Concatenate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("concat(") + _input.getValue("a", "0") + "," + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, lookup, "Lookup", "value", DATA_VARIABLE, "table", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("lookup(") + _input.getValue("value", "0") + ", " + _input.getValue("table", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, search, "Search for value", "needle", DATA_VARIABLE, "haystack", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("search(") + _input.getValue("needle", "0") + ", " + _input.getValue("haystack", "[]") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, parent_module, "Parent Module Name", "index", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("parent_module(") + _input.getValue("needle", "0") + std::string(")"))
    )

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_version()
{
    auto model = std::make_unique<NodeModelType>("version", "Version", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_version_process);
    return model;
}
void
OpenSCADBuiltins::f_const_version_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("version()"));
}
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_version_num()
{
    auto model = std::make_unique<NodeModelType>("version_num", "Version Number", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_pi_process);
    return model;
}
void
OpenSCADBuiltins::f_const_version_num_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("version_num()"));
}
