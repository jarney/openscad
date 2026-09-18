#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#include <QtWidgets/QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_CONST.getName()

NONARY_NODE(const, true, _OPENSCAD_NODE_CATEGORY, "True", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("true"));
}
NONARY_NODE(const, false, _OPENSCAD_NODE_CATEGORY, "False", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("false"));
}
NONARY_NODE(const, undef, _OPENSCAD_NODE_CATEGORY, "Undefined", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("undef"));
}

////////////////////////////////////////
// Integer Constant
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_const_int()
{
    auto nodeType = std::make_unique<NodeType>("const_int", "Integer", _OPENSCAD_NODE_CATEGORY);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE), "value");
    nodeType->setProcessor(f_const_int_process);
    nodeType->setInitializer(f_const_int_initializer);
    nodeType->setWidgetFactory(f_const_int_widget);
    return nodeType;
}
void
Builtins::f_const_int_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("value", node.getValue("value", "0"));
}
void
Builtins::f_const_int_initializer(Node & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "0");
    }
}
QWidget*
Builtins::f_const_int_widget(Node & node)
{
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getValue("value")));
    QIntValidator *validator = new QIntValidator();
    textEdit->setValidator(validator);
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
	if (textEdit->hasAcceptableInput()) {
	    node.setValue("value", textEdit->text().toStdString());
	}
    });
    return textEdit;
}

////////////////////////////////////////
// Float Constant
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_const_float()
{
    auto nodeType = std::make_unique<NodeType>("const_float", "Float", _OPENSCAD_NODE_CATEGORY);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE), "value");
    nodeType->setProcessor(f_const_float_process);
    nodeType->setInitializer(f_const_float_initializer);
    nodeType->setWidgetFactory(f_const_float_widget);
    return nodeType;
}
void
Builtins::f_const_float_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue("value", node.getValue("value", "0.0"));
}
void
Builtins::f_const_float_initializer(Node & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "0.0");
    }
}
QWidget*
Builtins::f_const_float_widget(Node & node)
{
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getValue("value")));
    QDoubleValidator *validator = new QDoubleValidator();
    textEdit->setValidator(validator);
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
	if (textEdit->hasAcceptableInput()) {
	    node.setValue("value", textEdit->text().toStdString());
	}
    });
    
    return textEdit;
}


////////////////////////////////////////
// String Constant
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_const_string()
{
    auto nodeType = std::make_unique<NodeType>("const_string", "String", _OPENSCAD_NODE_CATEGORY);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_VARIABLE), "value");
    nodeType->setProcessor(f_const_string_process);
    nodeType->setInitializer(f_const_string_initializer);
    nodeType->setWidgetFactory(f_const_string_widget);
    return nodeType;
}
void
Builtins::f_const_string_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    output.setValue(
	"value",
	std::string("\"") +
	node.getValue("value", "") + 
	std::string("\"")
	);
}

void
Builtins::f_const_string_initializer(Node & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "");
    }
}

QWidget*
Builtins::f_const_string_widget(Node & node)
{
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getValue("value")));
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
	node.setValue("value", textEdit->text().toStdString());
    });
    
    return textEdit;
}

////////////////////////////////////////
// PI Constant
////////////////////////////////////////
NONARY_NODE(const, pi, _OPENSCAD_NODE_CATEGORY, "PI", "value", DATA_VARIABLE)
{
    output.setValue("value", std::string("PI"));
}

