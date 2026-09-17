#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#include <QtWidgets/QLineEdit>

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_SYNTAX.getName()

////////////////////////////////////////
// Assignment
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_syntax_assign()
{
    auto model = std::make_unique<NodeModelType>("assign", "Assign", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "value"), "value");
    model->setProcessor(f_syntax_assign_process);
    model->setInitializer(f_syntax_assign_initializer);
    model->setWidgetFactory(f_syntax_assign_widget);
    return model;
}
void
OpenSCADBuiltins::f_syntax_assign_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, node, "value");
    std::string out = node.getValue("variable_name", "x") + std::string("= (") + joinArguments(args) + std::string(");");
    output.setValue("out", out);
}

void
OpenSCADBuiltins::f_syntax_assign_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("variable_name")) {
	node.setValue("variable_name", "x");
    }
}

QWidget*
OpenSCADBuiltins::f_syntax_assign_widget(OpenSCADBuiltinModel & node)
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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_syntax_assign_list()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_assign_list_process);
    return model;
}

void
OpenSCADBuiltins::f_syntax_assign_list_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Variable
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_syntax_variable()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_variable_process);
    return model;
}

void
OpenSCADBuiltins::f_syntax_variable_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Define Module
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_syntax_module()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_module_process);
    return model;
}

void
OpenSCADBuiltins::f_syntax_module_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Define Function
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_syntax_function()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_function_process);
    return model;
}

void
OpenSCADBuiltins::f_syntax_function_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Include
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_syntax_include()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_include_process);
    return model;
}

void
OpenSCADBuiltins::f_syntax_include_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Use
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_syntax_use()
{
    auto model = std::make_unique<NodeModelType>("sphere", "Sphere", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_syntax_use_process);
    return model;
}

void
OpenSCADBuiltins::f_syntax_use_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::vector<std::string> args;
    conditionalArg(args, input, model, "r");
    conditionalArg(args, input, model, "d");
    std::string out = std::string("sphere(") + joinArguments(args) + std::string(")");
    output.setValue("Geometry", out);
}
