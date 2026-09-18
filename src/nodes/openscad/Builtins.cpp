#include "nodes/NodeFactory.hpp"
#include "nodes/gui/NodeEditorWidget.hpp"
#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

const NodeModelCategory Builtins::CATEGORY_SYNTAX("syntax", "Syntax", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_CONST("const", "Constants", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_OPERATOR("operator", "Operators", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_2D("2d", "2D Primitives", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_3D("3d", "3d Primitives", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_BOOLEAN_OPS("op", "Boolean Operations", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_XFORM("xform", "Transformations", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_LIST("list", "List", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_FUNCTION("function", "Functions", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_FLOW("flow", "Flow Control", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_MATH("math", "Mathematical", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_TYPETEST("typetest", "Type Test", "../resources/icons/stopbutton.png");
const NodeModelCategory Builtins::CATEGORY_OTHER("other", "Other", "../resources/icons/stopbutton.png");

#define _OPENSCAD_NODE_REGISTER(name) ret->registerModel(std::make_unique<NodeFactoryTyped>(f_##name()))

std::shared_ptr<NodeProgramModelRegistry>
Builtins::registerDataModels()
{
    auto ret = std::make_shared<NodeProgramModelRegistry>();

    ret->registerCategory(CATEGORY_SYNTAX);
    ret->registerCategory(CATEGORY_CONST);
    ret->registerCategory(CATEGORY_OPERATOR);
    ret->registerCategory(CATEGORY_2D);
    ret->registerCategory(CATEGORY_3D);
    ret->registerCategory(CATEGORY_XFORM);
    ret->registerCategory(CATEGORY_LIST);
    ret->registerCategory(CATEGORY_BOOLEAN_OPS);
    ret->registerCategory(CATEGORY_FLOW);
    ret->registerCategory(CATEGORY_TYPETEST);
    ret->registerCategory(CATEGORY_OTHER);
    ret->registerCategory(CATEGORY_FUNCTION);
    ret->registerCategory(CATEGORY_MATH);
    
    // Syntax
    _OPENSCAD_NODE_REGISTER(syntax_assign);
//    _OPENSCAD_NODE_REGISTER(syntax_assign_list);
    _OPENSCAD_NODE_REGISTER(syntax_variable);
//    _OPENSCAD_NODE_REGISTER(syntax_module);
//    _OPENSCAD_NODE_REGISTER(syntax_function);
//    _OPENSCAD_NODE_REGISTER(syntax_include);
//    _OPENSCAD_NODE_REGISTER(syntax_use);
    
    // Constants
    _OPENSCAD_NODE_REGISTER(const_true);
    _OPENSCAD_NODE_REGISTER(const_false);
    _OPENSCAD_NODE_REGISTER(const_int);
    _OPENSCAD_NODE_REGISTER(const_float);
    _OPENSCAD_NODE_REGISTER(const_string);
    _OPENSCAD_NODE_REGISTER(const_undef);
    _OPENSCAD_NODE_REGISTER(const_pi);

    // Operators
    _OPENSCAD_NODE_REGISTER(operator_add);
    _OPENSCAD_NODE_REGISTER(operator_subtract);
    _OPENSCAD_NODE_REGISTER(operator_multiply);
    _OPENSCAD_NODE_REGISTER(operator_divide);
    _OPENSCAD_NODE_REGISTER(operator_modulo);
    _OPENSCAD_NODE_REGISTER(operator_exponentiate);
    _OPENSCAD_NODE_REGISTER(operator_lt);
    _OPENSCAD_NODE_REGISTER(operator_leq);
    _OPENSCAD_NODE_REGISTER(operator_eq);
    _OPENSCAD_NODE_REGISTER(operator_geq);
    _OPENSCAD_NODE_REGISTER(operator_gt);
    _OPENSCAD_NODE_REGISTER(operator_and);
    _OPENSCAD_NODE_REGISTER(operator_or);
    _OPENSCAD_NODE_REGISTER(operator_not);
    
    // 2D
    _OPENSCAD_NODE_REGISTER(2d_circle);
    _OPENSCAD_NODE_REGISTER(2d_square);
    _OPENSCAD_NODE_REGISTER(2d_polygon);
    _OPENSCAD_NODE_REGISTER(2d_text);
    _OPENSCAD_NODE_REGISTER(2d_projection);
    
    // 3D
    _OPENSCAD_NODE_REGISTER(3d_sphere);
    _OPENSCAD_NODE_REGISTER(3d_cube);
    _OPENSCAD_NODE_REGISTER(3d_cylinder);
    _OPENSCAD_NODE_REGISTER(3d_polyhedron);
    _OPENSCAD_NODE_REGISTER(3d_import);
    _OPENSCAD_NODE_REGISTER(3d_linear_extrude);
    _OPENSCAD_NODE_REGISTER(3d_rotate_extrude);
    _OPENSCAD_NODE_REGISTER(3d_surface);
    _OPENSCAD_NODE_REGISTER(3d_dxf_dim);
    _OPENSCAD_NODE_REGISTER(3d_dxf_cross);

    // Transformations
    _OPENSCAD_NODE_REGISTER(xform_translate);
    _OPENSCAD_NODE_REGISTER(xform_rotate);
    _OPENSCAD_NODE_REGISTER(xform_scale);
    _OPENSCAD_NODE_REGISTER(xform_resize);
    _OPENSCAD_NODE_REGISTER(xform_mirror);
    _OPENSCAD_NODE_REGISTER(xform_multmatrix);
    _OPENSCAD_NODE_REGISTER(xform_color);
    _OPENSCAD_NODE_REGISTER(xform_offset);
    _OPENSCAD_NODE_REGISTER(xform_hull);
    _OPENSCAD_NODE_REGISTER(xform_fill);
    _OPENSCAD_NODE_REGISTER(xform_minkowski);

    // Lists
    _OPENSCAD_NODE_REGISTER(list_index);
    _OPENSCAD_NODE_REGISTER(list_get_xyz);
    _OPENSCAD_NODE_REGISTER(list_set_xyz);
    _OPENSCAD_NODE_REGISTER(list_get_xy);
    _OPENSCAD_NODE_REGISTER(list_set_xy);
    _OPENSCAD_NODE_REGISTER(list_get_rgba);
    _OPENSCAD_NODE_REGISTER(list_set_rgba);
    _OPENSCAD_NODE_REGISTER(list_set_range);

    // Boolean Operations
    _OPENSCAD_NODE_REGISTER(op_union);
    _OPENSCAD_NODE_REGISTER(op_difference);
    _OPENSCAD_NODE_REGISTER(op_intersection);

    // Flow control
    _OPENSCAD_NODE_REGISTER(flow_for);
    _OPENSCAD_NODE_REGISTER(flow_intersection_for);
    _OPENSCAD_NODE_REGISTER(flow_if);
    _OPENSCAD_NODE_REGISTER(flow_let);
    _OPENSCAD_NODE_REGISTER(flow_comment);
    _OPENSCAD_NODE_REGISTER(flow_group);
    _OPENSCAD_NODE_REGISTER(flow_output);
    
    // Type Test functions
    _OPENSCAD_NODE_REGISTER(typetest_is_bool);
    _OPENSCAD_NODE_REGISTER(typetest_is_string);
    _OPENSCAD_NODE_REGISTER(typetest_is_num);
    _OPENSCAD_NODE_REGISTER(typetest_is_function);
    _OPENSCAD_NODE_REGISTER(typetest_is_list);
    _OPENSCAD_NODE_REGISTER(typetest_is_undef);

    // Other
    _OPENSCAD_NODE_REGISTER(other_echo);
    _OPENSCAD_NODE_REGISTER(other_render);
    _OPENSCAD_NODE_REGISTER(other_children);
    _OPENSCAD_NODE_REGISTER(other_assert);
    
    // Functions
    _OPENSCAD_NODE_REGISTER(function_concat);
    _OPENSCAD_NODE_REGISTER(function_lookup);
    _OPENSCAD_NODE_REGISTER(function_str);
    _OPENSCAD_NODE_REGISTER(function_chr);
    _OPENSCAD_NODE_REGISTER(function_ord);
    _OPENSCAD_NODE_REGISTER(function_search);
    _OPENSCAD_NODE_REGISTER(function_version);
    _OPENSCAD_NODE_REGISTER(function_version_num);
    _OPENSCAD_NODE_REGISTER(function_parent_module);
    
    // Math functions:
    _OPENSCAD_NODE_REGISTER(math_abs);
    _OPENSCAD_NODE_REGISTER(math_sign);
    _OPENSCAD_NODE_REGISTER(math_sin);
    _OPENSCAD_NODE_REGISTER(math_cos);
    _OPENSCAD_NODE_REGISTER(math_tan);
    _OPENSCAD_NODE_REGISTER(math_acos);
    _OPENSCAD_NODE_REGISTER(math_asin);
    _OPENSCAD_NODE_REGISTER(math_atan);
    _OPENSCAD_NODE_REGISTER(math_atan2);
    _OPENSCAD_NODE_REGISTER(math_floor);
    _OPENSCAD_NODE_REGISTER(math_round);
    _OPENSCAD_NODE_REGISTER(math_ceil);
    _OPENSCAD_NODE_REGISTER(math_ln);
    _OPENSCAD_NODE_REGISTER(math_len);
    _OPENSCAD_NODE_REGISTER(math_log);
    _OPENSCAD_NODE_REGISTER(math_pow);
    _OPENSCAD_NODE_REGISTER(math_sqrt);
    _OPENSCAD_NODE_REGISTER(math_exp);
    _OPENSCAD_NODE_REGISTER(math_rands);
    _OPENSCAD_NODE_REGISTER(math_min);
    _OPENSCAD_NODE_REGISTER(math_max);
    _OPENSCAD_NODE_REGISTER(math_norm);
    _OPENSCAD_NODE_REGISTER(math_cross);

    return ret;
}

std::string
Builtins::joinArguments(std::vector<std::string> list)
{
    std::string out;
    bool first = true;

    for (const std::string & s : list) {
	if (!first) {
	    out += std::string(",");
	    first = false;
	}
	out += s;
    }
    return out;
}

void
Builtins::conditionalArg(
    std::vector<std::string> & args,
    const PortFunctionData & input,
    const OpenSCADBuiltinModel & model,
    std::string key,
    std::string default_value)
{
    if (input.hasValue(key)) {
	args.push_back(key + std::string("=") + input.getValue(key, ""));
    }
    else if (model.hasValue(key)) {
	args.push_back(key + std::string("=") + model.getValue(key, ""));
    }
    else {
	args.push_back(key + std::string("=") + default_value);
    }
}

void
Builtins::conditionalArg(
    std::vector<std::string> & args,
    const PortFunctionData & input,
    const OpenSCADBuiltinModel & model,
    std::string key
    )
{
    if (input.hasValue(key)) {
	args.push_back(key + std::string("=") + input.getValue(key, ""));
    }
    else if (model.hasValue(key)) {
	args.push_back(key + std::string("=") + model.getValue(key, ""));
    }
}
