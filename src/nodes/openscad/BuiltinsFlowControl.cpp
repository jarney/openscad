#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#include "nodes/OpenSCADEvaluator.hpp"
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QLineEdit>
#include <QRegularExpressionValidator>
#include <Qsci/qsciscintilla.h>

using namespace JNodes::openscad;
using namespace JNodes::core;

#define _OPENSCAD_NODE_CATEGORY Builtins::CATEGORY_FLOW.getName()

////////////////////////////////////////
// For
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_flow_for()
{
    auto model = std::make_unique<NodeModelType>("for", "For Loop", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "range"), "range");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "start"), "start");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "end"), "end");
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "increment"), "increment");
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_for_process);
    model->setInitializer(f_flow_for_initializer);
    model->setWidgetFactory(f_flow_for_widget);
    return model;
}

void
Builtins::f_flow_for_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("for (");
    out += std::string(node.getValue("variable"));
    out += std::string(" = ");
    if (input.hasValue("range")) {
	out += input.getValue("range");
    }
    else {
	out += std::string("[");
	out += input.getValue("start", "0");
	out += std::string(":");
	out += input.getValue("end", "0");
	if (input.hasValue("increment")) {
	    out += std::string(":");
	    out += input.getValue("increment");
	}
	out += std::string("]");
    }
    out += std::string(") {");

    std::string bodyGraphId = node.getValue("graph");
    const NodeProgramGraphModel *subgraph =
	node.getGraph().getParent().getGraph(bodyGraphId);
    out += evaluateToSCAD(*subgraph);
    
    out += std::string("}");
    output.setValue("Geometry", out);
}

void
Builtins::f_flow_for_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("graph")) {
	NodeProgram::GraphId graphId =
	    node.getGraph().getParent().newGraphWithPrefix("for");
	node.setValue("graph", graphId);
    }
    if (!node.hasValue("variable")) {
	node.setValue("variable", "i");
    }
}

QWidget*
Builtins::f_flow_for_widget(OpenSCADBuiltinModel & node)
{
    QWidget *w = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(w);
    
    // We need a name for the loop iteration variable
    QLineEdit *textEdit = new QLineEdit();
    textEdit->setText(QString::fromStdString(node.getValue("variable")));
    QObject::connect(textEdit, &QLineEdit::textChanged, [&node, textEdit]() {
	node.setValue("variable", textEdit->text().toStdString());
    });
    
    layout->addWidget(textEdit);
    
    // We need an edit button to get to the body of the loop.
    QPushButton *button = new QPushButton();
    button->setText("Edit Body");
    QObject::connect(button, &QPushButton::clicked, [&node]() {
	// If we already have a graph, use it.
	NodeProgram::GraphId graphId = node.getValue("graph");
	node.editGraph(graphId);
    });
    layout->addWidget(button);
    
    return w;
}

////////////////////////////////////////
// Intersection For
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_flow_intersection_for()
{
    auto model = std::make_unique<NodeModelType>("intersection_for", "Intersection For", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "var"), "var");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_intersection_for_process);
    return model;
}

void
Builtins::f_flow_intersection_for_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    // TODO: Call/evaluate sub-flow
    std::string out = std::string();
    out += std::string("intersection_for(") + std::string("var = ") + input.getValue("var", "[1:10]") + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// If
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_flow_if()
{
    auto model = std::make_unique<NodeModelType>("if", "If", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "condition"), "condition");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "b"), "b", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_if_process);
    return model;
}

void
Builtins::f_flow_if_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("if(");
    out += input.getValue("condition", "true");
    out += std::string(") {\n");
    out += input.getValue("a", "");
    out += std::string("} else {\n");
    out += input.getValue("b", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Let
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_flow_let()
{
    auto model = std::make_unique<NodeModelType>("let", "Let", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "var"), "var");
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_let_process);
    return model;
}

void
Builtins::f_flow_let_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    // TODO: Call/evaluate sub-flow
    std::string out = std::string();
    out += std::string("let(") + std::string("var = ") + input.getValue("var", "[1:10]") + std::string(") {\n");
    out += input.getValue("Geometry", "");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Comment
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_flow_comment()
{
    auto model = std::make_unique<NodeModelType>("comment", "Comment", _OPENSCAD_NODE_CATEGORY);
    model->setResizable(true);
    model->setWidgetFactory(Builtins::f_flow_comment_widget);
    return model;
}
QWidget*
Builtins::f_flow_comment_widget(OpenSCADBuiltinModel & node)
{
    // Should we have rich text and markups or perhaps IDE-style
    // input boxes or is plain text enough?
    //    QsciScintilla *comment = new QsciScintilla();
    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    
    QPlainTextEdit *comment = new QPlainTextEdit();
    comment->setPlainText(QString::fromStdString(node.getValue("comment", "Place Comment Here...")));
    layout->addWidget(comment);
    
    QObject::connect(comment, &QPlainTextEdit::textChanged, [comment, &node]() {
	node.setValue("comment", comment->toPlainText().toStdString());
    });

    return widget;
}

////////////////////////////////////////
// Group
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_flow_group()
{
    auto model = std::make_unique<NodeModelType>("group", "Group", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_group_process);
    return model;
}
void
Builtins::f_flow_group_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("group() {\n") +
        input.getValue("a", "{}") +
        std::string("});"));
}

////////////////////////////////////////
// Output
////////////////////////////////////////
Builtins::RegistryItemPtr
Builtins::f_flow_output()
{
    auto model = std::make_unique<NodeModelType>("output", "Output", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY), "out", QtNodes::ConnectionPolicy::Many);
    model->setProcessor(f_flow_output_process);
    return model;
}
void
Builtins::f_flow_output_process(const OpenSCADBuiltinModel & node, const PortFunctionData & input, PortFunctionData & output)
{
    std::string s = input.getValue("out", "//No Geometry Output\n");
    output.setValue("out", s);
}



