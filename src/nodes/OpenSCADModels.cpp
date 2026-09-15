#include "OpenSCADModels.hpp"
#include <QtWidgets/QPushButton>
#include "JNodeProgramEditor.hpp"

/*********************************************/

static std::string FIND(const PortFunctionData & input, std::string key, std::string def)
{
    const auto it = input.find(key);
    if (it == input.end()) {
	fprintf(stderr, "Could not find key %s returning default\n", key.c_str());
	return def;
    }
    return it->second;
}
static bool KEY_EXISTS(const PortFunctionData & input, std::string key)
{
    const auto it = input.find(key);
    if (it == input.end()) {
	return false;
    }
    return true;
    
}

// Primitives
SCADModels::RegistryItemPtr
SCADModels::f_prim_sphere()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", "Primitives");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_sphere_process);
    return model;
}

void
SCADModels::f_prim_sphere_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("sphere(");
    if (KEY_EXISTS(input, "r")) {
	out += std::string("r=") + FIND(input, "r", "15.0");
    }
    else if (KEY_EXISTS(input, "d")) {
	out += std::string("d=") + FIND(input, "d", "30.0");
    }
    out += std::string(");\n");
    output["Geometry"] = out;
    
}

SCADModels::RegistryItemPtr
SCADModels::f_util_color()
{
    auto model = std::make_unique<NodeModelType>("color", "Color", "Utilities");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "c"), "color");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "alpha"), "alpha");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_util_color_process);
    return model;
}

void
SCADModels::f_util_color_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("color(");
    if (KEY_EXISTS(input, "color")) {
	out += std::string("c=") + FIND(input, "color", "15.0") + ", ";
    }
    else if (KEY_EXISTS(input, "alpha")) {
	out += std::string("alpha=") + FIND(input, "alpha", "30.0");
    }
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}


SCADModels::RegistryItemPtr
SCADModels::f_xform_translate()
{
    auto model = std::make_unique<NodeModelType>("translate", "Translate", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_translate_process);
    return model;
}
void
SCADModels::f_xform_translate_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("translate(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}
SCADModels::RegistryItemPtr
SCADModels::f_xform_mirror()
{
    auto model = std::make_unique<NodeModelType>("mirror", "Mirror", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_mirror_process);
    return model;
}
void
SCADModels::f_xform_mirror_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("mirror(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}
SCADModels::RegistryItemPtr
SCADModels::f_xform_scale()
{
    auto model = std::make_unique<NodeModelType>("scale", "Scale", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_scale_process);
    return model;
}
void
SCADModels::f_xform_scale_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("scale(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}

SCADModels::RegistryItemPtr
SCADModels::f_xform_resize()
{
    auto model = std::make_unique<NodeModelType>("resize", "Resize", "Transformations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_resize_process);
    return model;
}
void
SCADModels::f_xform_resize_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("resize(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}

SCADModels::RegistryItemPtr
SCADModels::f_flow_if()
{
    auto model = std::make_unique<NodeModelType>("if", "If", "Flow Control");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "condition"), "condition");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_if_process);
    return model;
}

void
SCADModels::f_flow_if_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("if(");
    out += FIND(input, "condition", "true");
    out += std::string(") {\n");
    out += FIND(input, "a", "");
    out += std::string("} else {\n");
    out += FIND(input, "b", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}

SCADModels::RegistryItemPtr
SCADModels::f_flow_for()
{
    fprintf(stderr, "For called\n");
    auto model = std::make_unique<NodeModelType>("for", "Loop", "Flow Control");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "start"), "start");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "end"), "end");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(SCADModels::f_flow_for_process);
    model->setWidgetFactory(SCADModels::f_flow_for_widget);
    return model;
}

QWidget*
SCADModels::f_flow_for_widget(NewSCADModel *model)
{
    fprintf(stderr, "Push button for edit of for content\n");
    QPushButton *button = new QPushButton();
    button->setText("Edit");
    QObject::connect(button, &QPushButton::clicked, [model]() {
	fprintf(stderr, "Edit button pushed\n");
	model->editGraph();
    });
    
    return button;
}


void
SCADModels::f_flow_for_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("for (i = ");
    out += FIND(input, "start", "0");
    out += std::string(";");
    out += std::string("i < ");
    out += FIND(input, "end", "10");
    out += std::string("; i++) {");
    out += std::string("// the sub-flow goes here...");
    out += std::string("}\n");
    output["Geometry"] = out;
}


SCADModels::RegistryItemPtr
SCADModels::f_prim_cube()
{
    auto model = std::make_unique<NodeModelType>("cube", "Cube", "Primitives");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "x"), "x");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "y"), "y");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "z"), "z");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_cube_process);
    return model;
}

void
SCADModels::f_prim_cube_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("        cube([") +
	FIND(input, "x", "15.0") + std::string(", ") +
	FIND(input, "y", "15.0") + std::string(", ") +
	FIND(input, "z", "15.0") +
	std::string("]);\n");
}

SCADModels::RegistryItemPtr
SCADModels::f_prim_cylinder()
{
    auto model = std::make_unique<NodeModelType>("cylinder", "Cylinder", "Primitives");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "h"), "h");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_cylinder_process);
    return model;
}
void
SCADModels::f_prim_cylinder_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("cylinder(") +
	FIND(input, "r", "15") + std::string(", ") +
	FIND(input, "h", "15") +
	std::string(");");
}

// Boolean operations
SCADModels::RegistryItemPtr
SCADModels::f_op_union()
{
    auto model = std::make_unique<NodeModelType>("union", "Union", "Operations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_union_process);
    return model;
}
void
SCADModels::f_op_union_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("union() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}

SCADModels::RegistryItemPtr
SCADModels::f_op_hull()
{
    auto model = std::make_unique<NodeModelType>("hull", "Convex Hull", "Operations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_hull_process);
    return model;
}
void
SCADModels::f_op_hull_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("hull() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}

SCADModels::RegistryItemPtr
SCADModels::f_op_minkowski()
{
    auto model = std::make_unique<NodeModelType>("minkowski", "Minkowski", "Operations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_minkowski_process);
    return model;
}
void
SCADModels::f_op_minkowski_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("minkowski() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}

SCADModels::RegistryItemPtr
SCADModels::f_op_difference()
{
    auto model = std::make_unique<NodeModelType>("difference", "Difference", "Operations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_difference_process);
    return model;
}
void
SCADModels::f_op_difference_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("difference() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}


SCADModels::RegistryItemPtr
SCADModels::f_op_intersection()
{
    auto model = std::make_unique<NodeModelType>("intersection", "Intersection", "Operations");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_intersection_process);
    return model;
}
void
SCADModels::f_op_intersection_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    fprintf(stderr, "Processing intersection\n");
    output["Geometry"] = std::string("intersection() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}


SCADModels::RegistryItemPtr
SCADModels::f_math_rands()
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
SCADModels::f_math_rands_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    auto args = std::string();

    args += FIND(input, "min", "0");
    args += std::string(", ");
    args += FIND(input, "max", "100");
    args += std::string(", ");
    args += FIND(input, "n", "1");
    if (KEY_EXISTS(input, "seed")) {
	args += std::string(", ");
	args += FIND(input, "seed", "0");
    }
    output["Geometry"] = std::string("rands(") + args + std::string(")");
}

SCADModels::RegistryItemPtr
SCADModels::f_import_dxf_dim()
{
    auto model = std::make_unique<NodeModelType>("dxf_dim", "Read DXF Dimension", "Import");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "name"), "name");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "layer"), "layer");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "origin"), "origin");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "scale"), "scale");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "out"), "out");
    model->setProcessor(f_import_dxf_dim_process);
    return model;
}
void
SCADModels::f_import_dxf_dim_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["out"] = std::string("dxf_dim(") +
	std::string("file=") + FIND(input, "file", "0") + "," +
	std::string("name=") + FIND(input, "name", "0") + "," +
	std::string("layer=") + FIND(input, "layer", "0") + "," +
	std::string("origin=") + FIND(input, "origin", "0") + "," +
	std::string("scale=") + FIND(input, "scale", "0") +
	std::string(")");

}

SCADModels::RegistryItemPtr
SCADModels::f_import_dxf_cross()
{
    auto model = std::make_unique<NodeModelType>("dxf_cross", "Read DXF Origin", "Import");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "layer"), "layer");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "origin"), "origin");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "scale"), "scale");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "out"), "out");
    model->setProcessor(f_import_dxf_cross_process);
    return model;
}

void
SCADModels::f_import_dxf_cross_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["out"] = std::string("dxf_cross(") +
	std::string("file=") + FIND(input, "file", "0") + "," +
	std::string("layer=") + FIND(input, "layer", "0") + "," +
	std::string("origin=") + FIND(input, "origin", "0") + "," +
	std::string("scale=") + FIND(input, "scale", "0") +
	std::string(")");
}

// Boolean Constants
SCADModels::RegistryItemPtr
SCADModels::f_const_true()
{
    auto model = std::make_unique<NodeModelType>("const_true", "True", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_true_process);
    return model;
}
void
SCADModels::f_const_true_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("true");
}


SCADModels::RegistryItemPtr
SCADModels::f_const_false()
{
    auto model = std::make_unique<NodeModelType>("const_false", "False", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_false_process);
    return model;
}
void
SCADModels::f_const_false_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("false");
}

// Other Constants
SCADModels::RegistryItemPtr
SCADModels::f_const_int()
{
    auto model = std::make_unique<NodeModelType>("const_int", "Integer", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_int_process);
    return model;
}
void
SCADModels::f_const_int_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("15");
}


SCADModels::RegistryItemPtr
SCADModels::f_const_float()
{
    auto model = std::make_unique<NodeModelType>("const_float", "Float", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_float_process);
    return model;
}
void
SCADModels::f_const_float_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("15.0");
}


SCADModels::RegistryItemPtr
SCADModels::f_const_string()
{
    auto model = std::make_unique<NodeModelType>("const_string", "String", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_string_process);
    return model;
}
void
SCADModels::f_const_string_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("\"constant string\"");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_undef()
{
    auto model = std::make_unique<NodeModelType>("const_undef", "Undefined", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_undef_process);
    return model;
}
void
SCADModels::f_const_undef_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("undef");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_pi()
{
    auto model = std::make_unique<NodeModelType>("PI", "PI", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_pi_process);
    return model;
}
void
SCADModels::f_const_pi_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("PI");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_version()
{
    auto model = std::make_unique<NodeModelType>("version", "Version", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_version_process);
    return model;
}
void
SCADModels::f_const_version_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("version()");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_version_num()
{
    auto model = std::make_unique<NodeModelType>("version_num", "Version Number", "Constants");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_pi_process);
    return model;
}
void
SCADModels::f_const_version_num_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("version_num()");
}

SCADModels::RegistryItemPtr
SCADModels::f_output()
{
    auto model = std::make_unique<NodeModelType>("output", "Output", "Output");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY), "out");
    model->setProcessor(f_output_process);
    return model;
}
void
SCADModels::f_output_process(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string s = FIND(input, "out", "//No Geometry Output\n");
    fprintf(stderr, "Output geometry %s\n", s.c_str());
    for (const auto & it : input) {
	fprintf(stderr, "Output map is %s -> %s\n", it.first.c_str(), it.second.c_str());
    }
    output["out"] = s;
}

#define UNARY_FUNCTION_BODY(prefix, name, description, input, atype, output, outtype, body) \
SCADModels::RegistryItemPtr                                                     \
SCADModels::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, "Math");   \
    model->addInputPort(std::make_unique<NodeModelPort>(atype), input);         \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), output);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
SCADModels::f_##prefix##_##name##_process(                                      \
    const NewSCADModel & model,                                                 \
    const PortFunctionData & _input,                                            \
    PortFunctionData & _output                                                  \
    )						                                \
{                                                                               \
    _output[output] = body;                                                     \
}

#define BINARY_FUNCTION_BODY(prefix, name, description, a, atype, b, btype, output, outtype, body) \
SCADModels::RegistryItemPtr                                                     \
SCADModels::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, "Math");   \
    model->addInputPort(std::make_unique<NodeModelPort>(atype), a);             \
    model->addInputPort(std::make_unique<NodeModelPort>(btype), b);             \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), output);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
SCADModels::f_##prefix##_##name##_process(                                      \
    const NewSCADModel & model,                                                 \
    const PortFunctionData & _input,                                            \
    PortFunctionData & _output                                                  \
    )						                                \
{                                                                               \
    _output[output] = body;                                                     \
}

UNARY_FUNCTION_BODY(math, asin, "Arc Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("asin(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sin, "Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sin(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, acos, "Arc Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("acos(") + FIND(_input, "x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, cos, "Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("cos(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, abs, "Absolute Value", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("abs(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, atan, "Arc Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("atan(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, tan, "Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("tan(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sign, "Sign", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sign(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ceil, "Ceiling", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ceil(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, floor, "Floor", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("floor(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, round, "Round", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("round(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ln, "Natural Log", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ln(") + FIND(_input, "x", "2.718") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, log, "Log Base 10", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("log(") + FIND(_input, "x", "10") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, exp, "Exponential(e)", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("exp(") + FIND(_input, "x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sqrt, "Square Root", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sqrt(") + FIND(_input, "x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, norm, "Norm", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("norm(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, len, "Length", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("len(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_bool, "Is Boolean", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_bool(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_string, "Is String", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_string(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_num, "Is Number", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_num(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_function, "Is Function", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_function") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_list, "Is List", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_list") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_undef, "Is Undefined", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_undef") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, str, "To String", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("str(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, chr, "To Char", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("chr(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ord, "To Ordinal", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ord(") + FIND(_input, "x", "true") + std::string(")"))
    )


BINARY_FUNCTION_BODY(math, max, "Maximum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("max(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, pow, "Power", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("pow(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, min, "Minimum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("min(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, concat, "Concatenate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("concat(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, atan2, "Arc Tan(2)", "x", DATA_VARIABLE, "y", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("atan2(") + FIND(_input, "x", "0") + "," + FIND(_input, "y", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, cross, "Cross Product", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("cross(") + FIND(_input, "a", "[1,0,0]") + "," + FIND(_input, "b", "[0,1,0]") + std::string(")"))
    )

BINARY_FUNCTION_BODY(math, add, "Add", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "+" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, subtract, "Subtract", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "-" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, multiply, "Multiply", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "*" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, divide, "Divide", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "/" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, modulo, "Modulo", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "%" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, exponentiate, "Exponentiate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "^" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, lt, "Less Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "<" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, leq, "Less Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "<=" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, eq, "Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "==" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, geq, "Greater Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + ">=" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, gt, "Greater Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + ">" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, and, "And", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "&&" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, or, "Or", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "||" + FIND(_input, "b", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, not, "Greater Than", "a", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(!") + FIND(_input, "a", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, lookup, "Lookup", "value", DATA_VARIABLE, "table", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("lookup(") + FIND(_input, "value", "0") + ", " + FIND(_input, "table", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, search, "Search for value", "needle", DATA_VARIABLE, "haystack", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("search(") + FIND(_input, "needle", "0") + ", " + FIND(_input, "haystack", "[]") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, parent_module, "Parent Module Name", "index", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("parent_module(") + FIND(_input, "needle", "0") + std::string(")"))
    )

std::shared_ptr<NodeProgramModelRegistry>
SCADModels::registerDataModels()
{
    auto ret = std::make_shared<NodeProgramModelRegistry>();
    
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_prim_sphere()));
    
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_prim_sphere()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_prim_cube()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_prim_cylinder()));
    
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_op_union()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_op_difference()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_op_intersection()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_op_hull()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_op_minkowski()));

    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_util_color()));
    
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_true()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_false()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_int()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_float()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_string()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_undef()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_pi()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_version()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_const_version_num()));

    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_output()));

    // Flow control
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_flow_for()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_flow_if()));
    //ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_flow_let);
    //ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_flow_group);

    // Transformations
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_xform_translate()));
    //ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_xform_offset()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_xform_scale()));
    //ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_xform_rotate()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_xform_mirror()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_xform_resize()));
    //ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_xform_multmatrix()));

    // Math functions:
    
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_asin()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_sin()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_acos()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_abs()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_atan()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_atan2()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_cos()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_tan()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_sign()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_ceil()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_floor()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_round()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_ln()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_len()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_log()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_exp()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_sqrt()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_concat()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_min()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_pow()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_max()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_norm()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_str()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_chr()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_ord()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_cross()));

    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_add()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_subtract()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_multiply()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_divide()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_modulo()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_exponentiate()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_lt()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_leq()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_eq()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_geq()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_gt()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_and()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_or()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_not()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_lookup()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_search()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_parent_module()));
    
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_is_bool()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_is_string()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_is_num()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_is_function()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_is_list()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_is_undef()));

    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_math_rands()));

    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_import_dxf_dim()));
    ret->registerModel(std::make_unique<OpenSCADNodeFactory>(f_import_dxf_cross()));
    
    return ret;
}

OpenSCADNodeFactory::OpenSCADNodeFactory(
    std::unique_ptr<NodeModelType> type
    )
    : _type(std::move(type))
{}

std::string
OpenSCADNodeFactory::getName() const
{
    return _type->getName();
}

std::string
OpenSCADNodeFactory::getCategory() const
{
    return _type->getCategory();
}

std::unique_ptr<QtNodes::NodeDelegateModel>
OpenSCADNodeFactory::create(void) const
{
    return std::make_unique<NewSCADModel>(*_type);
}

NewSCADModel::NewSCADModel(const NodeModelType & modelType)
    : _modelType(modelType)
    , _widget(nullptr)
    , _editor(nullptr)
{}

QString
NewSCADModel::name() const
{ return QString::fromStdString(_modelType.getName()); }

QString
NewSCADModel::caption() const
{ return QString::fromStdString(_modelType.getCaption()); }


QWidget *
NewSCADModel::embeddedWidget()
{
    if (!_widget) {
	_widget = _modelType.getWidgetFactory()(this);
    }
    return _widget;
}

unsigned int NewSCADModel::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPortCount();
    }
    else {
	return _modelType.getOutputPortCount();
    }
}

QtNodes::NodeDataType
NewSCADModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).nodeDataType();
    }
    else {
	return _modelType.getOutputPort(portIndex).nodeDataType();
    }
}

QString
NewSCADModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).portCaption();
    }
    else {
	return _modelType.getOutputPort(portIndex).portCaption();
    }
}

bool
NewSCADModel::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).portCaptionVisible();
    }
    else {
	return _modelType.getOutputPort(portIndex).portCaptionVisible();
    }
}

std::shared_ptr<QtNodes::NodeData> NewSCADModel::outData(QtNodes::PortIndex portIndex)
{
    /* Do nothing, we don't let the UI perform the evaluation */
    return nullptr;
}

void NewSCADModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex)
{
    if (!data) {
        Q_EMIT dataInvalidated(0);
    }

    /* Do nothing, we don't let the UI perform the evaluation */
}


std::string
NewSCADModel::inputPortName(QtNodes::PortIndex portIndex) const
{
    return _modelType.getInputPortName(portIndex);
}
std::string
NewSCADModel::outputPortName(QtNodes::PortIndex portIndex) const
{
    return _modelType.getOutputPortName(portIndex);
}

void
NewSCADModel::process(const PortFunctionData & input, PortFunctionData & output) const
{
    _modelType.getProcessor()(*this, input, output);
}

void
NewSCADModel::setEditor(JNodeProgramEditor *editor)
{
    _editor = editor;
}

JNodeProgramEditor *
NewSCADModel::getEditor() const
{
    return _editor;
}

void
NewSCADModel::editGraph()
{
    if (_editor) {
	_editor->editGraph("second-flow");
    }
}

