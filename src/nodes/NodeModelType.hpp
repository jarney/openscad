#pragma once

#include "nodes/OpenSCADDataTypes.hpp"
#include "nodes/NodeModelPort.hpp"

typedef std::map<std::string, std::string> PortFunctionData;

class BaseSCADModel;

class NodeModelType {
public:
    typedef std::function<void(const BaseSCADModel & model, const PortFunctionData &, PortFunctionData & )> NodeProcessor;
    typedef std::function<QWidget*(BaseSCADModel *)> WidgetFactory;
    
    NodeModelType(std::string name, std::string caption);
    std::string name() const;
    std::string caption() const;
    
    void addInputPort(std::unique_ptr<NodeModelPort> inputPort, std::string inputPortName);
    void addOutputPort(std::unique_ptr<NodeModelPort> outputPort, std::string outputPortName);

    void setProcessor(NodeProcessor processor);
    void setWidgetFactory(WidgetFactory widgetFactory);

    void dump();
private:
    std::string _name;
    std::string _caption;
    std::vector<std::unique_ptr<NodeModelPort>> _inputPorts;
    std::vector<std::unique_ptr<NodeModelPort>> _outputPorts;
    std::map<int, std::string> _inputPortNames;
    std::map<int, std::string> _outputPortNames;
    NodeModelType::NodeProcessor _processor;
    NodeModelType::WidgetFactory _widgetFactory;
};


