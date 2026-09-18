#include "nodes/NodeFactory.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

using namespace JNodes::core;

NodeFactoryTyped::NodeFactoryTyped(
    std::unique_ptr<NodeModelType> type
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
NodeFactoryTyped::create(NodeProgramGraphModel & model) const
{
    std::unique_ptr<OpenSCADBuiltinModel> node = std::make_unique<OpenSCADBuiltinModel>(*_type, model);
    return node;
}

std::string
NodeFactoryTyped::getIcon() const
{
    return _type->getIcon();
}

