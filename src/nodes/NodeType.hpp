#pragma once

#include "nodes/NodePort.hpp"
#include "nodes/NodePortData.hpp"

namespace JNodes {
    namespace core {

class Node;

class NodeType {
public:
    typedef std::function<void(const Node & node, const NodePortData &, NodePortData & )> NodeProcessor;
    typedef std::function<QWidget*(Node &)> WidgetFactory;
    typedef std::function<void(Node &)> Initializer;
    
    NodeType(std::string name, std::string caption, std::string category);
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
    static void default_processor(const Node & node, const NodePortData & input, NodePortData & output);
    
    void setInitializer(Initializer initializer);
    Initializer getInitializer() const;
    static void default_initializer(Node &model);
    
    void setWidgetFactory(WidgetFactory widgetFactory);
    WidgetFactory getWidgetFactory() const;
    static QWidget *default_widget_factory(Node &model);
    
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
    NodeType::NodeProcessor _processor;
    NodeType::Initializer _initializer;
    NodeType::WidgetFactory _widgetFactory;
};

    }
}
