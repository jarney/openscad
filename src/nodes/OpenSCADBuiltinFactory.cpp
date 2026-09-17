#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

OpenSCADBuiltinFactory::OpenSCADBuiltinFactory(
    std::unique_ptr<NodeModelType> type
    )
    : _type(std::move(type))
    , _icon("../resources/icons/prefsEditor.png")
{
}
std::string
OpenSCADBuiltinFactory::getName() const
{
    return _type->getName();
}

std::string
OpenSCADBuiltinFactory::getDescription() const
{
    return _type->getCaption();
}

std::string
OpenSCADBuiltinFactory::getCategory() const
{
    return _type->getCategory();
}

std::unique_ptr<QtNodes::NodeDelegateModel>
OpenSCADBuiltinFactory::create(NodeProgramGraphModel & model) const
{
    std::unique_ptr<OpenSCADBuiltinModel> node = std::make_unique<OpenSCADBuiltinModel>(*_type, model);
    return node;
}

std::string
OpenSCADBuiltinFactory::getIcon() const
{
    return _icon;
}

