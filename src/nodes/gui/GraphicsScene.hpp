#pragma once

#include <QtNodes/BasicGraphicsScene>
#include <QtNodes/internal/ConnectionGraphicsObject.hpp>
#include "nodes/NodeProgramGraphModel.hpp"
#include <QtNodes/internal/Export.hpp>
#include <QtNodes/internal/NodeConnectionInteraction.hpp>

/**
 * @brief An advanced scene working with data-propagating graphs.
 *
 * The class represents a scene that existed in v2.x but built wit the
 * new model-view approach in mind.
 */
class NODE_EDITOR_PUBLIC NodeProgramGraphicsScene : public QtNodes::BasicGraphicsScene
{
    Q_OBJECT
public:
    NodeProgramGraphicsScene(NodeProgramGraphModel &graphModel, QObject *parent = nullptr);
    ~NodeProgramGraphicsScene() = default;

public:
    std::vector<QtNodes::NodeId> selectedNodes() const;
    QMenu *createSceneMenu(QPointF const scenePos) override;
    void updateConnectionGraphics(const std::unordered_set<QtNodes::ConnectionId> &connections, bool state);

    QMenu *createGroupMenu(QPointF const scenePos, QtNodes::GroupGraphicsObject *groupGo);

public Q_SLOTS:
    bool save() const;
    bool load();

Q_SIGNALS:
    void sceneLoaded();

private:
    NodeProgramGraphModel &_graphModel;
};

