#pragma once

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeData>
#include "nodes/NodeModelType.hpp"

class JNodeProgramEditor;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class OpenSCADBuiltinModel : public QtNodes::NodeDelegateModel {
public:
    OpenSCADBuiltinModel(const NodeModelType & modelType);
    virtual ~OpenSCADBuiltinModel() = default;

    unsigned int nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    bool portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) override;

    QString name() const override;
    QString caption() const override;

    QWidget *embeddedWidget() override;

    void addInputPort(std::unique_ptr<NodeModelPort> inputPort, std::string inputPortName);
    void addOutputPort(std::unique_ptr<NodeModelPort> outputPort, std::string outputPortName);
    std::string inputPortName(QtNodes::PortIndex portIndex) const;
    std::string outputPortName(QtNodes::PortIndex portIndex) const;
    
    void process(const PortFunctionData & input, PortFunctionData & output) const;

    virtual bool resizable() const override;
    
    void setEditor(JNodeProgramEditor *receiver);
    JNodeProgramEditor *getEditor() const;

    bool hasValue(std::string key) const;
    std::string getValue(std::string key, std::string default_value) const;
    void setValue(std::string key, std::string value);
    
    void editGraph();

protected:
    // Data purely about the abstract node
    // that is the same for each instance.  Factor this out
    // to a node-type class.
    const NodeModelType & _modelType;
    QWidget *_widget;
    JNodeProgramEditor *_editor;
    std::map<std::string, std::string> _modelData;
};

