#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/DataFlowGraphModel>
#include "nodes/OpenSCADDataTypes.hpp"

class OpenSCADGraphModel : public QtNodes::DataFlowGraphModel {
public:
    OpenSCADGraphModel(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry);
    ~OpenSCADGraphModel();
    
    bool dataTypeConnectionAllowed(const QtNodes::NodeDataType & outType, const QtNodes::NodeDataType & inType) const;
};
