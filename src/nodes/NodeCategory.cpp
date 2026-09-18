#include "nodes/NodeCategory.hpp"

NodeCategory::NodeCategory(std::string name, std::string description, std::string icon)
    : _name(name)
    , _description(description)
    , _icon(icon)
{}

std::string
NodeCategory::getName() const
{
    return _name;
}

std::string
NodeCategory::getDescription() const
{
    return _description;
}

std::string
NodeCategory::getIcon() const
{
    return _icon;
}

