#include "nodes/PortFunctionData.hpp"

PortFunctionData::PortFunctionData()
{}

PortFunctionData::PortFunctionData(const PortFunctionData & other)
    : _data(other._data)
{}

void
PortFunctionData::setValue(std::string key, std::string value)
{
    _data[key] = value;
}

std::string
PortFunctionData::getValue(std::string key, std::string default_value) const
{
    const auto it = _data.find(key);
    if (it == _data.end()) {
	return default_value;
    }
    return it->second;
}
std::string
PortFunctionData::getValue(std::string key) const
{
    return getValue(key, "");
}

bool
PortFunctionData::hasValue(std::string key) const
{
    const auto it = _data.find(key);
    if (it == _data.end()) {
	return false;
    }
    return true;
}
