#pragma once

#include "nodes/gui/BreadcrumbsWidget.hpp"
#include "nodes/NodeProgram.hpp"

namespace QtNodes {
    class NodeGraphicsObject;
}

class JNodeProgramEditor : public QWidget {
public:
    JNodeProgramEditor(NodeProgram & program);
    ~JNodeProgramEditor();
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
    JBreadcrumbs *_jbreadcrumbs;
};

