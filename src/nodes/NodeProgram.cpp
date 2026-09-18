#include "NodeProgram.hpp"

using namespace JNodes::core;

NodeProgram::NodeProgram(std::shared_ptr<NodeProgramModelRegistry> registry)
    : _registry(registry)
    , _maxGraphId(0)
{}
NodeProgram::~NodeProgram()
{
    for (const auto graphId : getGraphs()) {
	deleteGraph(graphId);
    }
}

NodeGraph *
NodeProgram::getGraph(GraphId aId)
{
    auto it = graphs.find(aId);
    if (it == graphs.end()) {
	return {};
    }
    return it->second;
}

const NodeGraph *
NodeProgram::getGraph(GraphId aId) const
{
    auto it = graphs.find(aId);
    if (it == graphs.end()) {
	return {};
    }
    return it->second;
}

std::vector<NodeProgram::GraphId>
NodeProgram::getGraphs() const
{
    std::vector<NodeProgram::GraphId> list;
    list.reserve(graphs.size());
    for (const auto & it : graphs) {
	list.push_back(it.first);
    }
    return list;
}

NodeProgram::GraphId
NodeProgram::newGraphWithPrefix(std::string prefix)
{
    NodeGraph *newGraphObj = new NodeGraph(_registry, *this);
    // Need a much better way of doing this.
    while (true) {
	GraphId newId = prefix + std::string("_") + std::to_string(_maxGraphId++);
	if (graphs.count(newId) == 0) {
	    graphs[newId] = newGraphObj;
	    return newId;
	}
    }
}

NodeGraph *
NodeProgram::newGraph(NodeProgram::GraphId aId)
{
    if (graphs.find(aId) != graphs.end()) {
	return nullptr;
    }
    NodeGraph *newGraphObj = new NodeGraph(_registry, *this);
    graphs[aId] = newGraphObj;
    return newGraphObj;
}

void
NodeProgram::deleteGraph(NodeProgram::GraphId aId)
{
    delete graphs[aId];
    graphs.erase(aId);
}
void
NodeProgram::clear()
{
    for (const auto & it : graphs) {
	delete it.second;
    }
    graphs.clear();
}

