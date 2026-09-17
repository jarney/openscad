#include "JNodeProgramEditor.hpp"
#include "nodes/OpenSCADBuiltinModel.hpp"

#include "NodeProgramGraphicsScene.hpp"
#include "NodeProgramGraphModel.hpp"
#include <QtNodes/GraphicsView>
#include <QtNodes/ConnectionStyle>

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
	// Somehow this we should designate a main
	// graph through the program itself
	// presumably through some metadata.
	editGraph("main");
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


// So what if, actually, we encode the graph inside the 'for' node
// and do it recursively instead of making the graphs as peers?
// For module-call, we would still link to an external module statement,
// but the module statement itself would contain another flow, so that
// actually feels pretty natural.  Loading a module node defines the flow...
void
JNodeProgramEditor::editGraph(std::string editGraph)
{
    NodeProgramGraphModel *graph = _program.getGraph(editGraph);
    if (graph == nullptr) {
	return;
    }
    auto scene = new NodeProgramGraphicsScene(*graph);

    // Load up the groups.
    
    auto view = new QtNodes::GraphicsView(scene);
    view->centerScene();
    _jbreadcrumbs->addPage(view);
}

std::vector<QtNodes::NodeGraphicsObject*>
JNodeProgramEditor::selectedNodes()
{
    QtNodes::GraphicsView *view = (QtNodes::GraphicsView *)_jbreadcrumbs->getPage();
    QtNodes::BasicGraphicsScene *scene = view->getNodeScene();
    return scene->selectedNodes();
}

void
JNodeProgramEditor::createGroup(std::vector<QtNodes::NodeGraphicsObject*> & groupNodes, QString name)
{
    QtNodes::GraphicsView *view = (QtNodes::GraphicsView *)_jbreadcrumbs->getPage();
    QtNodes::BasicGraphicsScene *scene = view->getNodeScene();
    scene->createGroup(groupNodes, name);
}


void
JNodeProgramEditor::initializeStyles()
{
    QtNodes::ConnectionStyle::setConnectionStyle(
        R"(
  {
    "ConnectionStyle": {
      "ConstructionColor": "gray",
      "NormalColor": "black",
      "SelectedColor": "gray",
      "SelectedHaloColor": "deepskyblue",
      "HoveredColor": "deepskyblue",

      "LineWidth": 3.0,
      "ConstructionLineWidth": 2.0,
      "PointDiameter": 10.0,

      "UseDataDefinedColors": true
    }
  }
  )");
}

