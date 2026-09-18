#pragma once

//#include "Export.hpp"
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModel>
#include <QtNodes/internal/QStringStdHash.hpp>

#include <QtCore/QString>

#include <functional>
#include <memory>
#include <set>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "nodes/NodeCategory.hpp"
#include "nodes/NodeFactory.hpp"

namespace JNodes {
    namespace core {

class NodeGraph;

/// Class uses map for storing models (name, model)
class NodeFactoryRegistry {
public:
    using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
    using RegistryItemCreator = std::unique_ptr<JNodes::core::NodeFactory>;
    using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
    using CategoriesSet = std::set<QString>;

    //using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;

    NodeFactoryRegistry() = default;
    ~NodeFactoryRegistry() = default;

    NodeFactoryRegistry(NodeFactoryRegistry const &) = delete;
    NodeFactoryRegistry(NodeFactoryRegistry &&) = default;

    NodeFactoryRegistry &operator=(NodeFactoryRegistry const &) = delete;

    NodeFactoryRegistry &operator=(NodeFactoryRegistry &&) = default;

public:
    void registerCategory(const NodeCategory & category);

    const std::map<std::string, const NodeCategory *> & getCategories() const;

    const NodeCategory * getCategory(std::string category_name) const;
    
    const std::vector<const JNodes::core::NodeFactory *> & getModelsByCategory(std::string category) const;

    void registerModel(std::unique_ptr<JNodes::core::NodeFactory> factory);
    
    std::unique_ptr<QtNodes::NodeDelegateModel> create(QString const &modelName, NodeGraph & graph);

    const RegisteredModelCreatorsMap & getModels() const;
    
    CategoriesSet const &categories() const;

private:

    // Map of category name to  category.
    std::map<std::string, const NodeCategory *> _categoryMap;
    std::map<std::string, std::vector<const JNodes::core::NodeFactory *>> _nodesByCategory;
    
    CategoriesSet _categories;
    RegisteredModelCreatorsMap _registeredItemCreators;
};

    } // End core
} // End JNodes
