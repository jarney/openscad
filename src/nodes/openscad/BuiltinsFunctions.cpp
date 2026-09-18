#include "nodes/OpenSCADBuiltinFactory.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_FUNCTION.getName()

BINARY_NODE(function, concat, _OPENSCAD_NODE_CATEGORY, "Concatenate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("concat(") + input.getValue("a", "0") + "," + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(function, lookup, _OPENSCAD_NODE_CATEGORY, "Lookup", "value", DATA_VARIABLE, "table", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("lookup(") + input.getValue("value", "0") + ", " + input.getValue("table", "0") + std::string(")"));
}
UNARY_NODE(function, str, _OPENSCAD_NODE_CATEGORY, "To String", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("str(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(function, chr, _OPENSCAD_NODE_CATEGORY, "To Char", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("chr(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(function, ord, _OPENSCAD_NODE_CATEGORY, "To Ordinal", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("ord(") + input.getValue("x", "true") + std::string(")"));
}
BINARY_NODE(function, search, _OPENSCAD_NODE_CATEGORY, "Search for value", "needle", DATA_VARIABLE, "haystack", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("search(") + input.getValue("needle", "0") + ", " + input.getValue("haystack", "[]") + std::string(")"));
}

////////////////////////////////////////
// Version
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_function_version()
{
    auto model = std::make_unique<NodeModelType>("version", "Version", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_function_version_process);
    return model;
}
void
OpenSCADBuiltins::f_function_version_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("version()"));
}
////////////////////////////////////////
// Version Number
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_function_version_num()
{
    auto model = std::make_unique<NodeModelType>("version_num", "Version Number", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_function_version_process);
    return model;
}
void
OpenSCADBuiltins::f_function_version_num_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", std::string("version_num()"));
}

////////////////////////////////////////
// Parent Module
////////////////////////////////////////
UNARY_NODE(function, parent_module, _OPENSCAD_NODE_CATEGORY, "Parent Module Name", "index", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("parent_module(") + input.getValue("index", "0") + std::string(")"));
}

