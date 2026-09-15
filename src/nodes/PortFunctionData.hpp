#pragma once

#include <string>
#include <map>

class PortFunctionData {
public:
    PortFunctionData();
    PortFunctionData(const PortFunctionData & other);
    PortFunctionData & operator=(const PortFunctionData & ) = delete;
    ~PortFunctionData() = default;
    
    void setValue(std::string key, std::string value);
    std::string getValue(std::string key, std::string default_value) const;
    bool hasValue(std::string key) const;
private:
    std::map<std::string, std::string> _data;
};

