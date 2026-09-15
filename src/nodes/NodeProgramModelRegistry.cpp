#include "NodeProgramModelRegistry.hpp"

std::unique_ptr<QtNodes::NodeDelegateModel> NodeProgramModelRegistry::create(QString const &modelName)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end()) {
        return it->second->create();
    }

    return nullptr;
}

NodeProgramModelRegistry::RegisteredModelCreatorsMap const &
NodeProgramModelRegistry::registeredModelCreators() const
{
    return _registeredItemCreators;
}

NodeProgramModelRegistry::RegisteredModelsCategoryMap const &
NodeProgramModelRegistry::registeredModelsCategoryAssociation() const
{
    return _registeredModelsCategory;
}

NodeProgramModelRegistry::CategoriesSet const &NodeProgramModelRegistry::categories() const
{
    return _categories;
}

void
NodeProgramModelRegistry::registerModel(std::unique_ptr<NodeDelegateFactory> factory)
{
    QString const name = QString::fromStdString(factory->getName());
    if (!_registeredItemCreators.count(name)) {
	QString category = QString::fromStdString(factory->getCategory());
	_registeredItemCreators[name] = std::move(factory);
	_categories.insert(category);
	_registeredModelsCategory[name] = category;
    }	
}
