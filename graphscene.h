#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QtGui>

class Node;
class Link;

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:

    enum Mode { InsertNode, InsertLine, MoveItem };

    int nodesCount() const {return numNodes;}
    void nodeDeleted() {--numNodes;}
    void insertNodes(int num);
    void clearScene();
    void deleteSelectedItems();
    void deleteAllLinks();
    void setMode(GraphScene::Mode mode);
    QList<Node*> getAllNodes() const;


    explicit GraphScene(QObject *parent = 0);    

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    int numNodes;
    Mode curMode;
    bool leftButtonDown;
    QPointF startPoint;
    QGraphicsLineItem *line;
    
};

#endif // GRAPHSCENE_H
