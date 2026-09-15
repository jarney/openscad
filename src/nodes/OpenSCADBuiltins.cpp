#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"

#include "nodes/JNodeProgramEditor.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#define _OPENSCAD_NODE_REGISTER(name) ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_##name()))

std::shared_ptr<NodeProgramModelRegistry>
OpenSCADBuiltins::registerDataModels()
{
    auto ret = std::make_shared<NodeProgramModelRegistry>();

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

    // Boolean Operations
    _OPENSCAD_NODE_REGISTER(op_union);
    _OPENSCAD_NODE_REGISTER(op_difference);
    _OPENSCAD_NODE_REGISTER(op_intersection);

    // Transformations
    _OPENSCAD_NODE_REGISTER(xform_translate);
    //_OPENSCAD_NODE_REGISTER(xform_offset);
    _OPENSCAD_NODE_REGISTER(xform_scale);
    //_OPENSCAD_NODE_REGISTER(xform_rotate);
    _OPENSCAD_NODE_REGISTER(xform_mirror);
    _OPENSCAD_NODE_REGISTER(xform_resize);
    //_OPENSCAD_NODE_REGISTER(xform_multmatrix);
    _OPENSCAD_NODE_REGISTER(xform_hull);
    _OPENSCAD_NODE_REGISTER(xform_fill);
    _OPENSCAD_NODE_REGISTER(xform_minkowski);
    
    // Aeshetics
    _OPENSCAD_NODE_REGISTER(util_color);

    // Constants
    _OPENSCAD_NODE_REGISTER(const_true);
    _OPENSCAD_NODE_REGISTER(const_false);
    _OPENSCAD_NODE_REGISTER(const_int);
    _OPENSCAD_NODE_REGISTER(const_float);
    _OPENSCAD_NODE_REGISTER(const_string);
    _OPENSCAD_NODE_REGISTER(const_undef);
    _OPENSCAD_NODE_REGISTER(const_pi);
    _OPENSCAD_NODE_REGISTER(const_version);
    _OPENSCAD_NODE_REGISTER(const_version_num);

    // Output node, result of operations
    _OPENSCAD_NODE_REGISTER(output);
    //_OPENSCAD_NODE_REGISTER(echo);

    // Flow control
    _OPENSCAD_NODE_REGISTER(flow_for);
    _OPENSCAD_NODE_REGISTER(flow_if);
    //_OPENSCAD_NODE_REGISTER(flow_let);
    //_OPENSCAD_NODE_REGISTER(flow_group);
    //_OPENSCAD_NODE_REGISTER(flow_assert);
    //_OPENSCAD_NODE_REGISTER(flow_children);
    //_OPENSCAD_NODE_REGISTER(flow_module);
    //_OPENSCAD_NODE_REGISTER(flow_function);

    
    // Math functions:
    
    _OPENSCAD_NODE_REGISTER(math_asin);
    _OPENSCAD_NODE_REGISTER(math_sin);
    _OPENSCAD_NODE_REGISTER(math_acos);
    _OPENSCAD_NODE_REGISTER(math_abs);
    _OPENSCAD_NODE_REGISTER(math_atan);
    _OPENSCAD_NODE_REGISTER(math_atan2);
    _OPENSCAD_NODE_REGISTER(math_cos);
    _OPENSCAD_NODE_REGISTER(math_tan);
    _OPENSCAD_NODE_REGISTER(math_sign);
    _OPENSCAD_NODE_REGISTER(math_ceil);
    _OPENSCAD_NODE_REGISTER(math_floor);
    _OPENSCAD_NODE_REGISTER(math_round);
    _OPENSCAD_NODE_REGISTER(math_ln);
    _OPENSCAD_NODE_REGISTER(math_len);
    _OPENSCAD_NODE_REGISTER(math_log);
    _OPENSCAD_NODE_REGISTER(math_exp);
    _OPENSCAD_NODE_REGISTER(math_sqrt);
    _OPENSCAD_NODE_REGISTER(math_concat);
    _OPENSCAD_NODE_REGISTER(math_min);
    _OPENSCAD_NODE_REGISTER(math_pow);
    _OPENSCAD_NODE_REGISTER(math_max);
    _OPENSCAD_NODE_REGISTER(math_norm);
    _OPENSCAD_NODE_REGISTER(math_str);
    _OPENSCAD_NODE_REGISTER(math_chr);
    _OPENSCAD_NODE_REGISTER(math_ord);
    _OPENSCAD_NODE_REGISTER(math_cross);

    _OPENSCAD_NODE_REGISTER(math_add);
    _OPENSCAD_NODE_REGISTER(math_subtract);
    _OPENSCAD_NODE_REGISTER(math_multiply);
    _OPENSCAD_NODE_REGISTER(math_divide);
    _OPENSCAD_NODE_REGISTER(math_modulo);
    _OPENSCAD_NODE_REGISTER(math_exponentiate);
    _OPENSCAD_NODE_REGISTER(math_lt);
    _OPENSCAD_NODE_REGISTER(math_leq);
    _OPENSCAD_NODE_REGISTER(math_eq);
    _OPENSCAD_NODE_REGISTER(math_geq);
    _OPENSCAD_NODE_REGISTER(math_gt);
    _OPENSCAD_NODE_REGISTER(math_and);
    _OPENSCAD_NODE_REGISTER(math_or);
    _OPENSCAD_NODE_REGISTER(math_not);
    _OPENSCAD_NODE_REGISTER(math_lookup);
    _OPENSCAD_NODE_REGISTER(math_search);
    _OPENSCAD_NODE_REGISTER(math_parent_module);
    
    _OPENSCAD_NODE_REGISTER(math_is_bool);
    _OPENSCAD_NODE_REGISTER(math_is_string);
    _OPENSCAD_NODE_REGISTER(math_is_num);
    _OPENSCAD_NODE_REGISTER(math_is_function);
    _OPENSCAD_NODE_REGISTER(math_is_list);
    _OPENSCAD_NODE_REGISTER(math_is_undef);

    _OPENSCAD_NODE_REGISTER(math_rands);

    _OPENSCAD_NODE_REGISTER(import_dxf_dim);
    _OPENSCAD_NODE_REGISTER(import_dxf_cross);
    
    return ret;
}

std::string
OpenSCADBuiltins::joinArguments(std::vector<std::string> list)
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
OpenSCADBuiltins::conditionalArg(
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
OpenSCADBuiltins::conditionalArg(
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
