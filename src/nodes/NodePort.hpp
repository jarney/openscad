#pragma once

#include <QtNodes/NodeData>
#include <QtNodes/Definitions>

namespace JNodes {
    namespace core {

class NodePort {
public:
    NodePort(QtNodes::NodeDataType type, QString portCaption);
    NodePort(QtNodes::NodeDataType type);
    virtual ~NodePort();

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


    } // End core
} // End JNodes
