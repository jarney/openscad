#include "NodeModelType.hpp"

using namespace JNodes::core;

NodeModelType::NodeModelType(std::string name, std::string caption, std::string category)
    : _name(name)
    , _caption(caption)
    , _category(category)
    , _icon("../resources/icons/prefsEditor.png")
    , _resizable(false)
    , _processor(default_processor)
    , _initializer(default_initializer)
    , _widgetFactory(default_widget_factory)
{}

std::string
NodeModelType::getName() const
{ return _name; }

std::string
NodeModelType::getCaption() const
{ return _caption; }

std::string
NodeModelType::getCategory() const
{ return _category; }

std::string
NodeModelType::getIcon() const
{ return _icon; }

bool
NodeModelType::getResizable() const
{ return _resizable; }

void
NodeModelType::setResizable(bool resizable)
{ _resizable = resizable; }

void
NodeModelType::addInputPort(std::unique_ptr<NodePort> inputPort, std::string inputPortName, QtNodes::ConnectionPolicy policy)
{
    _inputPortNames[_inputPorts.size()] = inputPortName;
    inputPort->setConnectionPolicy(policy);
    _inputPorts.push_back(std::move(inputPort));
}

void
NodeModelType::addInputPort(std::unique_ptr<NodePort> inputPort, std::string inputPortName)
{
    addInputPort(std::move(inputPort), inputPortName, QtNodes::ConnectionPolicy::One);
}

unsigned int
NodeModelType::getInputPortCount() const
{
    return _inputPorts.size();
}

unsigned int
NodeModelType::getOutputPortCount() const
{
    return _outputPorts.size();
}
const NodePort &
NodeModelType::getInputPort(unsigned int index) const
{
    return *_inputPorts.at(index);
}
const NodePort &
NodeModelType::getOutputPort(unsigned int index) const
{
    return *_outputPorts.at(index);
}

std::string
NodeModelType::getInputPortName(unsigned int index) const
{
    return _inputPortNames.at(index);
}

std::string
NodeModelType::getOutputPortName(unsigned int index) const
{
    return _outputPortNames.at(index);
}

void
NodeModelType::addOutputPort(std::unique_ptr<NodePort> outputPort, std::string outputPortName)
{
    _outputPortNames[_outputPorts.size()] = outputPortName;
    outputPort->setConnectionPolicy(QtNodes::ConnectionPolicy::Many);
    _outputPorts.push_back(std::move(outputPort));
}

////////////////////////////////////////
// Processor
////////////////////////////////////////
void
NodeModelType::setProcessor(NodeModelType::NodeProcessor processor)
{
    _processor = processor;
}

NodeModelType::NodeProcessor
NodeModelType::getProcessor() const
{
    return _processor;
}

void
NodeModelType::default_processor(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{}

////////////////////////////////////////
// Initializer
////////////////////////////////////////
void
NodeModelType::setInitializer(NodeModelType::Initializer initializer)
{
    _initializer = initializer;
}

NodeModelType::Initializer
NodeModelType::getInitializer() const
{
    return _initializer;
}

void
NodeModelType::default_initializer(OpenSCADBuiltinModel &model)
{}

////////////////////////////////////////
// Widget Factory
////////////////////////////////////////
void
NodeModelType::setWidgetFactory(NodeModelType::WidgetFactory widgetFactory)
{
    _widgetFactory = widgetFactory;
}

NodeModelType::WidgetFactory
NodeModelType::getWidgetFactory() const
{
    return _widgetFactory;
}


QWidget*
NodeModelType::default_widget_factory(OpenSCADBuiltinModel &)
{
    return nullptr;
}

