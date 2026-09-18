#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeData>
#include "nodes/OpenSCADDataTypes.hpp"
#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

namespace JNodes {
    namespace openscad {

/**
 * This is a static class that contains all of the OpenSCAD builtins.
 * This is organized the same way as the famous "OpenSCAD Cheat Sheet"
 * so that it is easy to navigate.  Most of the actual implementations
 * of these methods are divided into files according to section
 * in the "Cheat Sheet" in order to keep the implementations
 * organized and easy to find.
 *
 * Many of the implementations are done through pre-processor
 * macros because there is very little variation between them, but
 * the constructs need to be fully generic in order to accommodate
 * user modules and functions with a lot of flexibility.  We
 * try to keep the scope of these pre-processor macros short
 * and prefixed so as to minimize the risk of pre-processor
 * namespace collisions.
 */
class Builtins {
public:
    using RegistryItemPtr = std::unique_ptr<NodeModelType>;

    static const NodeModelCategory CATEGORY_SYNTAX;
    static const NodeModelCategory CATEGORY_CONST;
    static const NodeModelCategory CATEGORY_OPERATOR;
    static const NodeModelCategory CATEGORY_2D;
    static const NodeModelCategory CATEGORY_3D;
    static const NodeModelCategory CATEGORY_XFORM;
    static const NodeModelCategory CATEGORY_LIST;
    static const NodeModelCategory CATEGORY_BOOLEAN_OPS;
    static const NodeModelCategory CATEGORY_FLOW;
    static const NodeModelCategory CATEGORY_TYPETEST;
    static const NodeModelCategory CATEGORY_OTHER;
    static const NodeModelCategory CATEGORY_FUNCTION;
    static const NodeModelCategory CATEGORY_MATH;
    
    static std::shared_ptr<NodeProgramModelRegistry> registerDataModels();

    // Most nodes only need a type and a processor
    // because the instances hold no state.  Addition
    // is addition, and no modifiers are needed.
#define _OPENSCAD_NODE_DECL(name)                                    \
    static std::unique_ptr<NodeModelType> f_##name();				\
    static void f_##name##_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output);

    // Some nodes carry state for each instance
    // of the node.  For example, nodes
    // representing variables carry the name
    // of the variable inside the node's instance,
    // so typically they need an initializer to
    // set the state to some known initial state
    // and a widget to allow the user to modify
    // things like the name of the variable.
#define _OPENSCAD_NODE_DECL_FULL(name)                                    \
    static std::unique_ptr<NodeModelType> f_##name();                     \
    static void f_##name##_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output); \
    static void f_##name##_initializer(OpenSCADBuiltinModel & node);      \
    static QWidget* f_##name##_widget(OpenSCADBuiltinModel & node)

    // Syntax
    _OPENSCAD_NODE_DECL_FULL(syntax_assign);
    _OPENSCAD_NODE_DECL(syntax_assign_list);
    _OPENSCAD_NODE_DECL_FULL(syntax_variable);
    _OPENSCAD_NODE_DECL(syntax_module);
    _OPENSCAD_NODE_DECL(syntax_function);
    _OPENSCAD_NODE_DECL(syntax_include);
    _OPENSCAD_NODE_DECL(syntax_use);
    
    // Constants
    _OPENSCAD_NODE_DECL(const_true);
    _OPENSCAD_NODE_DECL(const_false);
    _OPENSCAD_NODE_DECL_FULL(const_int);
    _OPENSCAD_NODE_DECL_FULL(const_float);
    _OPENSCAD_NODE_DECL_FULL(const_string);
    _OPENSCAD_NODE_DECL(const_undef);
    _OPENSCAD_NODE_DECL(const_pi);

    // Operators
    _OPENSCAD_NODE_DECL(operator_add);
    _OPENSCAD_NODE_DECL(operator_subtract);
    _OPENSCAD_NODE_DECL(operator_multiply);
    _OPENSCAD_NODE_DECL(operator_divide);
    _OPENSCAD_NODE_DECL(operator_modulo);
    _OPENSCAD_NODE_DECL(operator_exponentiate);
    _OPENSCAD_NODE_DECL(operator_lt);
    _OPENSCAD_NODE_DECL(operator_leq);
    _OPENSCAD_NODE_DECL(operator_eq);
    _OPENSCAD_NODE_DECL(operator_geq);
    _OPENSCAD_NODE_DECL(operator_gt);
    _OPENSCAD_NODE_DECL(operator_and);
    _OPENSCAD_NODE_DECL(operator_or);
    _OPENSCAD_NODE_DECL(operator_not);

    // Modifiers (TODO)
    // How should these be handled from a graph/UI perspective?
    // Maybe like a checkbox on each geometry node?
    // Maybe like a modifier you can drop in the geometry stream?
    
    // 2d primitives
    _OPENSCAD_NODE_DECL(2d_circle);
    _OPENSCAD_NODE_DECL(2d_square);
    _OPENSCAD_NODE_DECL(2d_polygon);
    _OPENSCAD_NODE_DECL(2d_text);
    _OPENSCAD_NODE_DECL(2d_projection);
    
    // 3d Primitives
    _OPENSCAD_NODE_DECL(3d_sphere);
    _OPENSCAD_NODE_DECL(3d_cube);
    _OPENSCAD_NODE_DECL(3d_cylinder);
    _OPENSCAD_NODE_DECL(3d_polyhedron);
    _OPENSCAD_NODE_DECL(3d_import);
    _OPENSCAD_NODE_DECL(3d_linear_extrude);
    _OPENSCAD_NODE_DECL(3d_rotate_extrude);
    _OPENSCAD_NODE_DECL(3d_surface);
    _OPENSCAD_NODE_DECL(3d_dxf_dim);
    _OPENSCAD_NODE_DECL(3d_dxf_cross);

    // Transformations
    _OPENSCAD_NODE_DECL(xform_translate);
    _OPENSCAD_NODE_DECL(xform_rotate);
    _OPENSCAD_NODE_DECL(xform_scale);
    _OPENSCAD_NODE_DECL(xform_resize);
    _OPENSCAD_NODE_DECL(xform_mirror);
    _OPENSCAD_NODE_DECL(xform_multmatrix);
    _OPENSCAD_NODE_DECL(xform_color);
    _OPENSCAD_NODE_DECL(xform_offset);
    _OPENSCAD_NODE_DECL(xform_hull);
    _OPENSCAD_NODE_DECL(xform_fill);
    _OPENSCAD_NODE_DECL(xform_minkowski);

    // Lists
    _OPENSCAD_NODE_DECL(list_index);
    _OPENSCAD_NODE_DECL(list_get_xyz);
    _OPENSCAD_NODE_DECL(list_set_xyz);
    _OPENSCAD_NODE_DECL(list_get_xy);
    _OPENSCAD_NODE_DECL(list_set_xy);
    _OPENSCAD_NODE_DECL(list_get_rgba);
    _OPENSCAD_NODE_DECL(list_set_rgba);
    _OPENSCAD_NODE_DECL(list_set_range);

    // Boolean operations
    _OPENSCAD_NODE_DECL(op_union);
    _OPENSCAD_NODE_DECL(op_difference);
    _OPENSCAD_NODE_DECL(op_intersection);

    // List Comprehensions (probably not needed?)
    // Maybe this can just be syntactic sugar?

    // Flow control
    _OPENSCAD_NODE_DECL_FULL(flow_for);
    _OPENSCAD_NODE_DECL(flow_intersection_for);
    _OPENSCAD_NODE_DECL(flow_if);
    _OPENSCAD_NODE_DECL(flow_let);
    _OPENSCAD_NODE_DECL_FULL(flow_comment);
    _OPENSCAD_NODE_DECL(flow_group);
    _OPENSCAD_NODE_DECL(flow_output);

    // Type Test functions
    _OPENSCAD_NODE_DECL(typetest_is_bool);
    _OPENSCAD_NODE_DECL(typetest_is_string);
    _OPENSCAD_NODE_DECL(typetest_is_num);
    _OPENSCAD_NODE_DECL(typetest_is_function);
    _OPENSCAD_NODE_DECL(typetest_is_list);
    _OPENSCAD_NODE_DECL(typetest_is_undef);

    // Other
    _OPENSCAD_NODE_DECL(other_echo);
    _OPENSCAD_NODE_DECL(other_render);
    _OPENSCAD_NODE_DECL(other_children);
    _OPENSCAD_NODE_DECL(other_assert);
    
    // Functions
    _OPENSCAD_NODE_DECL(function_concat);
    _OPENSCAD_NODE_DECL(function_lookup);
    _OPENSCAD_NODE_DECL(function_str);
    _OPENSCAD_NODE_DECL(function_chr);
    _OPENSCAD_NODE_DECL(function_ord);
    _OPENSCAD_NODE_DECL(function_search);
    _OPENSCAD_NODE_DECL(function_version);
    _OPENSCAD_NODE_DECL(function_version_num);
    _OPENSCAD_NODE_DECL(function_parent_module);

    // Mathematical
    _OPENSCAD_NODE_DECL(math_abs);
    _OPENSCAD_NODE_DECL(math_sign);
    _OPENSCAD_NODE_DECL(math_sin);
    _OPENSCAD_NODE_DECL(math_cos);
    _OPENSCAD_NODE_DECL(math_tan);
    _OPENSCAD_NODE_DECL(math_acos);
    _OPENSCAD_NODE_DECL(math_asin);
    _OPENSCAD_NODE_DECL(math_atan);
    _OPENSCAD_NODE_DECL(math_atan2);
    _OPENSCAD_NODE_DECL(math_floor);
    _OPENSCAD_NODE_DECL(math_round);
    _OPENSCAD_NODE_DECL(math_ceil);
    _OPENSCAD_NODE_DECL(math_ln);
    _OPENSCAD_NODE_DECL(math_len);
    _OPENSCAD_NODE_DECL(math_log);
    _OPENSCAD_NODE_DECL(math_pow);
    _OPENSCAD_NODE_DECL(math_sqrt);
    _OPENSCAD_NODE_DECL(math_exp);
    _OPENSCAD_NODE_DECL(math_rands);
    _OPENSCAD_NODE_DECL(math_min);
    _OPENSCAD_NODE_DECL(math_max);
    _OPENSCAD_NODE_DECL(math_norm);
    _OPENSCAD_NODE_DECL(math_cross);
    
#undef _OPENSCAD_NODE_DECL
private:
    static std::string joinArguments(std::vector<std::string> list);

    /**
     * This adds the argument to the list
     * and if it is not present in the input
     * or the model, the default value is used.
     */
    static void conditionalArg(
	std::vector<std::string> & args,
	const PortFunctionData & input,
	const OpenSCADBuiltinModel & model,
	std::string key,
	std::string default_value
    );
    /**
     * This adds the argument to the list if and only if
     * the value is present.
     */
    static void conditionalArg(
	std::vector<std::string> & args,
	const PortFunctionData & input,
	const OpenSCADBuiltinModel & model,
	std::string key
    );
};
	

    } // End openscad
} // End JNodes

