#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"

#include "nodes/JNodeProgramEditor.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

std::shared_ptr<NodeProgramModelRegistry>
OpenSCADBuiltins::registerDataModels()
{
    auto ret = std::make_shared<NodeProgramModelRegistry>();

    // 3d primitives
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_sphere()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_cube()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_cylinder()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_polyhedron()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_text()));

    // 2d primitives
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_circle()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_polygon()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_surface()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_square()));

    // 2d -> 3d
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_rotate_extrude()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_prim_linear_extrude()));
    
    
    // CSG operations
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_op_union()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_op_difference()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_op_intersection()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_op_hull()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_op_fill()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_op_minkowski()));

    // Aeshetics
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_util_color()));

    // Constants
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_true()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_false()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_int()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_float()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_string()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_undef()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_pi()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_version()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_const_version_num()));

    // Output node, result of operations
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_output()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_echo()));

    // Flow control
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_for()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_if()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_let()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_group()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_assert()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_children()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_module()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_flow_function()));

    
    // Transformations
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_translate()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_offset()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_scale()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_rotate()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_mirror()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_resize()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_multmatrix()));
    //ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_xform_projection()));

    // Math functions:
    
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_asin()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_sin()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_acos()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_abs()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_atan()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_atan2()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_cos()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_tan()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_sign()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_ceil()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_floor()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_round()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_ln()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_len()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_log()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_exp()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_sqrt()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_concat()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_min()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_pow()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_max()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_norm()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_str()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_chr()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_ord()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_cross()));

    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_add()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_subtract()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_multiply()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_divide()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_modulo()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_exponentiate()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_lt()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_leq()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_eq()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_geq()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_gt()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_and()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_or()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_not()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_lookup()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_search()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_parent_module()));
    
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_is_bool()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_is_string()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_is_num()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_is_function()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_is_list()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_is_undef()));

    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_math_rands()));

    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_import_dxf_dim()));
    ret->registerModel(std::make_unique<OpenSCADBuiltinFactory>(f_import_dxf_cross()));
    
    return ret;
}

