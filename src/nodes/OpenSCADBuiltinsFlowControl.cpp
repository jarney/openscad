#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPlainTextEdit>
#include <Qsci/qsciscintilla.h>

#define _OPENSCAD_NODE_CATEGORY OpenSCADBuiltins::CATEGORY_FLOW.getName()

////////////////////////////////////////
// For
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_for()
{
    auto model = std::make_unique<NodeModelType>("for", "For Loop", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "start"), "start");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "end"), "end");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_for_process);
    model->setInitializer(f_flow_for_initializer);
    model->setWidgetFactory(f_flow_for_widget);
    return model;
}

void
OpenSCADBuiltins::f_flow_for_initializer(OpenSCADBuiltinModel & node)
{
    if (!node.hasValue("graph")) {
	NodeProgram::GraphId graphId =
	    node.getGraph().getParent().newGraphWithPrefix("for");
	node.setValue("graph", graphId);
    }
}

QWidget*
OpenSCADBuiltins::f_flow_for_widget(OpenSCADBuiltinModel & node)
{
    QPushButton *button = new QPushButton();
    button->setText("Edit");
    QObject::connect(button, &QPushButton::clicked, [&node]() {
	// If we already have a graph, use it.
	NodeProgram::GraphId graphId = node.getValue("graph");
	node.editGraph(graphId);
    });
    
    return button;
}

void
OpenSCADBuiltins::f_flow_for_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("for (i = ");
    out += input.getValue("start", "0");
    out += std::string(";");
    out += std::string("i < ");
    out += input.getValue("end", "10");
    out += std::string("; i++) {");
    out += std::string("// the sub-flow goes here...");
    out += std::string("}\n");
    output.setValue("Geometry", out);
}

////////////////////////////////////////
// Intersection For
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_intersection_for()
{
    auto model = std::make_unique<NodeModelType>("intersection_for", "Intersection For", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "var"), "var");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_intersection_for_process);
    return model;
}

void
OpenSCADBuiltins::f_flow_intersection_for_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_if()
{
    auto model = std::make_unique<NodeModelType>("if", "If", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "condition"), "condition");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_if_process);
    return model;
}

void
OpenSCADBuiltins::f_flow_if_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_let()
{
    auto model = std::make_unique<NodeModelType>("let", "Let", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "var"), "var");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_let_process);
    return model;
}

void
OpenSCADBuiltins::f_flow_let_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_comment()
{
    auto model = std::make_unique<NodeModelType>("comment", "Comment", _OPENSCAD_NODE_CATEGORY);
    model->setResizable(true);
    model->setWidgetFactory(OpenSCADBuiltins::f_flow_comment_widget);
    return model;
}
QWidget*
OpenSCADBuiltins::f_flow_comment_widget(OpenSCADBuiltinModel & node)
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
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_group()
{
    auto model = std::make_unique<NodeModelType>("group", "Group", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a", QtNodes::ConnectionPolicy::Many);
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_group_process);
    return model;
}
void
OpenSCADBuiltins::f_flow_group_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output.setValue("Geometry", std::string("group() {\n") +
        input.getValue("a", "{}") +
        std::string("});"));
}

////////////////////////////////////////
// Output
////////////////////////////////////////
OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_output()
{
    auto model = std::make_unique<NodeModelType>("output", "Output", _OPENSCAD_NODE_CATEGORY);
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY), "out", QtNodes::ConnectionPolicy::Many);
    model->setProcessor(f_flow_output_process);
    return model;
}
void
OpenSCADBuiltins::f_flow_output_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string s = input.getValue("out", "//No Geometry Output\n");
    output.setValue("out", s);
}



