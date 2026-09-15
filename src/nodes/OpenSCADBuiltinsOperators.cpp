#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

BINARY_FUNCTION_BODY(math, add, "Add", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "+" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, subtract, "Subtract", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "-" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, multiply, "Multiply", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "*" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, divide, "Divide", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "/" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, modulo, "Modulo", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "%" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, exponentiate, "Exponentiate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "^" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, lt, "Less Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "<" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, leq, "Less Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "<=" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, eq, "Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "==" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, geq, "Greater Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + ">=" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, gt, "Greater Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + ">" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, and, "And", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "&&" + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, or, "Or", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + _input.getValue("a", "0") + "||" + _input.getValue("b", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, not, "Greater Than", "a", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(!") + _input.getValue("a", "0") + std::string(")"))
    )
