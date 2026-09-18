#pragma once

#include "nodes/gui/BreadcrumbsWidget.hpp"
#include "nodes/NodeProgram.hpp"

namespace QtNodes {
    class NodeGraphicsObject;
}

namespace JNodes {
    namespace gui {

class NodeEditorWidget : public QWidget {
public:
    NodeEditorWidget(NodeProgram & program);
    ~NodeEditorWidget();
    void editGraph(std::string editGraph);

    std::vector<QtNodes::NodeGraphicsObject*> selectedNodes();
    void createGroup(std::vector<QtNodes::NodeGraphicsObject*> & groupNodes, QString name);
    
    static void initializeStyles();
private:
    /**
     * This connects the program to the editor
     * widget so it can navigate on its behalf.
     */
    void prepareProgram();
    
    NodeProgram &_program;
    std::unique_ptr<QVBoxLayout> layout;
    BreadcrumbsWidget *_jbreadcrumbs;
};


    } // End gui
} // End JNodes 
