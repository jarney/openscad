#include "nodes/NodeFactory.hpp"
#include "nodes/Node.hpp"

using namespace JNodes::core;

NodeFactoryTyped::NodeFactoryTyped(
    std::unique_ptr<NodeType> type
    )
    : _type(std::move(type))
{
}
std::string
NodeFactoryTyped::getName() const
{
    return _type->getName();
}

std::string
NodeFactoryTyped::getDescription() const
{
    return _type->getCaption();
}

std::string
NodeFactoryTyped::getCategory() const
{
    return _type->getCategory();
}

std::unique_ptr<QtNodes::NodeDelegateModel>
NodeFactoryTyped::create(NodeGraph & model) const
{
    std::unique_ptr<Node> node = std::make_unique<Node>(*_type, model);
    return node;
}

std::string
NodeFactoryTyped::getIcon() const
{
    return _type->getIcon();
}

