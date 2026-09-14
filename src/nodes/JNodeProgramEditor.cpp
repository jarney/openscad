#include "JNodeProgramEditor.hpp"
#include "OpenSCADModels.hpp"

#include <QtNodes/DataFlowGraphicsScene>
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
    if (graphs.size() > 0) {
	editGraph(graphs.at(0));
    }

}

void
JNodeProgramEditor::prepareProgram()
{
    for (const auto & graphId : _program.getGraphs()) {
	const OpenSCADGraphModel *graph = _program.getGraph(graphId);
	for (const auto & nodeId : graph->allNodeIds()) {
	    BaseSCADModel *node = graph->delegateModel<BaseSCADModel>(nodeId);
	    node->setEditor(this);
	}
    }
}


JNodeProgramEditor::~JNodeProgramEditor()
{}


void
JNodeProgramEditor::editGraph(std::string editGraph)
{
    OpenSCADGraphModel *graph = _program.getGraph(editGraph);
    if (graph == nullptr) {
	return;
    }
    auto scene = new QtNodes::DataFlowGraphicsScene(*graph);
    auto view = new QtNodes::GraphicsView(scene);
    view->centerScene();
    _jbreadcrumbs->addPage(view);
}
