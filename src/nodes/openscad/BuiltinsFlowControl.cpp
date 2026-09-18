#include "nodes/openscad/Builtins.hpp"
#include "nodes/openscad/Builtins_helpers.hpp"

#include "nodes/openscad/NodeProgramSerializerOpenSCAD.hpp"
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
    auto nodeType = std::make_unique<NodeType>("for", "For Loop", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "range"), "range");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "start"), "start");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "end"), "end");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "increment"), "increment");
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_flow_for_process);
    nodeType->setInitializer(f_flow_for_initializer);
    nodeType->setWidgetFactory(f_flow_for_widget);
    return nodeType;
}

void
Builtins::f_flow_for_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    const NodeGraph *subgraph =
	node.getGraph().getParent().getGraph(bodyGraphId);
    out += NodeProgramSerializerOpenSCAD::toString(*subgraph);
    
    out += std::string("}");
    output.setValue("Geometry", out);
}

void
Builtins::f_flow_for_initializer(Node & node)
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
Builtins::f_flow_for_widget(Node & node)
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
    auto nodeType = std::make_unique<NodeType>("intersection_for", "Intersection For", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "var"), "var");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_flow_intersection_for_process);
    return nodeType;
}

void
Builtins::f_flow_intersection_for_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    auto nodeType = std::make_unique<NodeType>("if", "If", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "condition"), "condition");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "b"), "b", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_flow_if_process);
    return nodeType;
}

void
Builtins::f_flow_if_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    auto nodeType = std::make_unique<NodeType>("let", "Let", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_VARIABLE, "var"), "var");
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_flow_let_process);
    return nodeType;
}

void
Builtins::f_flow_let_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    auto nodeType = std::make_unique<NodeType>("comment", "Comment", _OPENSCAD_NODE_CATEGORY);
    nodeType->setResizable(true);
    nodeType->setWidgetFactory(Builtins::f_flow_comment_widget);
    return nodeType;
}
QWidget*
Builtins::f_flow_comment_widget(Node & node)
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
    auto nodeType = std::make_unique<NodeType>("group", "Group", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    nodeType->addOutputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    nodeType->setProcessor(f_flow_group_process);
    return nodeType;
}
void
Builtins::f_flow_group_process(const Node & node, const NodePortData & input, NodePortData & output)
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
    auto nodeType = std::make_unique<NodeType>("output", "Output", _OPENSCAD_NODE_CATEGORY);
    nodeType->addInputPort(std::make_unique<NodePort>(DATA_SOLID_GEOMETRY), "out", QtNodes::ConnectionPolicy::Many);
    nodeType->setProcessor(f_flow_output_process);
    return nodeType;
}
void
Builtins::f_flow_output_process(const Node & node, const NodePortData & input, NodePortData & output)
{
    std::string s = input.getValue("out", "//No Geometry Output\n");
    output.setValue("out", s);
}



