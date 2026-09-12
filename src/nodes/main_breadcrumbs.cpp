#include <Qsci/qsciscintilla.h>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QPushButton>

#include <QtGui/QScreen>

#include "JBreadcrumbs.hpp"

int main_breadcrumbs(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget mainWidget;

    auto menuBar = new QMenuBar();
    QMenu *menu = menuBar->addMenu("File");

    auto pushAction = menu->addAction("Add Crumb");
    auto popAction = menu->addAction("Pop Crumb");

    QVBoxLayout *l = new QVBoxLayout(&mainWidget);

    l->addWidget(menuBar);
    JBreadcrumbs *jw = new JBreadcrumbs();
    jw->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    l->addWidget(jw);

    jw->addPage(new QsciScintilla(jw));
    
    QObject::connect(pushAction, &QAction::triggered, [&jw]() {
	jw->addPage(new QsciScintilla(jw));
    });
    QObject::connect(popAction, &QAction::triggered, [&jw]() {
	jw->removePage(0);
    });

    
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(0);

    mainWidget.setWindowTitle("[*]Data Flow: simplest calculator");
    mainWidget.resize(800, 600);
    // Center window.
    mainWidget.move(QApplication::primaryScreen()->availableGeometry().center()
                    - mainWidget.rect().center());
    mainWidget.showNormal();

    return app.exec();
}
