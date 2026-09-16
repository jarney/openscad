#include "nodes/OpenSCADBuiltinFactory.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

OpenSCADBuiltinFactory::OpenSCADBuiltinFactory(
    std::unique_ptr<NodeModelType> type
    )
    : _type(std::move(type))
{
    _icon = new QIcon("../resources/icons/prefsEditor.png");
}
OpenSCADBuiltinFactory::~OpenSCADBuiltinFactory()
{
    delete _icon;
}
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
OpenSCADBuiltinFactory::create(NodeProgramGraphModel & model) const
{
    return std::make_unique<OpenSCADBuiltinModel>(*_type);
}

QIcon *
OpenSCADBuiltinFactory::getIcon()
{
    return _icon;
}

