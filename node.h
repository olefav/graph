#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QSet>
#include <QList>

class Link;

class Node : public QGraphicsItem
{
public:

    enum {Type = UserType + 50};

    int getNumber() const;
    QColor getColor() const;
    void setNumber(int n) {number = n;}
    void setColor(QColor color);
    Node(int f_number = 1);
    ~Node();
    void addLink(Link *link);
    void removeLink (Link *link);
    void removeLinks();
    QRectF boundingRect() const;
    QPainterPath shape() const;
    int type() const { return Type; }
    QList<Node *> linkedNodes();

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QRectF outerRect() const;
    int number;
    QColor bckColor;
    QSet<Link *> links;
    
};

#endif // NODE_H
