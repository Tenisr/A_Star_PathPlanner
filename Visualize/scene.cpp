#include "scene.h"
#include "obstacle.h"

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{

    edgeLine.setPen(QPen(Qt::black, 3.0)); //边框并不急着设置大小，Scene初始化的时候还没有设置大小
    this->addItem(&edgeLine);

    dragRectangleItem = nullptr;    //初始化需要指空，有时候指针会莫名其妙非法访问然后崩溃
    dragCircleItem = nullptr;

    startPoint = new QGraphicsEllipseItem();
    startPoint->setPen(Qt::NoPen);
    startPoint->setBrush(QBrush(Qt::green));
    startPoint->setRect(0, 0, 10, 10);//疑似有点理解为什么要把这俩设置为不可见了
    endPoint = new QGraphicsEllipseItem();
    endPoint->setPen(Qt::NoPen);
    endPoint->setBrush(QBrush(Qt::red));
    endPoint->setRect(0, 0, 10, 10);
    startPoint->setVisible(false);
    endPoint->setVisible(false);
    startPoint->setZValue(2);
    endPoint->setZValue(2);
    this->addItem(startPoint);
    this->addItem(endPoint);
}

void Scene::clearGrid() {
    for (const QGraphicsItem* i : gridItems) {
        delete i;
    }
    gridItems.clear();

    for (const QGraphicsItem* i : corridorItems) {
        delete i;
    }
    corridorItems.clear();

    for (const QGraphicsItem* i : trajectoryPointItems) {
        delete i;
    }
    trajectoryPointItems.clear();

    for (const QGraphicsLineItem* i : bezierCurveItems) {
        delete i;
    }
    bezierCurveItems.clear();
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)  {
    if(event ->button() == Qt::LeftButton)
    {
        qDebug() << "鼠标点击场景位置：" << event->scenePos();  // 输出点击位置
        qDebug() << "障碍物类型" << visualText;
        if(visualText == "起点终点")
        {
            QPointF startPos = event->scenePos();
            if(!this->sceneRect().contains(startPos))
            {
                qDebug() << "选取位置无效！";
                return;
            }
            startPoint->setRect(startPos.x()-5, startPos.y()-5, 10, 10);//半径默认为5，懒得改了
            startPoint->setVisible(true);
        }
        else if(visualText == "矩形障碍")
        {
            dragRectangleItem = new QGraphicsRectItem();    //申请临时矩形，对于qt会自动释放new开辟的内存
            dragRectangleItem->setZValue(2);
            dragRectangleItem->setPen(Qt::NoPen);
            dragRectangleItem->setBrush(QBrush(Qt::gray));
            dragRectanglePos = event->scenePos();
            dragRectangleItem->setRect(dragRectanglePos.x(), dragRectanglePos.y(), 0, 0);
            this->addItem(dragRectangleItem);
        }
        else if(visualText == "圆形障碍")
        {
            dragCircleItem = new QGraphicsEllipseItem();
            dragCircleItem->setZValue(2);
            dragCircleItem->setPen(Qt::NoPen);
            dragCircleItem->setBrush(QBrush(Qt::gray));
            dragCirclePos = event->scenePos();
            dragCircleItem->setRect(dragCirclePos.x(), dragCirclePos.y(), 0, 0);
            this->addItem(dragCircleItem);
        }
    }
    else if(event ->button() == Qt::RightButton)
    {
        if(visualText == "起点终点")
        {
            QPointF endPos = event->scenePos();
            if(!this->sceneRect().contains(endPos))
            {
                qDebug() << "选取位置无效！";
                return;
            }
            endPoint->setRect(endPos.x()-5, endPos.y()-5, 10, 10);
            endPoint->setVisible(true);
        }
        else
        {
            QGraphicsScene::mousePressEvent(event);
        }
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)  {
        if(visualText == "矩形障碍")
        {
            if(dragRectangleItem!= nullptr)
            {
                // qDebug() << "鼠标移动位置：" << event->scenePos();
                // qDebug() << "dragRectanglePos" << dragRectanglePos;
                QPointF mousePos = event->scenePos();
                float w = abs(mousePos.x() - dragRectanglePos.x());
                float h = abs(mousePos.y() - dragRectanglePos.y());
                float x = std::min(dragRectanglePos.x(), mousePos.x());
                float y = std::min(dragRectanglePos.y(), mousePos.y());
                dragRectangleItem->setRect(x, y, w, h);
            }
        }
        else if(visualText == "圆形障碍")
        {
            if(dragCircleItem!= nullptr)
            {
                QPointF mousePos = event->scenePos();
                float r  = sqrt((pow(mousePos.x()-dragCirclePos.x(), 2)+pow(mousePos.y()-dragCirclePos.y(), 2)));
                dragCircleItem->setRect(dragCirclePos.x()-r, dragCirclePos.y()-r, 2*r, 2*r);
            }
        }
    else{
        QGraphicsScene::mouseMoveEvent(event);
    }

}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event ->button() == Qt::LeftButton)
    {
        if(visualText == "矩形障碍")
        {
            if(dragRectangleItem!= nullptr)
            {
                // qDebug() << "鼠标释放：" << event->scenePos();
                RectangleObstacle* finalRect = new RectangleObstacle();//  唉史山
                finalRect->setRect(dragRectangleItem->rect());
                // finalRect->setOpacity(0.8);
                // finalRect->setZValue(2);
                if(finalRect->rect().width() != 0 && finalRect->rect().height() != 0){
                    this->addItem(finalRect);
                    obstacleItemList.push_back(finalRect);
                }
                delete dragRectangleItem;
                dragRectangleItem = nullptr;//这里指向Null...不指向的话会报错...不想知道为什么

                // qDebug()<<finalRect->type();
            }
        }
        else if(visualText == "圆形障碍")
        {
            if(dragCircleItem!= nullptr)
            {
                CircleObstacle* finalCir = new CircleObstacle();
                finalCir->setRect(dragCircleItem->rect());
                // finalCir->setOpacity(0.8);
                // finalCir->setZValue(2);
                if(finalCir->rect().width() != 0 && finalCir->rect().height() != 0){
                    this->addItem(finalCir);
                    obstacleItemList.push_back(finalCir);
                }
                delete dragCircleItem;
                dragCircleItem = nullptr;

                // qDebug()<<finalCir->type();
            }
        }
    }

    else
    {
        QGraphicsScene::mouseReleaseEvent(event);
    }
}

void Scene::onComboboxTextChanged(const QString &text){ //这里的参数需要和槽函数对应，所以为const QString&
    visualText = text;
    // qDebug() << "选项框被触发！";
}

