#include <QtNodes/GraphicsView>
#include <QtNodes/ConnectionStyle>

#include "nodes/OpenSCADBuiltinModel.hpp"
#include "nodes/NodeGraph.hpp"

#include "nodes/gui/NodeEditorWidget.hpp"
#include "nodes/gui/GraphicsScene.hpp"

using namespace JNodes::gui;
using namespace JNodes::core;

NodeEditorWidget::NodeEditorWidget(NodeProgram & program)
    : _program(program)
{
    // Prepare the program by letting it know about
    // the editor.
    prepareProgram();

    
    layout = std::make_unique<QVBoxLayout>(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    _jbreadcrumbs = new BreadcrumbsWidget();
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
NodeEditorWidget::prepareProgram()
{
    for (const auto & graphId : _program.getGraphs()) {
	const NodeGraph *graph = _program.getGraph(graphId);
	for (const auto & nodeId : graph->allNodeIds()) {
	    OpenSCADBuiltinModel *node = graph->delegateModel<OpenSCADBuiltinModel>(nodeId);
	    node->setEditor(this);
	}

	QObject::connect(
	    graph,
	    &NodeGraph::nodeCreated,
	    [graphId, this](QtNodes::NodeId const nodeId) {
		const NodeGraph *graph = this->_program.getGraph(graphId);
		OpenSCADBuiltinModel *node = graph->delegateModel<OpenSCADBuiltinModel>(nodeId);
		node->setEditor(this);
	    }
	);
    }
}


NodeEditorWidget::~NodeEditorWidget()
{}


void
NodeEditorWidget::editGraph(std::string editGraph)
{
    NodeGraph *graph = _program.getGraph(editGraph);
    if (graph == nullptr) {
	return;
    }
    auto scene = new GraphicsScene(*graph);

    // Load up the groups.
    
    auto view = new QtNodes::GraphicsView(scene);
    view->centerScene();
    _jbreadcrumbs->addPage(view);
}

std::vector<QtNodes::NodeGraphicsObject*>
NodeEditorWidget::selectedNodes()
{
    QtNodes::GraphicsView *view = (QtNodes::GraphicsView *)_jbreadcrumbs->getPage();
    QtNodes::BasicGraphicsScene *scene = view->getNodeScene();
    return scene->selectedNodes();
}

void
NodeEditorWidget::createGroup(std::vector<QtNodes::NodeGraphicsObject*> & groupNodes, QString name)
{
    QtNodes::GraphicsView *view = (QtNodes::GraphicsView *)_jbreadcrumbs->getPage();
    QtNodes::BasicGraphicsScene *scene = view->getNodeScene();
    scene->createGroup(groupNodes, name);
}


void
NodeEditorWidget::initializeStyles()
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

