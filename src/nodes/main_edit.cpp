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
#include <iostream>

#include "node--js/NodeModule.hpp"
#include "node--js/xml/SerializerXML.hpp"
#include "node--js/xml/ModuleLoaderNodeJSPath.hpp"
#include "nodes/gui/NodeEditorWidget.hpp"

using QtNodes::GraphicsView;

using namespace NodeJS::core;
using namespace NodeJS::xml;
using namespace NodeJS::gui;

int main_edit(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (argc != 2) {
	fprintf(stderr, "Usage: edit filename\n");
	return 1;
    }
    
    ModuleLoaderNodeJSPath loader;
    loader.setNODEJS_PATH("../submodules/node--js/test-data;.");
    SerializerErrorReporterStream err(std::cerr);
    NodeModule *loaded = loader.loadModule(argv[1], err);
    if (loaded == nullptr) {
	fprintf(stderr, "Could not find module %s in NODEJS_PATH\n", argv[1]);
	return 2;
    }
    NodeModule & nodeModule = *loaded;

    // Register builtins...
    NodeJS::gui::NodeEditorWidget::initializeStyles();

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

    NodeEditorWidget *jw = new NodeEditorWidget(nodeModule);
    qtab->addTab(jw, "Nodes");

    auto qsci = new QsciScintilla(qtab);
    qtab->addTab(qsci, "Source");

    QObject::connect(qtab, &QTabWidget::currentChanged, [&nodeModule, &qsci]() {
#if 0
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ostringstream output;
	serializer.write(program, output);
	qsci->setText(QString::fromStdString(output.str()));
#endif
    });


    QObject::connect(saveAction, &QAction::triggered, [&nodeModule, argv]() {
#if 0
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ofstream output(argv[1]);
	serializer.write(program, output);
#endif
    });

    // This is a hot mess, but fortunately we should not really
    // need to do this in the final product.
    QObject::connect(loadAction, &QAction::triggered, [qtab, &nodeModule, argv]() {
#if 0
	qtab->removeTab(0);
	const NodeProgramSerializer & serializer = NodeProgramSerializerJSON::instance();
	std::ifstream input(argv[1]);
	serializer.read(program, input);
	NodeEditorWidget *jw = new NodeEditorWidget(program);
	qtab->insertTab(0, jw, "Nodes-");
	qtab->setTabVisible(0, true);
#endif
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
