#include "NodeProgram.hpp"

NodeProgram::NodeProgram(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry)
    : _registry(registry)
{}
NodeProgram::~NodeProgram()
{
    for (const auto graphId : getGraphs()) {
	deleteGraph(graphId);
    }
}

OpenSCADGraphModel *
NodeProgram::getGraph(GraphId aId)
{
    auto it = graphs.find(aId);
    if (it == graphs.end()) {
	return {};
    }
    return it->second;
}

const OpenSCADGraphModel *
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
    OpenSCADGraphModel *newGraphObj = new OpenSCADGraphModel(_registry);
    GraphId newId = std::to_string(maxGraphId++);
    graphs[newId] = newGraphObj;
    return newId;
}

OpenSCADGraphModel *
NodeProgram::newGraph(NodeProgram::GraphId aId)
{
    if (graphs.find(aId) != graphs.end()) {
	return nullptr;
    }
    OpenSCADGraphModel *newGraphObj = new OpenSCADGraphModel(_registry);
    graphs[aId] = newGraphObj;
    return newGraphObj;
}

void
NodeProgram::deleteGraph(NodeProgram::GraphId aId)
{
    delete graphs[aId];
    graphs.erase(aId);
}

