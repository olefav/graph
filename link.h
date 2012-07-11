#ifndef LINK_H
#define LINK_H

#include <QGraphicsLineItem>
#include <QtGui>
#include "node.h"

class Link : public QGraphicsLineItem
{
public:
    enum {Type = UserType + 100};

    Link(Node *startNode_f, Node *endNode_f, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~Link();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    Node* getStartNode() {return startNode;}
    Node* getEndNode() {return endNode;}
    void updatePosition();

    int type() const {return Type; }

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    Node *startNode;
    Node *endNode;
    QColor color;
    QPolygonF arrowHead;
};

#endif // LINK_H
