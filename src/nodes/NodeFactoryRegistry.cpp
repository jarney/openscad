#include "NodeFactoryRegistry.hpp"

using namespace JNodes::core;

std::unique_ptr<QtNodes::NodeDelegateModel> NodeFactoryRegistry::create(QString const &modelName, NodeGraph & graph)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end()) {
        return it->second->create(graph);
    }

    return nullptr;
}

NodeFactoryRegistry::CategoriesSet const &
NodeFactoryRegistry::categories() const
{
    return _categories;
}

const NodeFactoryRegistry::RegisteredModelCreatorsMap &
NodeFactoryRegistry::getModels() const
{
    return _registeredItemCreators;
}

void
NodeFactoryRegistry::registerCategory(const NodeCategory & category)
{
    _categoryMap.insert(std::make_pair(category.getName(), &category));
}

const std::map<std::string, const NodeCategory *> &
NodeFactoryRegistry::getCategories() const
{
    return _categoryMap;
}

const NodeCategory *
NodeFactoryRegistry::getCategory(std::string category_name) const
{
    const auto it = _categoryMap.find(category_name);
    if (it == _categoryMap.end()) {
	return nullptr;
    }
    const NodeCategory *cat = it->second;
    return cat;
}

const std::vector<const NodeFactory *> &
NodeFactoryRegistry::getModelsByCategory(std::string category) const
{
    static const std::vector<const NodeFactory*> emptyList;
    
    const auto it = _nodesByCategory.find(category);
    if (it == _nodesByCategory.end()) {
	return emptyList;
    }
    return it->second;
}

void
NodeFactoryRegistry::registerModel(std::unique_ptr<NodeFactory> factory)
{
    QString const name = QString::fromStdString(factory->getName());
    if (!_registeredItemCreators.count(name)) {
	_nodesByCategory[factory->getCategory()].push_back(factory.get());
	_categories.insert(QString::fromStdString(factory->getCategory()));
	_registeredItemCreators[name] = std::move(factory);
    }	
}

