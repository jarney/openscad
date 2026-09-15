#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeData>
#include "nodes/OpenSCADDataTypes.hpp"
#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

class OpenSCADBuiltins {
public:
    using RegistryItemPtr = std::unique_ptr<NodeModelType>;
    static std::shared_ptr<NodeProgramModelRegistry> registerDataModels();

    // Math
#define _OPENSCAD_NODE_PROCESSOR_PROTOTYPE const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output
#define _OPENSCAD_NODE_DECL(name)                                    \
    static std::unique_ptr<NodeModelType> f_##name();				\
    static void f_##name##_process(_OPENSCAD_NODE_PROCESSOR_PROTOTYPE);

    // 3d Primitives
    _OPENSCAD_NODE_DECL(prim_sphere);
    _OPENSCAD_NODE_DECL(prim_cube);
    _OPENSCAD_NODE_DECL(prim_cylinder);
    _OPENSCAD_NODE_DECL(prim_polyhedron);
    _OPENSCAD_NODE_DECL(prim_linear_extrude);
    _OPENSCAD_NODE_DECL(prim_rotate_extrude);
    _OPENSCAD_NODE_DECL(prim_fill);

    // 2d primitives
    _OPENSCAD_NODE_DECL(prim_circle);
    _OPENSCAD_NODE_DECL(prim_polygon);
    _OPENSCAD_NODE_DECL(prim_surface);
    _OPENSCAD_NODE_DECL(prim_square);
    
    // Boolean operations
    _OPENSCAD_NODE_DECL(op_union);
    _OPENSCAD_NODE_DECL(op_difference);
    _OPENSCAD_NODE_DECL(op_intersection);
    _OPENSCAD_NODE_DECL(op_hull);
    _OPENSCAD_NODE_DECL(op_minkowski);

    // Special module operations
    _OPENSCAD_NODE_DECL(mod_children);

    // Debug/utility
    _OPENSCAD_NODE_DECL(util_echo);
    _OPENSCAD_NODE_DECL(util_assert);
    _OPENSCAD_NODE_DECL(util_render);
    _OPENSCAD_NODE_DECL(util_color);

    // Flow control
    _OPENSCAD_NODE_DECL(flow_for);
    static QWidget* f_flow_for_widget(OpenSCADBuiltinModel *model);
    
    _OPENSCAD_NODE_DECL(flow_if);
    _OPENSCAD_NODE_DECL(flow_let);
    _OPENSCAD_NODE_DECL(flow_group);

    // Transformations
    _OPENSCAD_NODE_DECL(xform_translate);
    _OPENSCAD_NODE_DECL(xform_offset);
    _OPENSCAD_NODE_DECL(xform_scale);
    _OPENSCAD_NODE_DECL(xform_rotate);
    _OPENSCAD_NODE_DECL(xform_mirror);
    _OPENSCAD_NODE_DECL(xform_resize);
    _OPENSCAD_NODE_DECL(xform_multmatrix);
    
    _OPENSCAD_NODE_DECL(math_asin);
    _OPENSCAD_NODE_DECL(math_sin);
    _OPENSCAD_NODE_DECL(math_acos);
    _OPENSCAD_NODE_DECL(math_cos);
    _OPENSCAD_NODE_DECL(math_abs);
    _OPENSCAD_NODE_DECL(math_atan);
    _OPENSCAD_NODE_DECL(math_atan2);
    _OPENSCAD_NODE_DECL(math_tan);
    _OPENSCAD_NODE_DECL(math_sign);
    _OPENSCAD_NODE_DECL(math_ceil);
    _OPENSCAD_NODE_DECL(math_floor);
    _OPENSCAD_NODE_DECL(math_round);
    _OPENSCAD_NODE_DECL(math_ln);
    _OPENSCAD_NODE_DECL(math_log);
    _OPENSCAD_NODE_DECL(math_exp);
    _OPENSCAD_NODE_DECL(math_pow);
    _OPENSCAD_NODE_DECL(math_sqrt);
    _OPENSCAD_NODE_DECL(math_min);
    _OPENSCAD_NODE_DECL(math_max);
    _OPENSCAD_NODE_DECL(math_concat);
    _OPENSCAD_NODE_DECL(math_norm);
    _OPENSCAD_NODE_DECL(math_len);
    _OPENSCAD_NODE_DECL(math_str);
    _OPENSCAD_NODE_DECL(math_chr);
    _OPENSCAD_NODE_DECL(math_ord);
    _OPENSCAD_NODE_DECL(math_cross);

    _OPENSCAD_NODE_DECL(math_add);
    _OPENSCAD_NODE_DECL(math_subtract);
    _OPENSCAD_NODE_DECL(math_multiply);
    _OPENSCAD_NODE_DECL(math_divide);
    _OPENSCAD_NODE_DECL(math_modulo);
    _OPENSCAD_NODE_DECL(math_exponentiate);
    _OPENSCAD_NODE_DECL(math_lt);
    _OPENSCAD_NODE_DECL(math_leq);
    _OPENSCAD_NODE_DECL(math_eq);
    _OPENSCAD_NODE_DECL(math_geq);
    _OPENSCAD_NODE_DECL(math_gt);
    _OPENSCAD_NODE_DECL(math_and);
    _OPENSCAD_NODE_DECL(math_or);
    _OPENSCAD_NODE_DECL(math_not);
    
    _OPENSCAD_NODE_DECL(math_is_bool);
    _OPENSCAD_NODE_DECL(math_is_string);
    _OPENSCAD_NODE_DECL(math_is_num);
    _OPENSCAD_NODE_DECL(math_is_function);
    _OPENSCAD_NODE_DECL(math_is_list);
    _OPENSCAD_NODE_DECL(math_is_undef);

    // Literal Constants
    _OPENSCAD_NODE_DECL(const_true);
    _OPENSCAD_NODE_DECL(const_false);
    _OPENSCAD_NODE_DECL(const_int);
    _OPENSCAD_NODE_DECL(const_float);
    _OPENSCAD_NODE_DECL(const_string);
    _OPENSCAD_NODE_DECL(const_undef);
    _OPENSCAD_NODE_DECL(const_version);
    _OPENSCAD_NODE_DECL(const_version_num);
    _OPENSCAD_NODE_DECL(const_pi);

    _OPENSCAD_NODE_DECL(math_rands);
    _OPENSCAD_NODE_DECL(math_lookup);
    _OPENSCAD_NODE_DECL(math_search);
    _OPENSCAD_NODE_DECL(math_parent_module);
    
    _OPENSCAD_NODE_DECL(import_dxf_dim);
    _OPENSCAD_NODE_DECL(import_dxf_cross);

    // Outputs
    _OPENSCAD_NODE_DECL(output);
    
#undef _OPENSCAD_NODE_DECL
#undef _OPENSCAD_NODE_PROCESSOR_PROTOTYPE

};
