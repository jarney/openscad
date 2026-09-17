#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#include <QtWidgets/QLineEdit>
#include <QIntValidator>
#include <QDoubleValidator>

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_CONST.getName()

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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_int()
{
    auto model = std::make_unique<NodeModelType>("const_int", "Integer", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_int_process);
    model->setInitializer(f_const_int_initializer);
    model->setWidgetFactory(f_const_int_widget);
    return model;
}
void
OpenSCADBuiltins::f_const_int_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", node.getValue("value", "0"));
}
void
OpenSCADBuiltins::f_const_int_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "0");
    }
}
QWidget*
OpenSCADBuiltins::f_const_int_widget(OpenSCADBuiltinModel & node)
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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_float()
{
    auto model = std::make_unique<NodeModelType>("const_float", "Float", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_float_process);
    model->setInitializer(f_const_float_initializer);
    model->setWidgetFactory(f_const_float_widget);
    return model;
}
void
OpenSCADBuiltins::f_const_float_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("value", node.getValue("value", "0.0"));
}
void
OpenSCADBuiltins::f_const_float_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "0.0");
    }
}
QWidget*
OpenSCADBuiltins::f_const_float_widget(OpenSCADBuiltinModel & node)
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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_const_string()
{
    auto model = std::make_unique<NodeModelType>("const_string", "String", _OPENSCAD_NODE_CATEGORY);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_string_process);
    model->setInitializer(f_const_string_initializer);
    model->setWidgetFactory(f_const_string_widget);
    return model;
}
void
OpenSCADBuiltins::f_const_string_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue(
	"value",
	std::string("\"") +
	node.getValue("value", "") + 
	std::string("\"")
	);
}

void
OpenSCADBuiltins::f_const_string_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("value")) {
	node.setValue("value", "");
    }
}

QWidget*
OpenSCADBuiltins::f_const_string_widget(OpenSCADBuiltinModel & node)
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

