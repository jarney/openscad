#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

UNARY_FUNCTION_BODY(math, asin, "Arc Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("asin(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sin, "Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sin(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, acos, "Arc Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("acos(") + _input.getValue("x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, cos, "Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("cos(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, abs, "Absolute Value", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("abs(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, atan, "Arc Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("atan(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, tan, "Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("tan(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sign, "Sign", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sign(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ceil, "Ceiling", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ceil(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, floor, "Floor", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("floor(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, round, "Round", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("round(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ln, "Natural Log", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ln(") + _input.getValue("x", "2.718") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, log, "Log Base 10", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("log(") + _input.getValue("x", "10") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, exp, "Exponential(e)", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("exp(") + _input.getValue("x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sqrt, "Square Root", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sqrt(") + _input.getValue("x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, norm, "Norm", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("norm(") + _input.getValue("x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, len, "Length", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("len(") + _input.getValue("x", "0") + std::string(")"))
    )


BINARY_FUNCTION_BODY(math, max, "Maximum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("max(") + _input.getValue("a", "0") + "," + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, pow, "Power", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("pow(") + _input.getValue("a", "0") + "," + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, min, "Minimum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("min(") + _input.getValue("a", "0") + "," + _input.getValue("b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, atan2, "Arc Tan(2)", "x", DATA_VARIABLE, "y", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("atan2(") + _input.getValue("x", "0") + "," + _input.getValue("y", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, cross, "Cross Product", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("cross(") + _input.getValue("a", "[1,0,0]") + "," + _input.getValue("b", "[0,1,0]") + std::string(")"))
    )


OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_math_rands()
{
    auto model = std::make_unique<NodeModelType>("rands", "Random Vector", "Math");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "min"), "max");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "min"), "max");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "n"), "n");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "seed"), "seed");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_math_rands_process);
    return model;
}
void
OpenSCADBuiltins::f_math_rands_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    auto args = std::string();

    args += input.getValue("min", "0");
    args += std::string(", ");
    args += input.getValue("max", "100");
    args += std::string(", ");
    args += input.getValue("n", "1");
    if (input.hasValue("seed")) {
	args += std::string(", ");
	args += input.getValue("seed", "0");
    }
    output.setValue("Geometry", std::string("rands(") + args + std::string(")"));
}

