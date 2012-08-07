#include <math.h>
#include "graphscene.h"

#include "node.h"
#include "link.h"

const double Pi = 3.14;

GraphScene::GraphScene(QObject *parent) :
    QGraphicsScene(parent)
{
    leftButtonDown = 0;
    numNodes = 0;
    curMode = MoveItem;
}


void GraphScene::setMode(Mode mode)
{
    curMode = mode;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() != Qt::LeftButton)
        return;
    Node *node;
    switch(curMode) {
        case InsertNode:
            node = new Node (++numNodes);
            addItem(node);
            node->setPos(mouseEvent->scenePos());
            break;
        case InsertLine:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            line->setPen(QPen(Qt::black, 2));
            addItem(line);
            break;
        case MoveItem:
            break;
        default:
            qDebug() << "Invalid mode!"; break;

    }

    QGraphicsScene::mousePressEvent(mouseEvent);
}


void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (curMode == InsertLine && line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }   else if(curMode == MoveItem) {
        //qDebug() << sceneRect().contains(mouseEvent->scenePos());
        if (!sceneRect().contains(mouseEvent->scenePos())) {
            mouseEvent->ignore();
            return;
        }
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && curMode == InsertLine) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if(startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if(endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if(startItems.count() > 0 && endItems.count() > 0 &&
           startItems.first() != endItems.first()) {
            Node *startNode = qgraphicsitem_cast<Node *>(startItems.first());
            Node *endNode = qgraphicsitem_cast<Node *>(endItems.first());
            Link *link = new Link(startNode, endNode);
            startNode->addLink(link);
            endNode->addLink(link);
            link->setZValue(-1000.0);
            addItem(link);
            link->updatePosition();
        }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::insertNodes(int num)
{
    int xc = 80, yc = 80, xone = 80, yone = 20;
    //the radius will be longer if there are many vertexes
    for(int i = 10; i < num; i += 10){
        xc += 30;
        yc += 30;
    }
    for(int i = 0; i < num; ++i) {
        Node *node = new Node (++numNodes);
        addItem(node);
        //node->setPos(xc+(xone-xc)*cos(2*Pi*i/num)+(yone-yc)*sin(2*Pi*i/num), yc+(xc-xone)*sin(2*Pi*i/num)+(yone-yc)*cos(2*Pi*i/num));
        node->setPos(xc+(xone-xc)*sin(2*Pi*i/num)+(yone-yc)*cos(2*Pi*i/num), yc+(xc-xone)*cos(2*Pi*i/num)+(yone-yc)*sin(2*Pi*i/num));
    }
}

void GraphScene::clearScene()
{

    //removing all links from the scene
    deleteAllLinks();

    //removing all nodes from the scene
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == Node::Type) {
            Node *nd = qgraphicsitem_cast<Node *>(item);
            nd->removeLinks();
            removeItem(item);
            --numNodes;
            //nodeDeleted();
            delete item;
        }
    }
}

void GraphScene::deleteSelectedItems()
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == Link::Type) {
            removeItem(item);
            Link *link = qgraphicsitem_cast<Link *>(item);
            link->getStartNode()->removeLink(link);
            link->getEndNode()->removeLink(link);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == Node::Type) {
            Node *nd = qgraphicsitem_cast<Node *>(item);
            nd->removeLinks();
            int num = nd->getNumber();
            foreach (QGraphicsItem *it, items()) {
                if(it->type() == Node::Type) {
                    Node *n = qgraphicsitem_cast<Node *>(it);
                    if(n->getNumber() > num)
                        n->setNumber(n->getNumber() - 1);
                    n->update();

                }
            }
            removeItem(item);
            nodeDeleted();
            delete item;
        }
    }
}

QList<Node*> GraphScene::getAllNodes() const
{
    QList<Node*> ls;
    QList<QGraphicsItem *> gl = items();
    for(int i = 0; i < gl.count(); ++i){
        Node *n = qgraphicsitem_cast<Node *> (gl.at(i));
        if(n)
            ls.insert(i, n);
    }
    return ls;
}

void GraphScene::deleteAllLinks()
{
    foreach (QGraphicsItem *item, items()) {
        if (item->type() == Link::Type) {
            removeItem(item);
            Link *link = qgraphicsitem_cast<Link *>(item);
            link->getStartNode()->removeLink(link);
            link->getEndNode()->removeLink(link);
            delete item;
        }
    }
}
