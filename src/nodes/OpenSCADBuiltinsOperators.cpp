#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#define _OPENSCAD_NODE_CATEGORY "Operators"

BINARY_NODE(math, add, _OPENSCAD_NODE_CATEGORY, "Add", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "+" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, subtract, _OPENSCAD_NODE_CATEGORY, "Subtract", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "-" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, multiply, _OPENSCAD_NODE_CATEGORY, "Multiply", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "*" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, divide, _OPENSCAD_NODE_CATEGORY, "Divide", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "/" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, modulo, _OPENSCAD_NODE_CATEGORY, "Modulo", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "%" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, exponentiate, _OPENSCAD_NODE_CATEGORY, "Exponentiate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "^" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, lt, _OPENSCAD_NODE_CATEGORY, "Less Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, leq, _OPENSCAD_NODE_CATEGORY, "Less Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "<=" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, eq, _OPENSCAD_NODE_CATEGORY, "Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "==" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, geq, _OPENSCAD_NODE_CATEGORY, "Greater Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">=" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, gt, _OPENSCAD_NODE_CATEGORY, "Greater Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + ">" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, and, _OPENSCAD_NODE_CATEGORY, "And", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "&&" + input.getValue("b", "0") + std::string(")"));
}
BINARY_NODE(math, or, _OPENSCAD_NODE_CATEGORY, "Or", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(") + input.getValue("a", "0") + "||" + input.getValue("b", "0") + std::string(")"));
}
UNARY_NODE(math, not, _OPENSCAD_NODE_CATEGORY, "Greater Than", "a", DATA_VARIABLE, "out", DATA_VARIABLE)
{
    output.setValue("out", std::string("(!") + input.getValue("a", "0") + std::string(")"));
}
