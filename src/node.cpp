/*
 * Author: Alex Nevsky
 *
 * WebSite: http://alexnevsky.com
 *
 * Copyright 2010 Alex A. Nevsky. 
 * GNU General Public License.
 */
 
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "edge.h"
#include "node.h"
#include "graphwidget.h"

Node::Node(GraphWidget *graphWidget)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    name = "vertex";
    textNode = 0;
    edgesSetting = 1;
    color = 0;
}

Node::Node(GraphWidget *graphWidget, QString nodeName)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    name = nodeName;
    textNode = 0;
    edgesSetting = 1;
    color = 0;
}

Node::Node(GraphWidget *graphWidget, QString nodeName, int c)
    : graph(graphWidget)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    name = nodeName;
    textNode = 0;
    edgesSetting = 1;
    color = c;
}

void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

void Node::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        newPos = pos();
        return;
    }

    qreal xvel = 0;
    qreal yvel = 0;

    if (edgesSetting == 1) {
        // Sum up all forces pushing this item away
        foreach (QGraphicsItem *item, scene()->items()) {
            Node *node = qgraphicsitem_cast<Node *>(item);
            if (!node)
                continue;

            QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
            qreal dx = line.dx();
            qreal dy = line.dy();
            double l = 2.0 * (dx * dx + dy * dy);
            if (l > 0) {
                xvel += (dx * 150.0) / l;
                yvel += (dy * 150.0) / l;
            }
        }

        // Now subtract all forces pulling items together
        double weight = (edgeList.size() + 1) * 10;
        foreach (Edge *edge, edgeList) {
            QPointF pos;
            if (edge->sourceNode() == this)
                pos = mapFromItem(edge->destNode(), 0, 0);
            else
                pos = mapFromItem(edge->sourceNode(), 0, 0);
            xvel += pos.x() / weight;
            yvel += pos.y() / weight;
        }
    }

    if (edgesSetting == 2) {
        // Sum up all forces pushing this item away
        foreach (QGraphicsItem *item, scene()->items()) {
            Node *node = qgraphicsitem_cast<Node *>(item);
            if (!node)
                continue;

            QLineF line(mapFromItem(node, 0, 0), QPointF(0, 0));
            qreal dx = line.dx();
            qreal dy = line.dy();
            double l = 2.0 * (dx * dx + dy * dy);
            if (l > 0) {
                xvel += (dx * 150.0) / l;
                yvel += (dy * 150.0) / l;
            }
        }
    }

    if (edgesSetting == 0) {
        foreach (QGraphicsItem *item, scene()->items()) {
            Node *node = qgraphicsitem_cast<Node *>(item);
            if (!node)
                continue;
        }
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));

    if (!textNode) {
        textNode = new QGraphicsTextItem(getName());
        textNode->setDefaultTextColor(Qt::darkMagenta);
        textNode->setFont(QFont("Normal", 10, QFont::Bold));
        textNode->setPos(this->pos().x(), this->pos().y());
        textNode->setZValue(2);
        graph->scene->addItem(textNode);
    } else {
        delete textNode;
        textNode = new QGraphicsTextItem(getName());
        textNode->setDefaultTextColor(Qt::darkMagenta);
        textNode->setFont(QFont("Normal", 10, QFont::Bold));
        textNode->setPos(this->pos().x(), this->pos().y());
        textNode->setZValue(2);
        graph->scene->addItem(textNode);
    }
}

bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                  23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{    
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);

    if (color == 0) {        
        if (option->state & QStyle::State_Sunken) {
            gradient.setCenter(3, 3);
            gradient.setFocalPoint(3, 3);
            gradient.setColorAt(1, QColor(Qt::yellow).light(120));
            gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
        } else {
            gradient.setColorAt(0, Qt::yellow);
            gradient.setColorAt(1, Qt::darkYellow);
        }
    }

    if (color == 1) {
        if (option->state & QStyle::State_Sunken) {
            gradient.setCenter(3, 3);
            gradient.setFocalPoint(3, 3);
            gradient.setColorAt(1, QColor(Qt::blue).light(120));
            gradient.setColorAt(0, QColor(Qt::darkBlue).light(120));
        } else {
            gradient.setColorAt(0, Qt::blue);
            gradient.setColorAt(1, Qt::darkBlue);
        }
    }

    if (color == 2) {
        if (option->state & QStyle::State_Sunken) {
            gradient.setCenter(3, 3);
            gradient.setFocalPoint(3, 3);
            gradient.setColorAt(1, QColor(Qt::red).light(120));
            gradient.setColorAt(0, QColor(Qt::darkRed).light(120));
        } else {
            gradient.setColorAt(0, Qt::red);
            gradient.setColorAt(1, Qt::darkRed);
        }
    }

    if (color == 3) {
        if (option->state & QStyle::State_Sunken) {
            gradient.setCenter(3, 3);
            gradient.setFocalPoint(3, 3);
            gradient.setColorAt(1, QColor(Qt::green).light(120));
            gradient.setColorAt(0, QColor(Qt::darkGreen).light(120));
        } else {
            gradient.setColorAt(0, Qt::green);
            gradient.setColorAt(1, Qt::darkGreen);
        }
    }

    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);

    /*
    painter->setFont(QFont("Times", 14, QFont::Normal));
    QString str = "vertex";
    painter->drawText(this->pos().x(), this->pos().y(), str);
    */

    /*
    QGraphicsTextItem *text = new QGraphicsTextItem("Vertex Fuck");
    text->setDefaultTextColor(Qt::red);
    text->setPos(this->pos().x(), this->pos().y());
    text->setZValue(2);
    graph->scene->addItem(text);
    */
}

void Node::paintCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::blue).light(120));
        gradient.setColorAt(0, QColor(Qt::darkBlue).light(120));
    } else {
        gradient.setColorAt(0, Qt::blue);
        gradient.setColorAt(1, Qt::darkBlue);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

void Node::paintPath(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::red).light(120));
        gradient.setColorAt(0, QColor(Qt::darkRed).light(120));
    } else {
        gradient.setColorAt(0, Qt::red);
        gradient.setColorAt(1, Qt::darkRed);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        graph->itemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);

    if (graph->newNodeSet.size() > 2) {
        graph->newNodeSet.clear();
    }

    if (graph->edgeEditing) {
        graph->newNodeSet.insert(getName());
    }
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);

    if (graph->edgeEditing && (graph->newNodeSet.size() == 2)) {
        QString s1, s2, s3;
        s1 = *graph->newNodeSet.begin();
        s2 = *(graph->newNodeSet.end()-1);
        s3 = "1";
        graph->addEdge(s1, s2, s3);
        graph->newNodeSet.clear();
    }

    if (graph->newNodeSet.size() > 2) {
        graph->newNodeSet.clear();
    }

    if (!textNode) {
        textNode = new QGraphicsTextItem(getName());
        textNode->setDefaultTextColor(Qt::darkMagenta);
        textNode->setFont(QFont("Normal", 10, QFont::Bold));
        textNode->setPos(this->pos().x(), this->pos().y());
        textNode->setZValue(2);
        graph->scene->addItem(textNode);
    } else {
        delete textNode;
        textNode = new QGraphicsTextItem(getName());
        textNode->setDefaultTextColor(Qt::darkMagenta);
        textNode->setFont(QFont("Normal", 10, QFont::Bold));
        textNode->setPos(this->pos().x(), this->pos().y());
        textNode->setZValue(2);
        graph->scene->addItem(textNode);
    }
}
