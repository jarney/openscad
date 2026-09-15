#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

UNARY_FUNCTION_BODY(math, is_bool, "Is Boolean", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_bool(") + _input.getValue("x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_string, "Is String", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_string(") + _input.getValue("x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_num, "Is Number", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_num(") + _input.getValue("x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_function, "Is Function", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_function") + _input.getValue("x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_list, "Is List", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_list") + _input.getValue("x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_undef, "Is Undefined", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_undef") + _input.getValue("x", "true") + std::string(")"))
    )
