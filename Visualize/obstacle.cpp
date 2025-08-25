#include "obstacle.h"
#include "qpainter.h"


RectangleObstacle :: RectangleObstacle(QGraphicsRectItem *parent)
    :QGraphicsRectItem(parent){
    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(Qt::darkGray));
}

void RectangleObstacle:: mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::RightButton) {
        Scene* parentScene = dynamic_cast<Scene*>(scene()); //将基类指针转换为派生类指针，这里转换的指针为通过scene()获取的障碍物所处的QGraphicsScene，再通过dynamic_cast转换为自定义的Scene类型
        parentScene->removeItem(this);
        parentScene->obstacleItemList.erase(std::remove(parentScene->obstacleItemList.begin(),  parentScene->obstacleItemList.end(),this),
                                            parentScene->obstacleItemList.end());
    }
    else {
        QGraphicsRectItem::mousePressEvent(mouseEvent);
    }

}

CircleObstacle:: CircleObstacle(QGraphicsEllipseItem *parent)
    :QGraphicsEllipseItem(parent){
    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(Qt::darkGray));
}

void CircleObstacle:: mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
    if (mouseEvent->button() == Qt::RightButton) {
        Scene* parentScene = dynamic_cast<Scene*>(scene());
        parentScene->removeItem(this);
        parentScene->obstacleItemList.erase(std::remove(parentScene->obstacleItemList.begin(),  parentScene->obstacleItemList.end(),this),
                                            parentScene->obstacleItemList.end());
    }
    else {
        QGraphicsEllipseItem::mousePressEvent(mouseEvent);
    }
}

