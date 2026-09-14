#include "NodeProgramModelRegistry.hpp"

std::unique_ptr<QtNodes::NodeDelegateModel> NodeProgramModelRegistry::create(QString const &modelName)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end()) {
        return it->second();
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
