#include "JNodeProgramEditor.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

#include "NodeProgramGraphicsScene.hpp"
#include "NodeProgramGraphModel.hpp"
#include <QtNodes/GraphicsView>

JNodeProgramEditor::JNodeProgramEditor(NodeProgram & program)
    : _program(program)
{
    // Prepare the program by letting it know about
    // the editor.
    prepareProgram();

    
    layout = std::make_unique<QVBoxLayout>(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    _jbreadcrumbs = new JBreadcrumbs();
    layout->addWidget(_jbreadcrumbs);

    // If the program is non-trivial, we should
    // edit the 'main' graph.
    std::vector<NodeProgram::GraphId> graphs = _program.getGraphs();
    fprintf(stderr, "Size of graphs is %ld\n", graphs.size());
    if (graphs.size() > 0) {
	editGraph(graphs.at(0));
    }

}

void
JNodeProgramEditor::prepareProgram()
{
    for (const auto & graphId : _program.getGraphs()) {
	const NodeProgramGraphModel *graph = _program.getGraph(graphId);
	for (const auto & nodeId : graph->allNodeIds()) {
	    OpenSCADBuiltinModel *node = graph->delegateModel<OpenSCADBuiltinModel>(nodeId);
	    node->setEditor(this);
	}

	QObject::connect(
	    graph,
	    &NodeProgramGraphModel::nodeCreated,
	    [graphId, this](QtNodes::NodeId const nodeId) {
		const NodeProgramGraphModel *graph = this->_program.getGraph(graphId);
		OpenSCADBuiltinModel *node = graph->delegateModel<OpenSCADBuiltinModel>(nodeId);
		node->setEditor(this);
	    }
	);
    }
}


JNodeProgramEditor::~JNodeProgramEditor()
{}


void
JNodeProgramEditor::editGraph(std::string editGraph)
{
    NodeProgramGraphModel *graph = _program.getGraph(editGraph);
    if (graph == nullptr) {
	return;
    }
    auto scene = new NodeProgramGraphicsScene(*graph);
    auto view = new QtNodes::GraphicsView(scene);
    view->centerScene();
    _jbreadcrumbs->addPage(view);
}
