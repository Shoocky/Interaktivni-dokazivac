#include "node.h"
#include <QGraphicsSceneMouseEvent>
#include <qdebug.h>
#include <sstream>
#include <ostream>
#include <string>

Node::Node(const QString str, const Formula & formula, const qreal width, const qreal height,const  int x, const int y)
{
    std::ostringstream stream;
    formula->printFormula(stream);

    text = QString::fromStdString(stream.str());
    qDebug() << "Node(): " + text;
    rect_width = width;
    rect_height = height;
    rect_x = x;
    rect_y = y;
    pressed = false;
    this->setFlag(QGraphicsItem::ItemIsSelectable);
}

QRectF Node::boundingRect() const
{
    return QRectF(rect_x, rect_y, rect_width, rect_height);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(pressed){
        QRectF rect = boundingRect();
        QPen pen(Qt::red, 1);
        painter->setPen(pen);
        painter->drawRect(rect);
        painter->drawText(rect, text);
    }
    else{
        QRectF rect = boundingRect();
        QPen pen(Qt::black, 1);
        painter->setPen(pen);
        painter->drawRect(rect);
        painter->drawText(rect, text);
    }
    if(isSelected()){

        qDebug() << "Selektovan";
    }
}

QString Node::getText() const
{
    return text;
}

qreal Node::getx() const
{
    return rect_x;
}
qreal Node::gety() const
{
    return rect_y;
}
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::RightButton) {
            this->setSelected(true);
            update();
        }
    else
    if(pressed){
        pressed = false;
        update();
        this->setSelected(false);
        QGraphicsItem::mousePressEvent(event);
    }
    else{
        pressed = true;
        this->setSelected(true);
        update();
        QGraphicsItem::mousePressEvent(event);
    }
}



