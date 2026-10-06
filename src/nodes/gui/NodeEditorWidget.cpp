#include <QtNodes/GraphicsView>
#include <QtNodes/ConnectionStyle>

#include "node--js/Node.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeModule.hpp"

#include "nodes/gui/NodeEditorWidget.hpp"
#include "nodes/gui/GraphicsScene.hpp"

using namespace NodeJS::gui;
using namespace NodeJS::core;

NodeEditorWidget::NodeEditorWidget(NodeModule & program)
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
    const std::map<NodeModule::GraphId, std::unique_ptr<NodeGraph>> & graphs = _program.getGraphs();
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
#if 0
    for (const auto & graphIt : _program.getGraphs()) {
	const NodeGraph *graph = graphIt.second.get();
	for (const auto & nodeId : graph->allNodeIds()) {
	    Node *node = graph->delegateModel<Node>(nodeId);
	    node->setEditor(this);
	}

	QObject::connect(
	    graph,
	    &NodeGraph::nodeCreated,
	    [graphId, this](QtNodes::NodeId const nodeId) {
		const NodeGraph *graph = this->_program.getGraph(graphId);
		Node *node = graph->delegateModel<Node>(nodeId);
		node->setEditor(this);
	    }
	);
    }
#endif
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
    std::unique_ptr<GraphModelAdapter> model = std::make_unique<GraphModelAdapter>(*graph);
    auto scene = new GraphicsScene(std::move(model));

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

