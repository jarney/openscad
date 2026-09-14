#pragma once

#include "JBreadcrumbs.hpp"
#include "NodeProgram.hpp"

class JNodeProgramEditor : public QWidget {
public:
    JNodeProgramEditor(NodeProgram & program);
    ~JNodeProgramEditor();
    void editGraph(std::string editGraph);
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

