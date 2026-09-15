#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

OpenSCADBuiltinFactory::OpenSCADBuiltinFactory(
    std::unique_ptr<NodeModelType> type
    )
    : _type(std::move(type))
{}

std::string
OpenSCADBuiltinFactory::getName() const
{
    return _type->getName();
}

std::string
OpenSCADBuiltinFactory::getCategory() const
{
    return _type->getCategory();
}

std::unique_ptr<QtNodes::NodeDelegateModel>
OpenSCADBuiltinFactory::create(void) const
{
    return std::make_unique<OpenSCADBuiltinModel>(*_type);
}

