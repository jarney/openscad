#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_TYPETEST.getName()

UNARY_NODE(typetest, is_bool, _OPENSCAD_NODE_CATEGORY, "Is Boolean", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("bool(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_string, _OPENSCAD_NODE_CATEGORY, "Is String", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("string(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_num, _OPENSCAD_NODE_CATEGORY, "Is Number", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("num(") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_function, _OPENSCAD_NODE_CATEGORY, "Is Function", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("function") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_list, _OPENSCAD_NODE_CATEGORY, "Is List", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("list") + input.getValue("x", "true") + std::string(")"));
}
UNARY_NODE(typetest, is_undef, _OPENSCAD_NODE_CATEGORY, "Is Undefined", "x", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("undef") + input.getValue("x", "true") + std::string(")"));
}
