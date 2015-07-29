#ifndef TREESCENE_H
#define TREESCENE_H

#include <QGraphicsScene>
#include "fol.hpp"
class TreeScene : public QGraphicsScene
{
    Q_OBJECT
public:
    TreeScene(QObject *parent);
};

#endif // TREESCENE_H
