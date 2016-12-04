/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include "graphwidget.h"
#include "edge.h"
#include "node.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>

#include <math.h>

GraphWidget::GraphWidget()
    : timerId(0)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-2000, -1500, 4000, 3000);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    edgeEditing = false;

    QPixmap logoPix(":/images/logo.png");
    scene->addPixmap(logoPix)->setPos(-300, -300);

    logoYes = true;

    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(700, 700);
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if (centerNode)
            centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        if (centerNode)
            centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        if (centerNode)
            centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        if (centerNode)
            centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        foreach (item, scene->items()) {
            if (qgraphicsitem_cast<Node *>(item))
                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (item, scene->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
	painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
	painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
/*
    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Hello, "
                       "world!"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
*/
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::addNode(QString str) {
    if (logoYes) {
        scene->clear();
        logoYes = false;
    }
    if (str.isEmpty()) {
        str += "v";
        str += QString::number(verticesSet.size() + 1);
        //WWWWWWWWWWWWWWWWWWWWWWWWWWWWHHHHHHHHHHHHHHHHHHHHHHHHAAAAAAAAAAAAAAAAAAAA vertex vertex vertex in widget
        //verticesNames.insert(str);
    }    

    if (!verticesNames.contains(str)) {

        if (pathSet.contains(str)) {
            nodes[verticesSet.size() + 1] = new Node(this, str, 2);
        } else if (pathWaySet.contains(str)) {
            nodes[verticesSet.size() + 1] = new Node(this, str, 3);
        } else if (centerSet.contains(str)) {
            nodes[verticesSet.size() + 1] = new Node(this, str, 1);
        } else {
            nodes[verticesSet.size() + 1] = new Node(this, str);
        }

        verticesSet.insert(nodes[verticesSet.size() + 1]);

        scene->addItem(nodes[verticesSet.size()]);

        nodes[verticesSet.size()]->setPos(qrand() % 50 + verticesSet.size(), 0 + verticesSet.size() * 2);

        verticesNames.insert(str);

        vertexMap.insert(str, nodes[verticesSet.size()]);
    }    

    QFile fileToWrite("sysFiles/sysShowVertices.igv");

    fileToWrite.open(QIODevice::WriteOnly);
    QTextStream out(&fileToWrite);

    QSetIterator<QString> iter(verticesNames);
    while (iter.hasNext()) {
        out << iter.next() << " ";
    }
    fileToWrite.close();
}

void GraphWidget::addEdge(QString v1, QString v2, QString cost) {
    if ((vertexMap.contains(v1) && vertexMap.contains(v2)) && !edgesSet.contains((((((v2 + "--") + v1) + "$") + cost) + "\n"))) {
        edges[edgesSet.size() + 1] = new Edge(vertexMap[v1], vertexMap[v2]);
        edgeMap.insert(((((v1 + "--") + v2) + "$") + cost) + "\n", edges[edgesSet.size()]);

        scene->addItem(edgeMap[((((v1 + "--") + v2) + "$") + cost) + "\n"]);

        edgesSet.insert(((((v1 + "--") + v2) + "$") + cost) + "\n");
    }
    if ((vertexMap.contains(v1) && vertexMap.contains(v2)) && !edgesSet.contains((((((v2 + "--") + v1) + "$") + cost) + "\n"))) {
        edges[edgesSet.size() + 1] = new Edge(vertexMap[v1], vertexMap[v2]);
        edgeMap.insert(((((v1 + "--") + v2) + "$") + cost) + "\n", edges[edgesSet.size()]);

        scene->addItem(edgeMap[((((v1 + "--") + v2) + "$") + cost) + "\n"]);

        edgesSet.insert(((((v1 + "--") + v2) + "$") + cost) + "\n");
    }
    if ((vertexMap.contains(v1) && vertexMap.contains(v2)) && !edgesSet.contains((((((v2 + "--") + v1) + "$") + cost) + "\n"))) {
        edges[edgesSet.size() + 1] = new Edge(vertexMap[v1], vertexMap[v2]);
        edgeMap.insert(((((v1 + "--") + v2) + "$") + cost) + "\n", edges[edgesSet.size()]);

        scene->addItem(edgeMap[((((v1 + "--") + v2) + "$") + cost) + "\n"]);

        edgesSet.insert(((((v1 + "--") + v2) + "$") + cost) + "\n");
    }
}
