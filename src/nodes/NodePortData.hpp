#pragma once

#include <string>
#include <map>

namespace JNodes {
    namespace core {

class NodePortData {
public:
    NodePortData();
    NodePortData(const NodePortData & other);
    NodePortData & operator=(const NodePortData & ) = delete;
    ~NodePortData() = default;
    
    void setValue(std::string key, std::string value);
    std::string getValue(std::string key, std::string default_value) const;
    std::string getValue(std::string key) const;
    bool hasValue(std::string key) const;
private:
    std::map<std::string, std::string> _data;
};

    } // End core
} // End JNodes
