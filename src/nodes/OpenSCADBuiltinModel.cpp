#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"
#include "nodes/gui/NodeEditorWidget.hpp"

OpenSCADBuiltinModel::OpenSCADBuiltinModel(const NodeModelType & modelType, NodeProgramGraphModel & graph)
    : _modelType(modelType)
    , _graph(graph)
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
	_widget = _modelType.getWidgetFactory()(*this);
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

QtNodes::ConnectionPolicy
OpenSCADBuiltinModel::portConnectionPolicy(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).getConnectionPolicy();
    }
    else {
	return _modelType.getOutputPort(portIndex).getConnectionPolicy();
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

bool
OpenSCADBuiltinModel::resizable() const
{
    return _modelType.getResizable();
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
OpenSCADBuiltinModel::editGraph(NodeProgram::GraphId graphId) const
{
    if (_editor) {
	_editor->editGraph(graphId);
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
std::string
OpenSCADBuiltinModel::getValue(std::string key) const
{
    return getValue(key, "");
}

void
OpenSCADBuiltinModel::setValue(std::string key, std::string value)
{
    _modelData[key] = value;
}

QJsonObject OpenSCADBuiltinModel::save() const
{
    QJsonObject modelJson;

    modelJson["model-name"] = name();
    QJsonObject data;
    for (const auto & it : _modelData) {
	data[QString::fromStdString(it.first)] = QString::fromStdString(it.second);
    }
    modelJson["data"] = data;

    return modelJson;
}

void OpenSCADBuiltinModel::load(QJsonObject const & obj)
{
    if (obj.contains("data")) {
	QJsonObject data = obj["data"].toObject();
	
	for (const auto [key, value] : data.asKeyValueRange()) {
	    const auto s_key = key.toString().toStdString();
	    const auto s_value = value.toString().toStdString();
	    _modelData[s_key] = s_value;
	}
    }
    _modelType.getInitializer()(*this);
}

NodeProgramGraphModel &
OpenSCADBuiltinModel::getGraph() const
{
    return _graph;
}
