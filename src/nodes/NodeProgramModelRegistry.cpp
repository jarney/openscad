#include "NodeProgramModelRegistry.hpp"

std::unique_ptr<QtNodes::NodeDelegateModel> NodeProgramModelRegistry::create(QString const &modelName, NodeProgramGraphModel & graph)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end()) {
        return it->second->create(graph);
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
NodeProgramModelRegistry::registerCategory(const NodeModelCategory & category)
{
    _categoryMap.insert(std::make_pair(category.getName(), category));
}

const std::map<std::string, const NodeModelCategory &> &
NodeProgramModelRegistry::getCategories() const
{
    return _categoryMap;
}

const NodeModelCategory *
NodeProgramModelRegistry::getCategory(std::string category_name) const
{
    const auto it = _categoryMap.find(category_name);
    if (it == _categoryMap.end()) {
	return nullptr;
    }
    return &it->second;
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

QIcon *
NodeProgramModelRegistry::getIcon(QString const & modelName) const
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end()) {
        return it->second->getIcon();
    }

    return nullptr;
}
