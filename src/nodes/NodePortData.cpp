#include "nodes/NodePortData.hpp"

using namespace JNodes::core;

NodePortData::NodePortData()
{}

NodePortData::NodePortData(const NodePortData & other)
    : _data(other._data)
{}

void
NodePortData::setValue(std::string key, std::string value)
{
    _data[key] = value;
}

std::string
NodePortData::getValue(std::string key, std::string default_value) const
{
    const auto it = _data.find(key);
    if (it == _data.end()) {
	return default_value;
    }
    return it->second;
}
std::string
NodePortData::getValue(std::string key) const
{
    return getValue(key, "");
}

bool
NodePortData::hasValue(std::string key) const
{
    const auto it = _data.find(key);
    if (it == _data.end()) {
	return false;
    }
    return true;
}
