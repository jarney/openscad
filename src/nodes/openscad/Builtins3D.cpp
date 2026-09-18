#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_3D.getName()

////////////////////////////////////////
// Sphere
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_sphere()
{
    auto nodeType = std::make_unique<NodeType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_sphere_process);
    return nodeType;
}

void
Builtins::f_3d_sphere_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
    
}

////////////////////////////////////////
// Cube
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_cube()
{
    auto nodeType = std::make_unique<NodeType>("cube", "Cube", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "size"), "size");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "center"), "center");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_cube_process);
    return nodeType;
}

void
Builtins::f_3d_cube_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "size");
    conditionalArg(args, input, node, "center");
    std::string out = std::string("cube(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Cylinder
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_cylinder()
{
    auto nodeType = std::make_unique<NodeType>("cylinder", "Cylinder", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "h"), "h");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r1"), "r1");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r2"), "r2");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d1"), "d1");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d2"), "d2");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "center"), "center");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_cylinder_process);
    return nodeType;
}
void
Builtins::f_3d_cylinder_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "h");
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "r1");
    conditionalArg(args, input, node, "r2");
    conditionalArg(args, input, node, "d");
    conditionalArg(args, input, node, "d1");
    conditionalArg(args, input, node, "d2");
    conditionalArg(args, input, node, "center");
    std::string out = std::string("cylinder(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Polyhedron
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_polyhedron()
{
    auto nodeType = std::make_unique<NodeType>("polyhedron", "Polyhedron", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "points"), "points");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "faces"), "faces");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_polyhedron_process);
    return nodeType;
}
void
Builtins::f_3d_polyhedron_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "points");
    conditionalArg(args, input, node, "faces");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("polyhedron(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_import()
{
    auto nodeType = std::make_unique<NodeType>("import", "Import", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "file"), "file");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "center"), "center");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "id"), "id");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "layer"), "layer");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "$fn"), "$fn");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "$fa"), "$fa");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "$fs"), "$fs");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_import_process);
    return nodeType;
}
void
Builtins::f_3d_import_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "convexity");
    conditionalArg(args, input, node, "id");
    conditionalArg(args, input, node, "layer");
    conditionalArg(args, input, node, "$fn");
    conditionalArg(args, input, node, "$fa");
    conditionalArg(args, input, node, "$fs");
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Linear Extrude
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_linear_extrude()
{
    auto nodeType = std::make_unique<NodeType>("linear_extrude", "Linear Extrude", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "height"), "height");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "v"), "v");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "center"), "center");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "twist"), "twist");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "scale"), "scale");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "slices"), "slices");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "segments"), "segments");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "$fn"), "$fn");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_linear_extrude_process);
    return nodeType;
}
void
Builtins::f_3d_linear_extrude_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "height");
    conditionalArg(args, input, node, "v");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "twist");
    conditionalArg(args, input, node, "scale");
    conditionalArg(args, input, node, "slices");
    conditionalArg(args, input, node, "segments");
    conditionalArg(args, input, node, "convexity");
    conditionalArg(args, input, node, "$fn");
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}


////////////////////////////////////////
// Rotate Extrude
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_rotate_extrude()
{
    auto nodeType = std::make_unique<NodeType>("rotate_extrude", "Rotate Extrude", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "angle"), "angle");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "start"), "start");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "a"), "a");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_rotate_extrude_process);
    return nodeType;
}
void
Builtins::f_3d_rotate_extrude_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "angle");
    conditionalArg(args, input, node, "start");
    conditionalArg(args, input, node, "convexity");
    conditionalArg(args, input, node, "a");
    std::string out = std::string("import(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}
////////////////////////////////////////
// Surface
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_surface()
{
    auto nodeType = std::make_unique<NodeType>("surface", "Heightmap Surface", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "file"), "file");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "center"), "center");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "invert"), "invert");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "convexity"), "convexity");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_3d_surface_process);
    return nodeType;
}
void
Builtins::f_3d_surface_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "center");
    conditionalArg(args, input, node, "invert");
    conditionalArg(args, input, node, "convexity");
    std::string out = std::string("surface(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import DXF Dimensions
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_dxf_dim()
{
    auto nodeType = std::make_unique<NodeType>("dxf_dim", "Read DXF Dimension", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "file"), "file");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "name"), "name");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "layer"), "layer");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "origin"), "origin");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "scale"), "scale");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "out"), "out");
    nodeType->setProcessor(f_3d_dxf_dim_process);
    return nodeType;
}

void
Builtins::f_3d_dxf_dim_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "name");
    conditionalArg(args, input, node, "layer");
    conditionalArg(args, input, node, "origin");
    conditionalArg(args, input, node, "scale");
    std::string out = std::string("dxf_dim(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Import DXF Cross (Origin)
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_3d_dxf_cross()
{
    auto nodeType = std::make_unique<NodeType>("dxf_cross", "Read DXF Origin", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "file"), "file");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "layer"), "layer");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "origin"), "origin");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "scale"), "scale");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "out"), "out");
    nodeType->setProcessor(f_3d_dxf_cross_process);
    return nodeType;
}

void
Builtins::f_3d_dxf_cross_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "file");
    conditionalArg(args, input, node, "layer");
    conditionalArg(args, input, node, "origin");
    conditionalArg(args, input, node, "scale");
    std::string out = std::string("dxf_cross(") + joinArguments(args) + std::string(");");
    output.setValue("Geometry", out);
}

