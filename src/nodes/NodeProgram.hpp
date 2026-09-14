#pragma once

#include "OpenSCADGraphModel.hpp"
#include <QtNodes/NodeDelegateModelRegistry>
#include <optional>

/**
 * A node program consists of a collection of
 * directed acyclic graphs where each node
 * represents transformation of some data
 * from one or more inputs to one or more
 * outputs.  Each edge represents immutable
 * data being transformed.  Many transformations
 * are simple single-stage transformations representing
 * operations like addition or subtraction.  Other
 * operations may be composite consisting of calls
 * to other graph models.  For example, function calls
 * consist of calling another graph with its own inputs
 * and outputs.
 */
class NodeProgram {
public:
    typedef std::string GraphId;

    NodeProgram(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _registry);
    ~NodeProgram();
    
    OpenSCADGraphModel *getGraph(GraphId aId);

    const OpenSCADGraphModel *getGraph(GraphId aId) const;
    /**
     * This method returns a vector of node IDs
     * for each of the graphs in this program.
     */
    std::vector<GraphId> getGraphs() const;

    /**
     * This creates a new graph and returns
     * the id of it.  It is responsible for
     * allocating the node ID.
     */
    GraphId newGraph();

    /**
     * This creates a new graph with the
     * given ID.  If this returns null
     * then the given ID already existed.
     */
    OpenSCADGraphModel *newGraph(GraphId id);
    
    /**
     * This deletes the graph with the given id.
     */
    void deleteGraph(GraphId aId);

private:
    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> _registry;
    int maxGraphId;
    std::map<GraphId, OpenSCADGraphModel*> graphs;
};
