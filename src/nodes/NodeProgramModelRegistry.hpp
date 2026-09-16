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

class NodeProgramGraphModel;

class NodeDelegateFactory {
public:
    virtual ~NodeDelegateFactory() = default;
    virtual std::string getName() const = 0;
    virtual std::string getCategory() const = 0;
    virtual std::unique_ptr<QtNodes::NodeDelegateModel> create(NodeProgramGraphModel & graph) const = 0;
    virtual QIcon *getIcon() = 0;
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
    void registerCategory(const NodeModelCategory & category);
    const std::map<std::string, const NodeModelCategory &> & getCategories() const;
    const NodeModelCategory * getCategory(std::string category_name) const;
    
    void registerModel(std::unique_ptr<NodeDelegateFactory> factory);
    
    std::unique_ptr<QtNodes::NodeDelegateModel> create(QString const &modelName, NodeProgramGraphModel & graph);

    RegisteredModelCreatorsMap const &registeredModelCreators() const;

    RegisteredModelsCategoryMap const &registeredModelsCategoryAssociation() const;

    CategoriesSet const &categories() const;

    /**
     * Returns an icon for displaying the model
     * on the UI if it has one or nullptr if it
     * does not.  The model factory retains ownership
     * of the icon.
     */
    QIcon * getIcon(QString const & modelName) const;

    
private:

    // Map of category name to  category.
    std::map<std::string, const NodeModelCategory &> _categoryMap;
    
    CategoriesSet _categories;
    RegisteredModelsCategoryMap _registeredModelsCategory;
    RegisteredModelCreatorsMap _registeredItemCreators;
};
