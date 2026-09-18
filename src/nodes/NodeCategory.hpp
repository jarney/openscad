#pragma once

#include <string>

class NodeCategory {
public:
    NodeCategory(std::string name, std::string description, std::string icon);
    ~NodeCategory() = default;
    NodeCategory(const NodeCategory & other) = delete;
    NodeCategory & operator=(const NodeCategory & other) = delete;
    std::string getName() const;
    std::string getDescription() const;
    std::string getIcon() const;
private:
    std::string _name;
    std::string _description;
    std::string _icon;
};

