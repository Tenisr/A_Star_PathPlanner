#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsScene>
#include "scene.h"

//https://doc.qt.io/qt-6/qgraphicsitem.html#type //对于每种graphicsitem都有唯一的type用于区别，在mainwindow.h内也有定义

#define Rect_type QGraphicsItem::UserType
#define Cir_type QGraphicsItem::UserType + 1

class CircleObstacle : public QGraphicsEllipseItem
{
    // Q_OBJECT //https://blog.csdn.net/u010168781/article/details/105298677
public:
    enum { Type = Cir_type };
    int type() const override
    {
        return Type;
    }
    explicit CircleObstacle(QGraphicsEllipseItem *parent = nullptr);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

};

class RectangleObstacle : public QGraphicsRectItem
{
    // Q_OBJECT
public:
    enum { Type = Rect_type };
    int type() const override
    {
        return Type;
    }
    explicit RectangleObstacle(QGraphicsRectItem *parent = nullptr);//这么写的吗

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
};

#endif // OBSTACLE_H
