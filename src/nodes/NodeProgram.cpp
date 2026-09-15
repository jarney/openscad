#include "NodeProgram.hpp"

NodeProgram::NodeProgram(std::shared_ptr<NodeProgramModelRegistry> registry)
    : _registry(registry)
{}
NodeProgram::~NodeProgram()
{
    for (const auto graphId : getGraphs()) {
	deleteGraph(graphId);
    }
}

NodeProgramGraphModel *
NodeProgram::getGraph(GraphId aId)
{
    auto it = graphs.find(aId);
    if (it == graphs.end()) {
	return {};
    }
    return it->second;
}

const NodeProgramGraphModel *
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
NodeProgram::newGraph()
{
    NodeProgramGraphModel *newGraphObj = new NodeProgramGraphModel(_registry, *this);
    GraphId newId = std::to_string(maxGraphId++);
    graphs[newId] = newGraphObj;
    return newId;
}

NodeProgramGraphModel *
NodeProgram::newGraph(NodeProgram::GraphId aId)
{
    if (graphs.find(aId) != graphs.end()) {
	return nullptr;
    }
    NodeProgramGraphModel *newGraphObj = new NodeProgramGraphModel(_registry, *this);
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

