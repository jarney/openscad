#include "nodes/OpenSCADBuiltinFactory.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_3D.getName()

////////////////////////////////////////
// Sphere
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_sphere()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_sphere_process);
    return model;
}

void
OpenSCADBuiltins::f_3d_sphere_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Cube
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_cube()
{
    auto model = std::make_unique<NodeModelType>("cube", "Cube", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "size"), "size");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "center"), "center");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_cube_process);
    return model;
}

void
OpenSCADBuiltins::f_3d_cube_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "size");
    conditionalArg(args, input, model, "center");
    std::string out = std::string("cube(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Cylinder
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_cylinder()
{
    auto model = std::make_unique<NodeModelType>("cylinder", "Cylinder", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "h"), "h");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r1"), "r1");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r2"), "r2");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d1"), "d1");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d2"), "d2");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "center"), "center");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_cylinder_process);
    return model;
}
void
OpenSCADBuiltins::f_3d_cylinder_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "h");
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "r1");
    conditionalArg(args, input, model, "r2");
    conditionalArg(args, input, model, "d");
    conditionalArg(args, input, model, "d1");
    conditionalArg(args, input, model, "d2");
    conditionalArg(args, input, model, "center");
    std::string out = std::string("cylinder(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Polyhedron
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_polyhedron()
{
    auto model = std::make_unique<NodeModelType>("polyhedron", "Polyhedron", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "points"), "points");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "faces"), "faces");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_polyhedron_process);
    return model;
}
void
OpenSCADBuiltins::f_3d_polyhedron_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "points");
    conditionalArg(args, input, model, "faces");
    conditionalArg(args, input, model, "convexity");
    std::string out = std::string("polyhedron(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_import()
{
    auto model = std::make_unique<NodeModelType>("import", "Import", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "center"), "center");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "id"), "id");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "layer"), "layer");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "$fn"), "$fn");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "$fa"), "$fa");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "$fs"), "$fs");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_import_process);
    return model;
}
void
OpenSCADBuiltins::f_3d_import_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "file");
    conditionalArg(args, input, model, "center");
    conditionalArg(args, input, model, "convexity");
    conditionalArg(args, input, model, "id");
    conditionalArg(args, input, model, "layer");
    conditionalArg(args, input, model, "$fn");
    conditionalArg(args, input, model, "$fa");
    conditionalArg(args, input, model, "$fs");
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Linear Extrude
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_linear_extrude()
{
    auto model = std::make_unique<NodeModelType>("linear_extrude", "Linear Extrude", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "height"), "height");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "v"), "v");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "center"), "center");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "twist"), "twist");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "scale"), "scale");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "slices"), "slices");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "segments"), "segments");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "$fn"), "$fn");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_linear_extrude_process);
    return model;
}
void
OpenSCADBuiltins::f_3d_linear_extrude_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "height");
    conditionalArg(args, input, model, "v");
    conditionalArg(args, input, model, "center");
    conditionalArg(args, input, model, "twist");
    conditionalArg(args, input, model, "scale");
    conditionalArg(args, input, model, "slices");
    conditionalArg(args, input, model, "segments");
    conditionalArg(args, input, model, "convexity");
    conditionalArg(args, input, model, "$fn");
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}


////////////////////////////////////////
// Rotate Extrude
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_rotate_extrude()
{
    auto model = std::make_unique<NodeModelType>("rotate_extrude", "Rotate Extrude", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "angle"), "angle");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "start"), "start");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "a"), "a");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_rotate_extrude_process);
    return model;
}
void
OpenSCADBuiltins::f_3d_rotate_extrude_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "angle");
    conditionalArg(args, input, model, "start");
    conditionalArg(args, input, model, "convexity");
    conditionalArg(args, input, model, "a");
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}
////////////////////////////////////////
// Surface
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_surface()
{
    auto model = std::make_unique<NodeModelType>("surface", "Heightmap Surface", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "center"), "center");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "invert"), "invert");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "convexity"), "convexity");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_3d_surface_process);
    return model;
}
void
OpenSCADBuiltins::f_3d_surface_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "file");
    conditionalArg(args, input, model, "center");
    conditionalArg(args, input, model, "invert");
    conditionalArg(args, input, model, "convexity");
    std::string out = std::string("surface(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import DXF Dimensions
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_dxf_dim()
{
    auto model = std::make_unique<NodeModelType>("dxf_dim", "Read DXF Dimension", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "name"), "name");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "layer"), "layer");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "origin"), "origin");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "scale"), "scale");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "out"), "out");
    model->setProcessor(f_3d_dxf_dim_process);
    return model;
}

void
OpenSCADBuiltins::f_3d_dxf_dim_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "file");
    conditionalArg(args, input, model, "name");
    conditionalArg(args, input, model, "layer");
    conditionalArg(args, input, model, "origin");
    conditionalArg(args, input, model, "scale");
    std::string out = std::string("dxf_dim(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import DXF Cross (Origin)
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_3d_dxf_cross()
{
    auto model = std::make_unique<NodeModelType>("dxf_cross", "Read DXF Origin", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "layer"), "layer");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "origin"), "origin");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "scale"), "scale");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "out"), "out");
    model->setProcessor(f_3d_dxf_cross_process);
    return model;
}

void
OpenSCADBuiltins::f_3d_dxf_cross_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "file");
    conditionalArg(args, input, model, "layer");
    conditionalArg(args, input, model, "origin");
    conditionalArg(args, input, model, "scale");
    std::string out = std::string("dxf_cross(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

