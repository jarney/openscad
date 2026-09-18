#include "nodes/NodeFactoryRegistry.hpp"
#include "nodes/Node.hpp"
#include "nodes/gui/NodeEditorWidget.hpp"

using namespace JNodes::core;

Node::Node(const NodeType & modelType, NodeGraph & graph)
    : _modelType(modelType)
    , _graph(graph)
    , _widget(nullptr)
    , _editor(nullptr)
{}

QString
Node::name() const
{ return QString::fromStdString(_modelType.getName()); }

QString
Node::caption() const
{ return QString::fromStdString(_modelType.getCaption()); }


QWidget *
Node::embeddedWidget()
{
    if (!_widget) {
	_widget = _modelType.getWidgetFactory()(*this);
    }
    return _widget;
}

unsigned int Node::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPortCount();
    }
    else {
	return _modelType.getOutputPortCount();
    }
}

QtNodes::NodeDataType
Node::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).nodeDataType();
    }
    else {
	return _modelType.getOutputPort(portIndex).nodeDataType();
    }
}

QtNodes::ConnectionPolicy
Node::portConnectionPolicy(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).getConnectionPolicy();
    }
    else {
	return _modelType.getOutputPort(portIndex).getConnectionPolicy();
    }
}

QString
Node::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).portCaption();
    }
    else {
	return _modelType.getOutputPort(portIndex).portCaption();
    }
}

bool
Node::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _modelType.getInputPort(portIndex).portCaptionVisible();
    }
    else {
	return _modelType.getOutputPort(portIndex).portCaptionVisible();
    }
}

std::shared_ptr<QtNodes::NodeData> Node::outData(QtNodes::PortIndex portIndex)
{
    /* Do nothing, we don't let the UI perform the evaluation */
    return nullptr;
}

void Node::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex)
{
    if (!data) {
        Q_EMIT dataInvalidated(0);
    }

    /* Do nothing, we don't let the UI perform the evaluation */
}


std::string
Node::inputPortName(QtNodes::PortIndex portIndex) const
{
    return _modelType.getInputPortName(portIndex);
}
std::string
Node::outputPortName(QtNodes::PortIndex portIndex) const
{
    return _modelType.getOutputPortName(portIndex);
}

void
Node::process(const NodePortData & input, NodePortData & output) const
{
    _modelType.getProcessor()(*this, input, output);
}

bool
Node::resizable() const
{
    return _modelType.getResizable();
}

void
Node::setEditor(JNodes::gui::NodeEditorWidget *editor)
{
    _editor = editor;
}

JNodes::gui::NodeEditorWidget *
Node::getEditor() const
{
    return _editor;
}

void
Node::editGraph(NodeProgram::GraphId graphId) const
{
    if (_editor) {
	_editor->editGraph(graphId);
    }
}

bool
Node::hasValue(std::string key) const
{
    const auto it = _modelData.find(key);
    if (it == _modelData.end()) {
	return false;
    }
    return true;
    
}

std::string
Node::getValue(std::string key, std::string default_value) const
{
    const auto it = _modelData.find(key);
    if (it == _modelData.end()) {
	return default_value;
    }
    return it->second;    
}
std::string
Node::getValue(std::string key) const
{
    return getValue(key, "");
}

void
Node::setValue(std::string key, std::string value)
{
    _modelData[key] = value;
}

QJsonObject Node::save() const
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

void Node::load(QJsonObject const & obj)
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

NodeGraph &
Node::getGraph() const
{
    return _graph;
}
