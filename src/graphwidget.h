/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QtGui>

#include "node.h"

class Node;
class QGraphicsScene;
class QGraphicsItem;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget();
    QGraphicsScene *scene;

    void itemMoved();
    void addNode(QString str);
    void addEdge(QString v1, QString v2, QString cost);
    QSet<Node*> verticesSet;
    QSet<QString> verticesNames;
    QMap<QString, Node*> vertexMap;

    QSet<QString> edgesSet;
    QMap<QString, Edge*> edgeMap;

    enum { NumNodes = 1000 , NumEdges = 3000 };
    Node *nodes[NumNodes];
    Edge *edges[NumEdges];

    Node *centerNode;

    bool edgeEditing;

    QSet<QString> newNodeSet;

    QSet<QString> centerSet;
    QSet<QString> pathSet;
    QSet<QString> pathWaySet;

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

    void scaleView(qreal scaleFactor);

private:    

    QGraphicsItem *item;

    bool logoYes;

    int timerId;        
};

#endif
