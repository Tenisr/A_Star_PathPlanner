#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include "math.h"
#include <Eigen/Dense>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);  //explcit禁止隐式转换
    QGraphicsRectItem edgeLine;//边框
    std::vector<QGraphicsItem*> obstacleItemList;//用一个父类容器包含所有的障碍物类型
    QGraphicsEllipseItem* startPoint;
    QGraphicsEllipseItem* endPoint;
    float pointRadius = 5;//点的半径
    std::vector<QGraphicsRectItem*> gridItems;   //栅格
    std::vector<QGraphicsRectItem*> corridorItems; //飞行走廊
    std::vector<QGraphicsEllipseItem*> trajectoryPointItems;
    std::vector<QGraphicsLineItem*> bezierCurveItems;
    void clearGrid();



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent) override{
        this->mousePressEvent(mouseEvent);  //检测item是很好用，那么代价是什么呢，双击会被检测到
    };

private:
    void mousePressHandler(QGraphicsSceneMouseEvent *event);
    void mouseMoveHandler(QGraphicsSceneMouseEvent *event);
    void mouseReleaseHandler(QGraphicsSceneMouseEvent *event);


    QGraphicsRectItem* dragRectangleItem; // 用于绘制临时矩形，与finalItem是不同类型
    QGraphicsEllipseItem* dragCircleItem;
    QPointF dragRectanglePos; // 用于存放矩形起始点
    QPointF dragCirclePos; // 用于存放圆形起始点
    QString visualText = "未选择";

public slots:
    void onComboboxTextChanged(const QString &text);

signals:
};
#endif // SCENE_H
