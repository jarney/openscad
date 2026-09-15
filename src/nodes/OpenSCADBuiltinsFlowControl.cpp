#include "nodes/OpenSCADBuiltins.hpp"
#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltins_helpers.hpp"

#include <QtWidgets/QPushButton>

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_if()
{
    auto model = std::make_unique<NodeModelType>("if", "If", "Flow Control");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "condition"), "condition");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "b"), "b");
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

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_flow_for()
{
    auto model = std::make_unique<NodeModelType>("for", "Loop", "Flow Control");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "start"), "start");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "end"), "end");
    model->addOutputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(OpenSCADBuiltins::f_flow_for_process);
    model->setWidgetFactory(OpenSCADBuiltins::f_flow_for_widget);
    return model;
}

QWidget*
OpenSCADBuiltins::f_flow_for_widget(OpenSCADBuiltinModel *model)
{
    fprintf(stderr, "Push button for edit of for content\n");
    QPushButton *button = new QPushButton();
    button->setText("Edit");
    QObject::connect(button, &QPushButton::clicked, [model]() {
	fprintf(stderr, "Edit button pushed\n");
	model->editGraph();
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

OpenSCADBuiltins::RegistryItemPtr
OpenSCADBuiltins::f_output()
{
    auto model = std::make_unique<NodeModelType>("output", "Output", "Output");
    model->addInputPort(std::make_unique<NodeModelPort>(DATA_SOLID_GEOMETRY), "out");
    model->setProcessor(f_output_process);
    return model;
}
void
OpenSCADBuiltins::f_output_process(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string s = input.getValue("out", "//No Geometry Output\n");
    output.setValue("out", s);
}

