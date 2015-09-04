#include "treescene.h"
#include <QDebug>

TreeScene::TreeScene(QObject* parent)
    :QGraphicsScene(0,0,200,200,parent)
{
    ;
}


void TreeScene::addNode(Node *node)
{
    m_nodes.push_back(node);
    addItem(node);
}

void TreeScene::removeNode(QGraphicsItem *node)
{
    static int brojac = 0;
    qDebug() << "Node:  ";
    qDebug() << node;
QGraphicsItem* tmp;

    qDebug() << "****************parent nodes****************";
    for(int i = 0; i<m_nodes.size(); i++)
{
        qDebug() << "===============================================";
        qDebug() << "Node:   " << m_nodes[i];
        qDebug() << "Parent: " << ((Node*)m_nodes[i])->parentNode();
        qDebug() << "===============================================";
}
    for(int i = 0; i<m_nodes.size(); i++){
        if(((Node*)m_nodes[i])->parentNode() == node){
            qDebug() << "POZIVAM ZA: " << m_nodes[i];
            brojac++;
            tmp= m_nodes[i];
            removeNode(tmp);
            i=0;
        }
    }

    m_nodes.remove(m_nodes.indexOf(node));
    removeItem(node);


    qDebug () << "BROJAC:  " << brojac;

}


