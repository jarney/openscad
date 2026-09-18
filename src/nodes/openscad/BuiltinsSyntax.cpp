#include "nodes/OpenSCADBuiltinFactory.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#include <QtWidgets/QLineEdit>

using namespace JNodes::openscad;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_SYNTAX.getName()

////////////////////////////////////////
// Assignment
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_syntax_assign()
{
    auto model = std::make_unique<NodeModelType>("assign", "Assign", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "value"), "value");
    model->setProcessor(f_syntax_assign_process);
    model->setInitializer(f_syntax_assign_initializer);
    model->setWidgetFactory(f_syntax_assign_widget);
    return model;
}
void
Builtins::f_syntax_assign_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
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
Builtins::f_syntax_assign_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("variable_name")) {
	node.setValue("variable_name", "x");
    }
}

QWidget*
Builtins::f_syntax_assign_widget(OpenSCADBuiltinModel & node)
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
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_assign_list_process);
    return model;
}

void
Builtins::f_syntax_assign_list_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
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
    auto model = std::make_unique<NodeModelType>("variable", "Variable", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "variable"), "variable");
    model->setProcessor(f_syntax_variable_process);
    model->setInitializer(f_syntax_variable_initializer);
    model->setWidgetFactory(f_syntax_variable_widget);
    return model;
}

void
Builtins::f_syntax_variable_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = node.getValue("variable_name");
    output.setValue("variable", out);
}

void
Builtins::f_syntax_variable_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("variable_name")) {
	node.setValue("variable_name", "x");
    }
}

QWidget*
Builtins::f_syntax_variable_widget(OpenSCADBuiltinModel & node)
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
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_module_process);
    return model;
}

void
Builtins::f_syntax_module_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
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
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_function_process);
    return model;
}

void
Builtins::f_syntax_function_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
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
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_include_process);
    return model;
}

void
Builtins::f_syntax_include_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
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
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_use_process);
    return model;
}

void
Builtins::f_syntax_use_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "r");
    conditionalArg(args, input, node, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}
