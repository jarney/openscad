#include "NodeModelPort.hpp"

NodeModelPort::NodeModelPort(QtNodes::NodeDataType type, QString caption)
    : _type(type)
    , _caption(caption)
{}

NodeModelPort::NodeModelPort(QtNodes::NodeDataType type)
    : _type(type)
    , _caption("")
{}

NodeModelPort::~NodeModelPort()
{}

QtNodes::NodeDataType
NodeModelPort::nodeDataType() const
{ return _type; }

QWidget *
NodeModelPort::portWidget()
{ return nullptr; }

QString
NodeModelPort::portCaption() const
{ return _caption; }

bool
NodeModelPort::portCaptionVisible() const
{ return _caption.length() > 0; }

void
NodeModelPort::setData(std::shared_ptr<QtNodes::NodeData> data)
{ _data = data; }

std::shared_ptr<QtNodes::NodeData>
NodeModelPort::getData() const
{ return _data; }
