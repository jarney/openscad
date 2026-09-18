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

#include "nodes/NodeModelCategory.hpp"
#include "nodes/NodeFactory.hpp"

namespace JNodes {
    namespace core {

class NodeProgramGraphModel;

/// Class uses map for storing models (name, model)
class NodeProgramModelRegistry {
public:
    using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
    using RegistryItemCreator = std::unique_ptr<JNodes::core::NodeFactory>;
    using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
    using CategoriesSet = std::set<QString>;

    //using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;

    NodeProgramModelRegistry() = default;
    ~NodeProgramModelRegistry() = default;

    NodeProgramModelRegistry(NodeProgramModelRegistry const &) = delete;
    NodeProgramModelRegistry(NodeProgramModelRegistry &&) = default;

    NodeProgramModelRegistry &operator=(NodeProgramModelRegistry const &) = delete;

    NodeProgramModelRegistry &operator=(NodeProgramModelRegistry &&) = default;

public:
    void registerCategory(const NodeModelCategory & category);

    const std::map<std::string, const NodeModelCategory *> & getCategories() const;

    const NodeModelCategory * getCategory(std::string category_name) const;
    
    const std::vector<const JNodes::core::NodeFactory *> & getModelsByCategory(std::string category) const;

    void registerModel(std::unique_ptr<JNodes::core::NodeFactory> factory);
    
    std::unique_ptr<QtNodes::NodeDelegateModel> create(QString const &modelName, NodeProgramGraphModel & graph);

    const RegisteredModelCreatorsMap & getModels() const;
    
    CategoriesSet const &categories() const;

private:

    // Map of category name to  category.
    std::map<std::string, const NodeModelCategory *> _categoryMap;
    std::map<std::string, std::vector<const JNodes::core::NodeFactory *>> _nodesByCategory;
    
    CategoriesSet _categories;
    RegisteredModelCreatorsMap _registeredItemCreators;
};

    } // End core
} // End JNodes
