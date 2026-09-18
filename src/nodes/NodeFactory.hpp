#pragma once

#include "nodes/NodeModelType.hpp"

namespace QtNodes {
    class NodeDelegateModel;
}

namespace JNodes {
    namespace core {

class NodeProgramGraphModel;

class NodeFactory {
public:
    virtual ~NodeFactory() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::unique_ptr<QtNodes::NodeDelegateModel> create(NodeProgramGraphModel & graph) const = 0;
    virtual std::string getIcon() const = 0;
};


class NodeFactoryTyped : public NodeFactory {
public:
    NodeFactoryTyped(
	std::unique_ptr<NodeModelType> type
    );
    virtual ~NodeFactoryTyped() = default;
    
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getCategory() const override;
    std::unique_ptr<QtNodes::NodeDelegateModel> create(NodeProgramGraphModel & model) const;
    std::string getIcon(void) const override;
private:
    std::unique_ptr<NodeModelType> _type;
};

    } // End core
} // End JNodes
