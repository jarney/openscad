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

class NodeDelegateFactory {
public:
    virtual std::string getName() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::unique_ptr<QtNodes::NodeDelegateModel> create() const = 0;
};

/// Class uses map for storing models (name, model)
class NodeProgramModelRegistry {
public:
    using RegistryItemPtr = std::unique_ptr<QtNodes::NodeDelegateModel>;
    using RegistryItemCreator = std::unique_ptr<NodeDelegateFactory>;
    using RegisteredModelCreatorsMap = std::unordered_map<QString, RegistryItemCreator>;
    using RegisteredModelsCategoryMap = std::unordered_map<QString, QString>;
    using CategoriesSet = std::set<QString>;

    //using RegisteredTypeConvertersMap = std::map<TypeConverterId, TypeConverter>;

    NodeProgramModelRegistry() = default;
    ~NodeProgramModelRegistry() = default;

    NodeProgramModelRegistry(NodeProgramModelRegistry const &) = delete;
    NodeProgramModelRegistry(NodeProgramModelRegistry &&) = default;

    NodeProgramModelRegistry &operator=(NodeProgramModelRegistry const &) = delete;

    NodeProgramModelRegistry &operator=(NodeProgramModelRegistry &&) = default;

public:

    void registerModel(std::unique_ptr<NodeDelegateFactory> factory);
    
    std::unique_ptr<QtNodes::NodeDelegateModel> create(QString const &modelName);

    RegisteredModelCreatorsMap const &registeredModelCreators() const;

    RegisteredModelsCategoryMap const &registeredModelsCategoryAssociation() const;

    CategoriesSet const &categories() const;

private:
    CategoriesSet _categories;
    RegisteredModelsCategoryMap _registeredModelsCategory;
    RegisteredModelCreatorsMap _registeredItemCreators;
};
