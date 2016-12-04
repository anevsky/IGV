/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QString>

class Edge;
class GraphWidget;

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget);
    Node(GraphWidget *graphWidget, QString nodeName);
    Node(GraphWidget *graphWidget, QString nodeName, int c);

    int edgesSetting;

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    void calculateForces();
    bool advance();

    void setName(QString str) { name = str; }
    QString getName() { return name; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintPath(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void paintCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    
private:
    QList<Edge *> edgeList;
    QPointF newPos;
    QString name;
    GraphWidget *graph;
    QGraphicsTextItem *textNode;
    int color;
};

#endif
