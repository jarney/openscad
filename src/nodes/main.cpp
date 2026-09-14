#include <Qsci/qsciscintilla.h>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <QtNodes/ConnectionStyle>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTabWidget>

#include <QtGui/QScreen>

#include "core/Builtins.h"

#include "OpenSCADModels.hpp"
#include "OpenSCADEvaluator.hpp"
#include "OpenSCADSerializer.hpp"
#include "NodeProgramGraphModel.hpp"
#include "NodeProgramModelRegistry.hpp"
#include "NodeProgramGraphicsScene.hpp"
#include "JBreadcrumbs.hpp"

#include "nodes/NodeModelPort.hpp"

using QtNodes::ConnectionStyle;
using QtNodes::GraphicsView;

static void setStyle()
{
    ConnectionStyle::setConnectionStyle(
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


int main_old(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setStyle();
    std::shared_ptr<NodeProgramModelRegistry> registry = SCADModels::registerDataModels();

    // TODO: We need more than one model
    // to handle scope, if, for, ...
//    models.push_back(NodeProgramGraphModel(registry));
    NodeProgramGraphModel dataFlowGraphModel(registry);

    QObject::connect(&dataFlowGraphModel,
		     &NodeProgramGraphModel::nodeCreated, [&dataFlowGraphModel](QtNodes::NodeId const nodeId) {
			 fprintf(stderr, "Node created %d\n", nodeId);
			 BaseSCADModel *model = dataFlowGraphModel.delegateModel<BaseSCADModel>(nodeId);
		     });

    // Register builtins...
    Builtins::initialize();

    QWidget mainWidget;

    auto menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("File");

    auto saveAction = menu->addAction("Save Scene");
    saveAction->setShortcut(QKeySequence::Save);

    auto loadAction = menu->addAction("Load Scene");
    loadAction->setShortcut(QKeySequence::Open);

    auto groupAction = menu->addAction("Create Group");
    groupAction->setShortcut(QKeySequence::Close);

    QVBoxLayout *l = new QVBoxLayout(&mainWidget);


    if (QFileInfo::exists("example.json")) {
	QJsonObject object = oscd_loadJson("example.json");
	dataFlowGraphModel.load(object);
    }

    
    l->addWidget(menuBar);
    auto scene = new NodeProgramGraphicsScene(dataFlowGraphModel, &mainWidget);

    auto qtab = new QTabWidget(&mainWidget);
    auto qtabLayout = new QVBoxLayout(qtab);
    l->addWidget(qtab);
    
    QString nodeName("Nodes");
    JBreadcrumbs *jw = new JBreadcrumbs();
    auto view = new GraphicsView(scene);
    jw->addPage(view);
    qtab->addTab(jw, nodeName);

    auto qsci = new QsciScintilla(qtab);
    QString sourceName("Source");
    qtab->addTab(qsci, sourceName);

    QObject::connect(qtab, &QTabWidget::currentChanged, [&dataFlowGraphModel, &qsci]() {
	std::string val = evaluateToSCAD(dataFlowGraphModel);
	qsci->setText(QString::fromStdString(val));
    });
    
    //l->addWidget(view);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    QObject::connect(saveAction, &QAction::triggered, scene, [scene, &dataFlowGraphModel]() {
	QJsonObject object = dataFlowGraphModel.save();
	oscd_saveJson(object, "example.json");
	evaluateToSCAD(dataFlowGraphModel);
    });

    QObject::connect(loadAction, &QAction::triggered, scene, &NodeProgramGraphicsScene::load);

    QObject::connect(groupAction, &QAction::triggered, scene, [scene, &dataFlowGraphModel]() {
	std::vector<QtNodes::NodeGraphicsObject*> groupNodes = ((QtNodes::BasicGraphicsScene*)scene)->selectedNodes();

	// We will need a few of this type.
	// A function (input parameters, output return-value)
	// A module (input parameters, output geometry)
	// A scope (group of nodes with 'let' statements to hold variable context)
	// A 'for' loop with a variable to indicate loop state and values, output geometry.

	// This is effectively a "function".
	auto arguments = std::make_unique<BaseSCADModel>("for", "Loop");
	arguments->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "range"), "range");
	arguments->addOutputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "index"), "index");
	arguments->setProcessor(SCADModels::f_prim_sphere_process);

	auto ret = std::make_unique<BaseSCADModel>("Return", "Return");
	ret->addInputPort(std::make_unique<NodeModelPort>(DATA_VARIABLE, "return"), "return");
	ret->setProcessor(SCADModels::f_prim_sphere_process);

	groupNodes.push_back(scene->nodeGraphicsObject(dataFlowGraphModel.addNode(std::move(arguments))));
	groupNodes.push_back(scene->nodeGraphicsObject(dataFlowGraphModel.addNode(std::move(ret))));

	auto nodeGroup = scene->createGroup(groupNodes, QString("Some Group"));
    });

    QObject::connect(scene, &NodeProgramGraphicsScene::sceneLoaded, view, &GraphicsView::centerScene);

    QObject::connect(scene, &NodeProgramGraphicsScene::modified, &mainWidget, [&mainWidget]() {
        mainWidget.setWindowModified(true);
    });

    if (scene->groupingEnabled()) {
        auto loadGroupAction = menu->addAction("Load Group...");
        QObject::connect(loadGroupAction, &QAction::triggered, [scene] { scene->loadGroupFile(); });
    }

    mainWidget.setWindowTitle("[*]Data Flow: simplest calculator");
    mainWidget.resize(800, 600);
    // Center window.
    mainWidget.move(QApplication::primaryScreen()->availableGeometry().center()
                    - mainWidget.rect().center());
    mainWidget.showNormal();

    return app.exec();
}
