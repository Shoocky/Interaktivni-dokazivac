#include "treescene.h"
#include <QDebug>

TreeScene::TreeScene(QObject* parent)
    :QGraphicsScene(0,0,200,200,parent)
{
}


void TreeScene::addNode(Node *node)
{
    m_nodes.push_back(node);
    addItem(node);
}

void TreeScene::removeNode(QGraphicsItem *node)
{
    qDebug() << "******** 111111111111111 *******";
    static int brojac = 0;
    qDebug() << "Node:  ";
    qDebug() << node;
QGraphicsItem* tmp;
//m_nodes.remove(m_nodes.indexOf(node));
//removeItem(node);
    /*
    qDebug() << "****************NOOODEEEESSSSS****************";
    for(int i = 0; i<m_nodes.size(); i++)
        qDebug() << m_nodes[i];
*/
    qDebug() << "****************parent nodes****************";
    for(int i = 0; i<m_nodes.size(); i++)
{
        qDebug() << "===============================================";
        qDebug() << "Node:   " << m_nodes[i];
        qDebug() << "Parent: " << ((Node*)m_nodes[i])->parentNode();
        qDebug() << "===============================================";
}
    qDebug() << "*******USOOO";

    qDebug() << "******** 222222222222 *******";
    for(int i = 0; i<m_nodes.size(); i++){
        if(((Node*)m_nodes[i])->parentNode() == node){
            qDebug() << "POZIVAM ZA: " << m_nodes[i];
            brojac++;
            tmp= m_nodes[i];
            removeNode(tmp);
            i=0;
        }
    }

    qDebug() << "******* 333333333333333 *******";
    m_nodes.remove(m_nodes.indexOf(node));
    qDebug() << "******** 4444444444444444 *******";
    removeItem(node);


    qDebug () << "BROJAC:  " << brojac;
    /*
    for(int i = 0; i < m_nodes.size(); i++){
        if(m_nodes[i] == node){
                continue;
        }
        if(((Node*)m_nodes[i])->parentNode() == node){
            removeNode(m_nodes[i]);
        }
    }

    m_nodes.remove(m_nodes.indexOf(node));
    removeItem(node);
    */
}


