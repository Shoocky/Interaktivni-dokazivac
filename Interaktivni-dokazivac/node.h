#ifndef NODE_H
#define NODE_H
#include <QPainter>
#include <QGraphicsItem>
#include "fol.hpp"

class Node : public QGraphicsItem
{

public:
    Node(QString text, const Formula & formula, qreal, qreal, int, int, QGraphicsItem* parent_node = nullptr);


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getText() const;
    qreal getx() const;
    qreal gety() const;
    QGraphicsItem* parentNode() const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    QGraphicsItem * m_parent;
    Formula m_formula;
    QString text;
    qreal rect_width;
    qreal rect_height;
    int rect_x;
    int rect_y;
    bool pressed;
};

#endif // NODE_H
