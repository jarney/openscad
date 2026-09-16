#pragma once

#include "nodes/OpenSCADDataTypes.hpp"

class NodeModelPort {
public:
    NodeModelPort(QtNodes::NodeDataType type, QString portCaption);
    NodeModelPort(QtNodes::NodeDataType type);
    virtual ~NodeModelPort();

    QtNodes::NodeDataType nodeDataType() const;
    QtNodes::ConnectionPolicy getConnectionPolicy() const;
    void setConnectionPolicy(QtNodes::ConnectionPolicy policy);
    
    QWidget *portWidget();
    void setData(std::shared_ptr<QtNodes::NodeData> data);
    std::shared_ptr<QtNodes::NodeData> getData(void) const;
    
    QString portCaption() const;
    bool portCaptionVisible() const;
private:
    QtNodes::NodeDataType _type;
    QString _caption;
    QtNodes::ConnectionPolicy _policy;
    std::shared_ptr<QtNodes::NodeData> _data;
};


