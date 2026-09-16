#pragma once

#include <string>

class NodeModelCategory {
public:
    NodeModelCategory(std::string name, std::string description, std::string icon);
    ~NodeModelCategory() = default;
    NodeModelCategory(const NodeModelCategory & other) = delete;
    NodeModelCategory & operator=(const NodeModelCategory & other) = delete;
    std::string getName() const;
    std::string getDescription() const;
    std::string getIcon() const;
private:
    std::string _name;
    std::string _description;
    std::string _icon;
};

