#ifndef TREESCENE_H
#define TREESCENE_H

#include <QGraphicsScene>
#include "fol.hpp"
#include "node.h"
class TreeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    TreeScene(QObject *parent);
    void addNode(Node* node);
    void removeNode(QGraphicsItem* node);
private:
    QVector<QGraphicsItem*> m_nodes;
};

#endif // TREESCENE_H
