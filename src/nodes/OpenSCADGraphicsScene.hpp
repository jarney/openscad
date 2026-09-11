#pragma once

#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/ConnectionGraphicsObject>
#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/Export>
#include <QtNodes/NodeConnectionInteraction>

/**
 * @brief An advanced scene working with data-propagating graphs.
 *
 * The class represents a scene that existed in v2.x but built wit the
 * new model-view approach in mind.
 */
class OpenSCADGraphicsScene : public BasicGraphicsScene
{
    Q_OBJECT
public:
    OpenSCADGraphicsScene(DataFlowGraphModel &graphModel, QObject *parent = nullptr);
    ~OpenSCADGraphicsScene() = default;

public:
    std::vector<NodeId> selectedNodes() const;
    QMenu *createSceneMenu(QPointF const scenePos) override;
    void updateConnectionGraphics(const std::unordered_set<ConnectionId> &connections, bool state);
    void setGraphModel(DataFlowGraphModel & model);
												  
Q_SIGNALS:
    void sceneLoaded();

private:
    DataFlowGraphModel &_graphModel;
};
