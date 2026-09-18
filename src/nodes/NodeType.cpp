#include "NodeType.hpp"

using namespace JNodes::core;

NodeType::NodeType(std::string name, std::string caption, std::string category)
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
NodeType::getName() const
{ return _name; }

std::string
NodeType::getCaption() const
{ return _caption; }

std::string
NodeType::getCategory() const
{ return _category; }

std::string
NodeType::getIcon() const
{ return _icon; }

bool
NodeType::getResizable() const
{ return _resizable; }

void
NodeType::setResizable(bool resizable)
{ _resizable = resizable; }

void
NodeType::addInputPort(std::unique_ptr<NodePort> inputPort, std::string inputPortName, QtNodes::ConnectionPolicy policy)
{
    _inputPortNames[_inputPorts.size()] = inputPortName;
    inputPort->setConnectionPolicy(policy);
    _inputPorts.push_back(std::move(inputPort));
}

void
NodeType::addInputPort(std::unique_ptr<NodePort> inputPort, std::string inputPortName)
{
    addInputPort(std::move(inputPort), inputPortName, QtNodes::ConnectionPolicy::One);
}

unsigned int
NodeType::getInputPortCount() const
{
    return _inputPorts.size();
}

unsigned int
NodeType::getOutputPortCount() const
{
    return _outputPorts.size();
}
const NodePort &
NodeType::getInputPort(unsigned int index) const
{
    return *_inputPorts.at(index);
}
const NodePort &
NodeType::getOutputPort(unsigned int index) const
{
    return *_outputPorts.at(index);
}

std::string
NodeType::getInputPortName(unsigned int index) const
{
    return _inputPortNames.at(index);
}

std::string
NodeType::getOutputPortName(unsigned int index) const
{
    return _outputPortNames.at(index);
}

void
NodeType::addOutputPort(std::unique_ptr<NodePort> outputPort, std::string outputPortName)
{
    _outputPortNames[_outputPorts.size()] = outputPortName;
    outputPort->setConnectionPolicy(QtNodes::ConnectionPolicy::Many);
    _outputPorts.push_back(std::move(outputPort));
}

////////////////////////////////////////
// Processor
////////////////////////////////////////
void
NodeType::setProcessor(NodeType::NodeProcessor processor)
{
    _processor = processor;
}

NodeType::NodeProcessor
NodeType::getProcessor() const
{
    return _processor;
}

void
NodeType::default_processor(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{}

////////////////////////////////////////
// Initializer
////////////////////////////////////////
void
NodeType::setInitializer(NodeType::Initializer initializer)
{
    _initializer = initializer;
}

NodeType::Initializer
NodeType::getInitializer() const
{
    return _initializer;
}

void
NodeType::default_initializer(OpenSCADBuiltinModel &model)
{}

////////////////////////////////////////
// Widget Factory
////////////////////////////////////////
void
NodeType::setWidgetFactory(NodeType::WidgetFactory widgetFactory)
{
    _widgetFactory = widgetFactory;
}

NodeType::WidgetFactory
NodeType::getWidgetFactory() const
{
    return _widgetFactory;
}


QWidget*
NodeType::default_widget_factory(OpenSCADBuiltinModel &)
{
    return nullptr;
}

