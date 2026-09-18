#include <Qsci/qsciscintilla.h>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

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

#include "nodes/NodeProgram.hpp"
#include "nodes/NodeProgramGraphModel.hpp"
#include "nodes/NodeProgramSerializer.hpp"
#include "nodes/NodeProgramModelRegistry.hpp"
#include "nodes/gui/GraphicsScene.hpp"
#include "nodes/gui/NodeEditorWidget.hpp"

#include "nodes/openscad/Builtins.hpp"
#include "nodes/OpenSCADEvaluator.hpp"

using QtNodes::GraphicsView;

using namespace JNodes::core;

int main_edit(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc != 2) {
	fprintf(stderr, "Usage: edit filename\n");
	return 1;
    }

    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "File %s does not exist\n", argv[1]);
	fprintf(stderr, "Usage: edit filename\n");
	return 2;
    }
    
    std::shared_ptr<NodeProgramModelRegistry> registry = JNodes::openscad::Builtins::registerDataModels();
    NodeProgram program(registry);
    
    const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
    std::ifstream exampleInputFile(argv[1]);
    serializer.read(program, exampleInputFile);

    // Register builtins...
    Builtins::initialize();
    JNodes::gui::NodeEditorWidget::initializeStyles();

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

    JNodes::gui::NodeEditorWidget *jw = new JNodes::gui::NodeEditorWidget(program);
    qtab->addTab(jw, "Nodes");

    auto qsci = new QsciScintilla(qtab);
    qtab->addTab(qsci, "Source");

    QObject::connect(qtab, &QTabWidget::currentChanged, [&program, &qsci]() {
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ostringstream output;
	serializer.write(program, output);
	qsci->setText(QString::fromStdString(output.str()));
    });


    QObject::connect(saveAction, &QAction::triggered, [&program, argv]() {
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ofstream output(argv[1]);
	serializer.write(program, output);
    });

    // This is a hot mess, but fortunately we should not really
    // need to do this in the final product.
    QObject::connect(loadAction, &QAction::triggered, [qtab, &program, argv]() {
	qtab->removeTab(0);
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ifstream input(argv[1]);
	serializer.read(program, input);
	JNodes::gui::NodeEditorWidget *jw = new JNodes::gui::NodeEditorWidget(program);
	qtab->insertTab(0, jw, "Nodes-");
	qtab->setTabVisible(0, true);
    });

    QObject::connect(groupAction, &QAction::triggered, [jw]() {
	std::vector<QtNodes::NodeGraphicsObject*> groupNodes = jw->selectedNodes();
	jw->createGroup(groupNodes, QString("Some Group"));
    });
    
    mainWidget.setWindowTitle("[*]Data Flow: simplest calculator");
    mainWidget.resize(800, 600);
    // Center window.
    mainWidget.move(QApplication::primaryScreen()->availableGeometry().center()
                    - mainWidget.rect().center());
    mainWidget.showNormal();

    return app.exec();
}
