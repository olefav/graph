#include "link.h"

#include <math.h>

const qreal Pi = M_PI;

Link::Link(Node *startNode_f, Node *endNode_f, QGraphicsItem *parent, QGraphicsScene *scene)
    :QGraphicsLineItem(parent,scene)
{
    startNode = startNode_f;
    endNode = endNode_f;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    color = Qt::black;
    setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

Link::~Link()
{
    startNode->removeLink(this);
    endNode->removeLink(this);
}

QRectF Link::boundingRect() const
{
    if(!startNode || !endNode)
        return QRectF();

    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                    line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Link::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Link::updatePosition()
{
    QLineF line(mapFromItem(startNode, 0, 0), mapFromItem(endNode, 0, 0));
    setLine(line);
}

void Link::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (startNode->collidesWithItem(endNode))
        return;

    QPen myPen = pen();
    myPen.setColor(color);
    qreal arrowSize = 8;
    painter->setPen(myPen);
    painter->setBrush(color);

    painter->setRenderHint(QPainter::Antialiasing, true);

    QLineF cLine(startNode->pos(), endNode->pos());
    double length = cLine.length();

    QPointF dPoint;

    if(length > 20.) {
        QPointF edgeOffset((cLine.dx() * 10) / length, (cLine.dy() * 10) / length);
        dPoint = cLine.p2() - edgeOffset;
    }

    setLine(QLineF(dPoint, startNode->pos()));
    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize,
                                        cos(angle + Pi / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                        cos(angle + Pi - Pi / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;

        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        if (isSelected()) {
            painter->setPen(QPen(color, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
