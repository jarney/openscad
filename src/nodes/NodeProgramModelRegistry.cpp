#include "NodeProgramModelRegistry.hpp"

using namespace JNodes::core;

std::unique_ptr<QtNodes::NodeDelegateModel> NodeProgramModelRegistry::create(QString const &modelName, NodeProgramGraphModel & graph)
{
    auto it = _registeredItemCreators.find(modelName);

    if (it != _registeredItemCreators.end()) {
        return it->second->create(graph);
    }

    return nullptr;
}

NodeProgramModelRegistry::CategoriesSet const &
NodeProgramModelRegistry::categories() const
{
    return _categories;
}

const NodeProgramModelRegistry::RegisteredModelCreatorsMap &
NodeProgramModelRegistry::getModels() const
{
    return _registeredItemCreators;
}

void
NodeProgramModelRegistry::registerCategory(const NodeModelCategory & category)
{
    _categoryMap.insert(std::make_pair(category.getName(), &category));
}

const std::map<std::string, const NodeModelCategory *> &
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
    const NodeModelCategory *cat = it->second;
    return cat;
}

const std::vector<const NodeFactory *> &
NodeProgramModelRegistry::getModelsByCategory(std::string category) const
{
    static const std::vector<const NodeFactory*> emptyList;
    
    const auto it = _nodesByCategory.find(category);
    if (it == _nodesByCategory.end()) {
	return emptyList;
    }
    return it->second;
}

void
NodeProgramModelRegistry::registerModel(std::unique_ptr<NodeFactory> factory)
{
    QString const name = QString::fromStdString(factory->getName());
    if (!_registeredItemCreators.count(name)) {
	_nodesByCategory[factory->getCategory()].push_back(factory.get());
	_categories.insert(QString::fromStdString(factory->getCategory()));
	_registeredItemCreators[name] = std::move(factory);
    }	
}

