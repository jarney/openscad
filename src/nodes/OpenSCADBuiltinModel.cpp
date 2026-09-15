//#include <QString>

#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"
#include "nodes/JNodeProgramEditor.hpp"

OpenSCADBuiltinModel::OpenSCADBuiltinModel(const NodeModelType & modelType)
    : _modelType(modelType)
    , _widget(nullptr)
    , _editor(nullptr)
{}

QString
OpenSCADBuiltinModel::name() const
{ return QString::fromStdString(_modelType.getName()); }

QString
OpenSCADBuiltinModel::caption() const
{ return QString::fromStdString(_modelType.getCaption()); }


QWidget *
OpenSCADBuiltinModel::embeddedWidget()
{
    if (!_widget) {
	_widget = _modelType.getWidgetFactory()(this);
    }
    return _widget;
}

unsigned int OpenSCADBuiltinModel::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPortCount();
    }
    else {
	return _modelType.getOutputPortCount();
    }
}

QtNodes::NodeDataType
OpenSCADBuiltinModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).nodeDataType();
    }
    else {
	return _modelType.getOutputPort(portIndex).nodeDataType();
    }
}

QString
OpenSCADBuiltinModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).portCaption();
    }
    else {
	return _modelType.getOutputPort(portIndex).portCaption();
    }
}

bool
OpenSCADBuiltinModel::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).portCaptionVisible();
    }
    else {
	return _modelType.getOutputPort(portIndex).portCaptionVisible();
    }
}

std::shared_ptr<QtNodes::NodeData> OpenSCADBuiltinModel::outData(QtNodes::PortIndex portIndex)
{
    /* Do nothing, we don't let the UI perform the evaluation */
    return nullptr;
}

void OpenSCADBuiltinModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex)
{
    if (!data) {
        Q_EMIT dataInvalidated(0);
    }

    /* Do nothing, we don't let the UI perform the evaluation */
}


std::string
OpenSCADBuiltinModel::inputPortName(QtNodes::PortIndex portIndex) const
{
    return _modelType.getInputPortName(portIndex);
}
std::string
OpenSCADBuiltinModel::outputPortName(QtNodes::PortIndex portIndex) const
{
    return _modelType.getOutputPortName(portIndex);
}

void
OpenSCADBuiltinModel::process(const PortFunctionData & input, PortFunctionData & output) const
{
    _modelType.getProcessor()(*this, input, output);
}

void
OpenSCADBuiltinModel::setEditor(JNodeProgramEditor *editor)
{
    _editor = editor;
}

JNodeProgramEditor *
OpenSCADBuiltinModel::getEditor() const
{
    return _editor;
}

void
OpenSCADBuiltinModel::editGraph()
{
    if (_editor) {
	_editor->editGraph("second-flow");
    }
}

bool
OpenSCADBuiltinModel::hasValue(std::string key) const
{
    const auto it = _modelData.find(key);
    if (it == _modelData.end()) {
	return false;
    }
    return true;
    
}

std::string
OpenSCADBuiltinModel::getValue(std::string key, std::string default_value) const
{
    const auto it = _modelData.find(key);
    if (it == _modelData.end()) {
	return default_value;
    }
    return it->second;    
}

void
OpenSCADBuiltinModel::setValue(std::string key, std::string value)
{
    _modelData[key] = value;
}

