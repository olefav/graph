#include <QtGui>
#include "node.h"

#include "link.h"

Node::Node(int f_number) :
    number(f_number), bckColor(Qt::white)
{
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);
}

Node::~Node()
{
    foreach (Link *link, links)
        delete link;
}

int Node::getNumber() const
{
    return number;
}

QColor Node::getColor() const
{
    return bckColor;
}

void Node::setColor(QColor color)
{
    bckColor = color;
}


void Node::addLink(Link *link)
{
    links.insert(link);
}

void Node::removeLink(Link *link)
{
    links.remove(link);
}

void Node::removeLinks()
{
    foreach (Link *link, links) {
        link->getStartNode()->removeLink(link);
        link->getEndNode()->removeLink(link);
        scene()->removeItem(link);
        delete link;
    }
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
            //qDebug() << "Pos changed...";
            foreach (Link *link, links)
                link->updatePosition();
        }
    return QGraphicsItem::itemChange(change, value);
}

QRectF Node::boundingRect() const
{
    const int margin = 1;
    return outerRect().adjusted(-margin, -margin, +margin+2, +margin+2); // +2 for shadow(??)
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(outerRect());
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;
    if (option->state & QStyle::State_Selected) {
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
    }
    painter->setPen(pen);
    painter->setBrush(bckColor);

    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->drawEllipse(outerRect());

    painter->setPen(Qt::black);

    painter->drawText(outerRect(), Qt::AlignCenter, QString::number(number));

}

QList<Node *> Node::linkedNodes()
{
    QList<Node *> nls;
    foreach(Link *curLink, links) {
        Node *a = curLink->getEndNode();
        if(a != this)
            nls.append(a);
    }

    return nls;
}

QRectF Node::outerRect() const
{
    return QRectF(-10, -10, 20, 20);
}
