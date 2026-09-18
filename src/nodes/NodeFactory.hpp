#pragma once

#include "nodes/NodeType.hpp"

namespace QtNodes {
    class NodeDelegateModel;
}

namespace JNodes {
    namespace core {

class NodeGraph;

class NodeFactory {
public:
    virtual ~NodeFactory() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::unique_ptr<QtNodes::NodeDelegateModel> create(NodeGraph & graph) const = 0;
    virtual std::string getIcon() const = 0;
};


class NodeFactoryTyped : public NodeFactory {
public:
    NodeFactoryTyped(
	std::unique_ptr<NodeType> type
    );
    virtual ~NodeFactoryTyped() = default;
    
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getCategory() const override;
    std::unique_ptr<QtNodes::NodeDelegateModel> create(NodeGraph & model) const;
    std::string getIcon(void) const override;
private:
    std::unique_ptr<NodeType> _type;
};

    } // End core
} // End JNodes
