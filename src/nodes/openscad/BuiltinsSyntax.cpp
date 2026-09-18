#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#include <QtWidgets/QLineEdit>

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_SYNTAX.getName()

////////////////////////////////////////
// Assignment
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_assign()
{
    auto nodeType = std::make_unique<NodeType>("assign", "Assign", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "value"), "value");
    nodeType->setProcessor(f_syntax_assign_process);
    nodeType->setInitializer(f_syntax_assign_initializer);
    nodeType->setWidgetFactory(f_syntax_assign_widget);
    return nodeType;
}
void
Builtins::f_syntax_assign_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string value;
    if (input.hasValue("value")) {
	value = input.getValue("value");
    }
    else {
	value = "undef";
    }
    std::string out = node.getValue("variable_name", "x") + std::string("= ") + value + std::string(";");
    output.setValue("out", out);
}

void
Builtins::f_syntax_assign_initializer(Node & node)
{
    if (!node.hasValue("variable_name")) {
	node.setValue("variable_name", "x");
    }
}

QWidget*
Builtins::f_syntax_assign_widget(Node & node)
{
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getValue("variable_name")));
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
	node.setValue("variable_name", textEdit->text().toStdString());
    });
    
    return textEdit;
}

////////////////////////////////////////
// Assign List
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_assign_list()
{
    auto nodeType = std::make_unique<NodeType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_syntax_assign_list_process);
    return nodeType;
}

void
Builtins::f_syntax_assign_list_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Variable
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_variable()
{
    auto nodeType = std::make_unique<NodeType>("variable", "Variable", _OPENSCAD_NODE_CATEGORY);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE, "variable"), "variable");
    nodeType->setProcessor(f_syntax_variable_process);
    nodeType->setInitializer(f_syntax_variable_initializer);
    nodeType->setWidgetFactory(f_syntax_variable_widget);
    return nodeType;
}

void
Builtins::f_syntax_variable_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string out = node.getValue("variable_name");
    output.setValue("variable", out);
}

void
Builtins::f_syntax_variable_initializer(Node & node)
{
    if (!node.hasValue("variable_name")) {
	node.setValue("variable_name", "x");
    }
}

QWidget*
Builtins::f_syntax_variable_widget(Node & node)
{
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getValue("variable_name")));
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
	node.setValue("variable_name", textEdit->text().toStdString());
    });
    
    return textEdit;
}
////////////////////////////////////////
// Define Module
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_module()
{
    auto nodeType = std::make_unique<NodeType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_syntax_module_process);
    return nodeType;
}

void
Builtins::f_syntax_module_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Define Function
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_function()
{
    auto nodeType = std::make_unique<NodeType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_syntax_function_process);
    return nodeType;
}

void
Builtins::f_syntax_function_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Include
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_include()
{
    auto nodeType = std::make_unique<NodeType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_syntax_include_process);
    return nodeType;
}

void
Builtins::f_syntax_include_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Use
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_use()
{
    auto nodeType = std::make_unique<NodeType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "r"), "r");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "d"), "d");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_syntax_use_process);
    return nodeType;
}

void
Builtins::f_syntax_use_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}
