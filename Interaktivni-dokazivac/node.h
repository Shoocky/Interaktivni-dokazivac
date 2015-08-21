#ifndef NODE_H
#define NODE_H
#include <QPainter>
#include <QGraphicsItem>
#include "fol.hpp"

class Node : public QGraphicsItem
{

public:
    Node(const Formula & formula, qreal, qreal, int, int, QGraphicsItem* parent_node = nullptr);
    Node(const Formula & formula, qreal width, qreal height, int x, int y, QGraphicsItem* parent_node, QVector<Formula> assumptions);


    // QGraphicsItem interface
public:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString getText() const;
    qreal getx() const;
    qreal gety() const;
    const Formula &getFormula() const;
    QGraphicsItem* parentNode() const;
    QVector<Formula>& getAssumptions(){ return m_assumptions;}
    bool checkAssumption() const;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private:
    QGraphicsItem * m_parent;
    Formula m_formula;
    QVector<Formula> m_assumptions;
    QString text;
    qreal rect_width;
    qreal rect_height;
    int rect_x;
    int rect_y;
    bool pressed;
};

#endif // NODE_H
