#pragma once

#include "nodes/OpenSCADDataTypes.hpp"
#include "nodes/NodeModelPort.hpp"

typedef std::map<std::string, std::string> PortFunctionData;

class NewSCADModel;

class NodeModelType {
public:
    typedef std::function<void(const NewSCADModel & model, const PortFunctionData &, PortFunctionData & )> NodeProcessor;
    typedef std::function<QWidget*(NewSCADModel *)> WidgetFactory;
    
    NodeModelType(std::string name, std::string caption, std::string category);
    std::string getName() const;
    std::string getCaption() const;
    std::string getCategory() const;
    
    void addInputPort(std::unique_ptr<NodeModelPort> inputPort, std::string inputPortName);
    void addOutputPort(std::unique_ptr<NodeModelPort> outputPort, std::string outputPortName);

    unsigned int getInputPortCount() const;
    unsigned int getOutputPortCount() const;
    const NodeModelPort & getInputPort(unsigned int index) const;
    const NodeModelPort & getOutputPort(unsigned int index) const;
    std::string getInputPortName(unsigned int index) const;
    std::string getOutputPortName(unsigned int index) const;

    void setProcessor(NodeProcessor processor);
    NodeProcessor getProcessor() const;
    
    void setWidgetFactory(WidgetFactory widgetFactory);
    WidgetFactory getWidgetFactory() const;

    static void default_processor(const NewSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static QWidget *default_widget_factory(NewSCADModel *model);

    void dump();
private:
    std::string _name;
    std::string _caption;
    std::string _category;
    std::vector<std::unique_ptr<NodeModelPort>> _inputPorts;
    std::vector<std::unique_ptr<NodeModelPort>> _outputPorts;
    std::map<int, std::string> _inputPortNames;
    std::map<int, std::string> _outputPortNames;
    NodeModelType::NodeProcessor _processor;
    NodeModelType::WidgetFactory _widgetFactory;
};


