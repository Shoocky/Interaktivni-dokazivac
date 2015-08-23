#include "node.h"
#include <QGraphicsSceneMouseEvent>
#include <qdebug.h>
#include <sstream>
#include <ostream>
#include <string>

Node::Node(const Formula & formula, qreal width, qreal height, const  int x, const int y, QGraphicsItem *parent_node)
{
    std::ostringstream stream;
    formula->printFormula(stream);
    m_assumptions = QVector<Formula>();
    m_formula = formula;
    text = QString::fromStdString(stream.str());
    qDebug() << "Node(): " + text;
    rect_width = width;
    rect_height = height;
    rect_x = x;
    rect_y = y;
    pressed = false;
    m_parent = parent_node;
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);

}

Node::Node(const Formula &formula, qreal width, qreal height, int x, int y, QGraphicsItem *parent_node, QVector<Formula> assumptions)
    :Node(formula, width, height, x, y, parent_node)
{
    std::ostringstream stream;
    m_formula->printFormula(stream);
    qDebug() << "Node added: " << QString::fromStdString(stream.str());
    m_assumptions = assumptions;
    if(checkAssumption()){
           rect_width = 35;
            text = "[";
            text += QString::fromStdString(stream.str());
            text += "]";
            rect_width = text.length()*11;
    }
}


QRectF Node::boundingRect() const
{
    return QRectF(rect_x, rect_y, rect_width, rect_height);
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if(isSelected()){
        QRectF rect = boundingRect();
        QPen pen(Qt::red, 1);
        painter->setPen(pen);
        if(text != "Done")
            painter->drawLine(rect.topLeft(),rect.topRight());
        //painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, text);
    }
    else{
        QRectF rect = boundingRect();
        QPen pen(Qt::black, 1);
        painter->setPen(pen);
        if(text != "Done")
            painter->drawLine(rect.topLeft(),rect.topRight());
        //painter->drawRect(rect);
        painter->drawText(rect, Qt::AlignCenter, text);
    }
    if(isSelected()){

        qDebug() << "Selektovan";
    }
}

QString Node::getText() const
{
    return text;
}
qreal Node::getWidth() const{
    return rect_width;
}

qreal Node::getx() const
{
    return sceneBoundingRect().x();
}
qreal Node::gety() const
{
    return sceneBoundingRect().y();
}

const Formula& Node::getFormula() const
{
    return m_formula;
}

QGraphicsItem *Node::parentNode() const
{
    return m_parent;
}

bool Node::checkAssumption() const
{
    for(auto it = m_assumptions.begin(); it != m_assumptions.end(); it++){
        if(m_formula->equalTo(*it)){
            return true;
        }
    }
    return false;
}
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /*
    if (event->button() == Qt::MouseButton::LeftButton) {
            qDebug() << "node: mouse press";
            bool selected = isSelected();
            setSelected(!selected);
            isSelected() ? qDebug() << "selektovan je" : qDebug() << "nije selektovan";
            update();
        }
        */
}

QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange){
        qDebug() << "item changed";

        return QPointF(value.toPointF().x(), pos().y());
    }
    return QGraphicsItem::itemChange(change,value);

}



