#pragma once

#include "nodes/NodePort.hpp"
#include "nodes/PortFunctionData.hpp"

namespace JNodes {
    namespace core {

class OpenSCADBuiltinModel;

class NodeModelType {
public:
    typedef std::function<void(const OpenSCADBuiltinModel & model, const PortFunctionData &, PortFunctionData & )> NodeProcessor;
    typedef std::function<QWidget*(OpenSCADBuiltinModel &)> WidgetFactory;
    typedef std::function<void(OpenSCADBuiltinModel &)> Initializer;
    
    NodeModelType(std::string name, std::string caption, std::string category);
    std::string getName() const;
    std::string getCaption() const;
    std::string getCategory() const;
    std::string getIcon() const;
    bool getResizable() const;
    void setResizable(bool resizable);
    
    void addInputPort(std::unique_ptr<NodePort> inputPort, std::string inputPortName, QtNodes::ConnectionPolicy policy);
    void addInputPort(std::unique_ptr<NodePort> inputPort, std::string inputPortName);
    void addOutputPort(std::unique_ptr<NodePort> outputPort, std::string outputPortName);

    unsigned int getInputPortCount() const;
    unsigned int getOutputPortCount() const;
    const NodePort & getInputPort(unsigned int index) const;
    const NodePort & getOutputPort(unsigned int index) const;
    std::string getInputPortName(unsigned int index) const;
    std::string getOutputPortName(unsigned int index) const;

    void setProcessor(NodeProcessor processor);
    NodeProcessor getProcessor() const;
    static void default_processor(const OpenSCADBuiltinModel & model, const PortFunctionData & input, PortFunctionData & output);
    
    void setInitializer(Initializer initializer);
    Initializer getInitializer() const;
    static void default_initializer(OpenSCADBuiltinModel &model);
    
    void setWidgetFactory(WidgetFactory widgetFactory);
    WidgetFactory getWidgetFactory() const;
    static QWidget *default_widget_factory(OpenSCADBuiltinModel &model);
    
    void dump();
private:
    std::string _name;
    std::string _caption;
    std::string _category;
    std::string _icon;
    bool _resizable;
    std::vector<std::unique_ptr<NodePort>> _inputPorts;
    std::vector<std::unique_ptr<NodePort>> _outputPorts;
    std::map<int, std::string> _inputPortNames;
    std::map<int, std::string> _outputPortNames;
    NodeModelType::NodeProcessor _processor;
    NodeModelType::Initializer _initializer;
    NodeModelType::WidgetFactory _widgetFactory;
};

    }
}
