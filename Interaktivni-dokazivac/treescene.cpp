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

    qDebug() << "****************NOOODEEEESSSSS****************";
    for(int i = 0; i<m_nodes.size(); i++)
        qDebug() << m_nodes[i];

    qDebug() << "****************parent nodes****************";
    for(int i = 0; i<m_nodes.size(); i++)
        qDebug() << ((Node*)m_nodes[i])->parentNode();

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


