#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

/*
 * This contains the OpensCAD 3d primitives.  This is laid out roughly
 * the way the "OpenSCAD Cheat Sheet" is.
 */
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_prim_sphere()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", "Primitives");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_sphere_process);
    return model;
}

void
OpenSCADBuiltins::f_prim_sphere_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("sphere(");
    if (input.hasValue("r")) {
	out += std::string("r=") + input.getValue("r", "15.0");
    }
    else if (input.hasValue("d")) {
	out += std::string("d=") + input.getValue("d", "30.0");
    }
    out += std::string(");\n");
    output.setValue("Geometry", out);
    
}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_prim_cube()
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
OpenSCADBuiltins::f_prim_cube_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("        cube([") +
	input.getValue("x", "15.0") + std::string(", ") +
	input.getValue("y", "15.0") + std::string(", ") +
        input.getValue("z", "15.0") +
        std::string("]));\n"));
}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_prim_cylinder()
{
    auto model = std::make_unique<NodeModelType>("cylinder", "Cylinder", "Primitives");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "h"), "h");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_cylinder_process);
    return model;
}
void
OpenSCADBuiltins::f_prim_cylinder_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("cylinder(") +
	input.getValue("r", "15") + std::string(", ") +
	input.getValue("h", "15") +
        std::string("));"));
}
// polyhedron
// import
// linear_extrude
// rotate_extrude
// surface

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_import_dxf_dim()
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
OpenSCADBuiltins::f_import_dxf_dim_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("out", std::string("dxf_dim(") +
	std::string("file=") + input.getValue("file", "0") + "," +
	std::string("name=") + input.getValue("name", "0") + "," +
	std::string("layer=") + input.getValue("layer", "0") + "," +
	std::string("origin=") + input.getValue("origin", "0") + "," +
	std::string("scale=") + input.getValue("scale", "0") +
        std::string(")"));

}

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_import_dxf_cross()
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
OpenSCADBuiltins::f_import_dxf_cross_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("out", std::string("dxf_cross(") +
	std::string("file=") + input.getValue("file", "0") + "," +
	std::string("layer=") + input.getValue("layer", "0") + "," +
	std::string("origin=") + input.getValue("origin", "0") + "," +
	std::string("scale=") + input.getValue("scale", "0") +
		    std::string(")"));
}

