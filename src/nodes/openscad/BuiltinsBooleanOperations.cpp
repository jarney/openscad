#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_BOOLEAN_OPS.getName()

////////////////////////////////////////
// Union
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_op_union()
{
    auto nodeType = std::make_unique<NodeType>("union", "Union", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_op_union_process);
    return nodeType;
}
void
Builtins::f_op_union_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("union() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Difference
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_op_difference()
{
    auto nodeType = std::make_unique<NodeType>("difference", "Difference", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::One);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "b"), "b", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_op_difference_process);
    return nodeType;
}
void
Builtins::f_op_difference_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("difference() {\n");
    out += std::string("    {\n");
    out +=     input.getValue("a", "{}");
    out += std::string("    }");
    out += std::string("    {\n");
    out +=     input.getValue("b", "{}");
    out += std::string("    }");
    out += std::string("}");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Intersection
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_op_intersection()
{
    auto nodeType = std::make_unique<NodeType>("intersection", "Intersection", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_op_intersection_process);
    return nodeType;
}
void
Builtins::f_op_intersection_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out;
    out += std::string("intersection() {\n");
    out +=     input.getValue("Geometry", "{}");
    out += std::string("}");
    output.setValue("Geometry", out);
}


