#include "nodes/NodeModelCategory.hpp"

NodeModelCategory::NodeModelCategory(std::string name, std::string description, std::string icon)
    : _name(name)
    , _description(description)
    , _icon(icon)
{}

std::string
NodeModelCategory::getName() const
{
    return _name;
}

std::string
NodeModelCategory::getDescription() const
{
    return _description;
}

std::string
NodeModelCategory::getIcon() const
{
    return _icon;
}

