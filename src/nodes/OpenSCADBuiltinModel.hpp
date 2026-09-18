#pragma once

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeData>
#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgram.hpp"

namespace JNodes {
    namespace gui {
	class NodeEditorWidget;
    }

    namespace core {
	
class NodeProgramGraphModel;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class OpenSCADBuiltinModel : public QtNodes::NodeDelegateModel {
public:
    OpenSCADBuiltinModel(const NodeModelType & modelType, NodeProgramGraphModel & graph);
    virtual ~OpenSCADBuiltinModel() = default;

    unsigned int nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QtNodes::ConnectionPolicy portConnectionPolicy(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const;
    
    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    bool portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) override;

    QString name() const override;
    QString caption() const override;

    QWidget *embeddedWidget() override;

    void addInputPort(std::unique_ptr<NodePort> inputPort, std::string inputPortName);
    void addOutputPort(std::unique_ptr<NodePort> outputPort, std::string outputPortName);
    std::string inputPortName(QtNodes::PortIndex portIndex) const;
    std::string outputPortName(QtNodes::PortIndex portIndex) const;
    
    void process(const PortFunctionData & input, PortFunctionData & output) const;

    virtual bool resizable() const override;
    
    void setEditor(JNodes::gui::NodeEditorWidget *receiver);
    JNodes::gui::NodeEditorWidget *getEditor() const;

    bool hasValue(std::string key) const;
    std::string getValue(std::string key, std::string default_value) const;
    std::string getValue(std::string key) const;
    void setValue(std::string key, std::string value);
    
    void editGraph(NodeProgram::GraphId graphId) const;

    QJsonObject save() const;
    void load(QJsonObject const &);

    NodeProgramGraphModel & getGraph() const;
    
protected:
    // Data purely about the abstract node
    // that is the same for each instance.  Factor this out
    // to a node-type class.
    const NodeModelType & _modelType;
    NodeProgramGraphModel & _graph;
    QWidget *_widget;
    JNodes::gui::NodeEditorWidget *_editor;
    std::map<std::string, std::string> _modelData;
};

    } // End core
} // End JNodes
