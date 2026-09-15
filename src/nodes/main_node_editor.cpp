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
#include <fstream>

#include "core/Builtins.h"

#include "OpenSCADBuiltins.hpp"
#include "OpenSCADEvaluator.hpp"
#include "NodeProgramGraphModel.hpp"
#include "JBreadcrumbs.hpp"
#include "JNodeProgramEditor.hpp"
#include "NodeProgram.hpp"
#include "NodeProgramSerializer.hpp"
#include "NodeProgramModelRegistry.hpp"
#include "NodeProgramGraphicsScene.hpp"

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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setStyle();
    std::shared_ptr<NodeProgramModelRegistry> registry = OpenSCADBuiltins::registerDataModels();
    
    NodeProgram program(registry);
    
    if (QFileInfo::exists("../example.json")) {
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ifstream exampleInputFile("../example.json");
	serializer.read(program, exampleInputFile);
	fprintf(stderr, "Finished reading\n");
    }

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
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);
    l->addWidget(menuBar);

    auto qtab = new QTabWidget(&mainWidget);
    auto qtabLayout = new QVBoxLayout(qtab);
    l->addWidget(qtab);
    
    JNodeProgramEditor *jw = new JNodeProgramEditor(program);
    qtab->addTab(jw, "Nodes");

    auto qsci = new QsciScintilla(qtab);
    qtab->addTab(qsci, "Source");

    QObject::connect(qtab, &QTabWidget::currentChanged, [&program, &qsci]() {
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ostringstream output;
	serializer.write(program, output);
	qsci->setText(QString::fromStdString(output.str()));
    });


    QObject::connect(saveAction, &QAction::triggered, [&program]() {
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ofstream output("../example.json");
	serializer.write(program, output);
    });

    // This is a hot mess, but fortunately we should not really
    // need to do this in the final product.
    QObject::connect(loadAction, &QAction::triggered, [qtab, &program]() {
	qtab->removeTab(0);
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ifstream input("../example.json");
	serializer.read(program, input);
	JNodeProgramEditor *jw = new JNodeProgramEditor(program);
	qtab->insertTab(0, jw, "Nodes-");
	qtab->setTabVisible(0, true);
    });

#if 0
    QObject::connect(groupAction, &QAction::triggered, scene, [scene, &dataFlowGraphModel]() {
	std::vector<QtNodes::NodeGraphicsObject*> groupNodes = ((QtNodes::BasicGraphicsScene*)scene)->selectedNodes();

	// We will need a few of this type.
	// A function (input parameters, output return-value)
	// A module (input parameters, output geometry)
	// A scope (group of nodes with 'let' statements to hold variable context)
	// A 'for' loop with a variable to indicate loop state and values, output geometry.

	// This is effectively a "function".
	auto arguments = std::make_unique<BaseSCADModel>("for", "Loop");
	arguments->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "range"), "range");
	arguments->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "index"), "index");
	arguments->setProcessor(OpenSCADBuiltins::f_prim_sphere_process);

	auto ret = std::make_unique<BaseSCADModel>("Return", "Return");
	ret->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "return"), "return");
	ret->setProcessor(OpenSCADBuiltins::f_prim_sphere_process);

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
#endif
    fprintf(stderr, "Going to run the window\n");
    
    mainWidget.setWindowTitle("[*]Data Flow: simplest calculator");
    mainWidget.resize(800, 600);
    // Center window.
    mainWidget.move(QApplication::primaryScreen()->availableGeometry().center()
                    - mainWidget.rect().center());
    mainWidget.showNormal();

    return app.exec();
}
