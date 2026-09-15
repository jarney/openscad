#include "NodeModelType.hpp"

NodeModelType::NodeModelType(std::string name, std::string caption, std::string category)
    : _name(name)
    , _caption(caption)
    , _category(category)
    , _processor(default_processor)
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

void
NodeModelType::addInputPort(std::unique_ptr<NodeModelPort> inputPort, std::string inputPortName)
{
    _inputPortNames[_inputPorts.size()] = inputPortName;
    _inputPorts.push_back(std::move(inputPort));
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
const NodeModelPort &
NodeModelType::getInputPort(unsigned int index) const
{
    return *_inputPorts.at(index);
}
const NodeModelPort &
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
NodeModelType::addOutputPort(std::unique_ptr<NodeModelPort> outputPort, std::string outputPortName)
{
    _outputPortNames[_outputPorts.size()] = outputPortName;
    _outputPorts.push_back(std::move(outputPort));
}

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
NodeModelType::setWidgetFactory(NodeModelType::WidgetFactory widgetFactory)
{
    _widgetFactory = widgetFactory;
}

NodeModelType::WidgetFactory
NodeModelType::getWidgetFactory() const
{
    return _widgetFactory;
}


void
NodeModelType::default_processor(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output)
{
}

QWidget*
NodeModelType::default_widget_factory(OpenSCADBuiltinModel *)
{
    return nullptr;
}

