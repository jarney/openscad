#include "NodePort.hpp"

using namespace JNodes::core;

NodePort::NodePort(QtNodes::NodeDataType type, QString caption)
    : _type(type)
    , _caption(caption)
    , _policy(QtNodes::ConnectionPolicy::One)
{
}

NodePort::NodePort(QtNodes::NodeDataType type)
    : _type(type)
    , _caption("")
{}

NodePort::~NodePort()
{}

QtNodes::NodeDataType
NodePort::nodeDataType() const
{ return _type; }

QtNodes::ConnectionPolicy
NodePort::getConnectionPolicy() const
{ return _policy; }

void
NodePort::setConnectionPolicy(QtNodes::ConnectionPolicy policy)
{ _policy = policy; }

QWidget *
NodePort::portWidget()
{ return nullptr; }

QString
NodePort::portCaption() const
{ return _caption; }

bool
NodePort::portCaptionVisible() const
{ return _caption.length() > 0; }

void
NodePort::setData(std::shared_ptr<QtNodes::NodeData> data)
{ _data = data; }

std::shared_ptr<QtNodes::NodeData>
NodePort::getData() const
{ return _data; }
