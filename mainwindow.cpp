#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QGraphicsItem"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);  //加载UI布局
    /* 实例化场景对象 */
    sceneMaxSize = 675; //长边大小
    graphicsScene = new Scene(this);    //开辟一片新内存，Qt会负责这块内存的释放
    graphicsScene->setSceneRect(0, 0, sceneMaxSize / 15000 * 8000, sceneMaxSize);//调整Scene大小
    graphicsScene->edgeLine.setRect(graphicsScene->sceneRect()); //画边框
    ui->graphicsView->setFixedSize(sceneMaxSize+20,  sceneMaxSize+20);//固定view，防止突然扩张导致图形移动，这里+20是为了给周围留出空隙，美观

    graphicsScene->setBackgroundBrush(Qt::white); //背景颜色
    ui->graphicsView->setScene(graphicsScene);//由于使用UI布局所以这里直接调用布局内的视图

    signalConnect();

}

void MainWindow::onStratPlanButtonClicked(){    //关于planner的内存释放需要问一下
    graphicsScene->clearGrid();

    std::vector<float> heuPara={(float)ui->fstPenaltyRad->value(),
                                (float)ui->fstPenaltyValue->value(),
                                (float)ui->sedPenaltyRad->value(),
                                (float)ui->sedPenaltyValue->value(),};
    A_Star planner(graphicsScene->width(),  graphicsScene->height(), ui->gridSize->value(), graphicsScene->startPoint, graphicsScene->endPoint,graphicsScene->pointRadius,heuPara);

    std::cout<<"起点位置\n"<< planner.startRealPoint.coordinate << std::endl;//qDebug输出不了eigen库
    std::cout<<"终点位置\n"<< planner.endRealPoint.coordinate << std::endl;//起点终点的rect坐标为0，中心不为0

    obsScene2Grid(planner);//这里mainwindow的定位更像是一个中间层，将UI层的数据与data层的容器进行对接

    obsGrid2HeuGrid(planner);//惩罚地图
    // std::cout<<planner.gridMap.fstHeuGridMap<<std::endl;
    // std::cout<<std::endl;
    // std::cout<<planner.gridMap.sedHeuGridMap<<std::endl;

    gridVisualize(planner.gridMap.sedHeuGridMap,QColor(255, 204, 204),planner.gridMap); //因为函数没有z方向的区分，所以这里显示主要靠后面进行覆盖
    gridVisualize(planner.gridMap.fstHeuGridMap,QColor(226, 62, 87),planner.gridMap);
    gridVisualize(planner.gridMap.obsMap,QColor(170,150,218),planner.gridMap); //障碍物栅格可视化
    obsReDraw();    //障碍物重绘

    planner.path_plan();    //路径规划，启动！

    Corridor corridorInstance(planner.gridMap);
    corridorInstance.getFlightCorridor(planner.waypoint);

    Trajectory trajectoryInstance(corridorInstance.optTrajPoint,planner.startRealPoint.coordinate,planner.endRealPoint.coordinate,planner.gridMap.square_side,corridorInstance.realCorridorlist);
    trajectoryInstance.solveProblem(); //轨迹优化，启动！
    //路径可视化
    pathVisualize(planner.waypoint,planner.gridMap.square_side);

    //轨迹可视化
    drawCurve(trajectoryInstance.controlPoints,trajectoryInstance.wayPoints,trajectoryInstance.order,0.01);

    //飞行走廊可视化
    if(ui->drawCorridor->checkState() == Qt::Checked){
        corridorVisualize(corridorInstance.realCorridorlist);
        corridorInstance.corridorDebug();
        qDebug()<<"飞行走廊已绘制！\n";
    }

    //轨迹点可视化
    if(ui->drawOptWaypoint->checkState() == Qt::Checked){
        trajPointVisualize(corridorInstance.optTrajPoint,planner.gridMap.square_side);
        qDebug()<<"轨迹点已绘制！\n";
    }

}

void MainWindow::signalConnect(){
    QObject::connect(ui->visualComboBox,  &QComboBox::currentTextChanged,   //新信号槽写法，参数自动匹配
                     graphicsScene,  &Scene::onComboboxTextChanged);

    QObject::connect(ui->pathPlanButton, &QPushButton::clicked,
                     this, &MainWindow::onStratPlanButtonClicked);

    QObject::connect(ui->clearButton, &QPushButton::clicked,
                     this, &MainWindow::onClearButtonClicked);
}

void MainWindow::onClearButtonClicked(){
    graphicsScene->clearGrid();
    for (QGraphicsItem* item : graphicsScene->obstacleItemList) {
        auto type = item->type();
        if(type != QGraphicsEllipseItem::Type){
            delete item;
        }
    }
    graphicsScene->obstacleItemList.clear();
    graphicsScene->startPoint->setVisible(false);
    graphicsScene->endPoint->setVisible(false);
}

void MainWindow::obsScene2Grid(A_Star &_planner){
    //栅格地图输入
    for(QGraphicsItem* item : graphicsScene->obstacleItemList){
        if (item->type() == Rect_type)
        {
            RectangleObstacle *obsRect =  qgraphicsitem_cast<RectangleObstacle*>(item);
            float w_f = obsRect->rect().width();
            float h_f = obsRect->rect().height();
            float x_f = obsRect->rect().x() + w_f/2;
            float y_f = obsRect->rect().y() + h_f/2;
            // qDebug()<< "A_Star_obstacle_rect" << x_f<<y_f<<w_f<<h_f;
            _planner.gridMap.addObstacle(x_f, y_f, w_f, h_f);
        }
        else if(item->type() == Cir_type){
            CircleObstacle *obsCir =  qgraphicsitem_cast<CircleObstacle*>(item);
            float r_f = obsCir->rect().width()/2;
            float x_f = obsCir->rect().x() + r_f;
            float y_f = obsCir->rect().y() + r_f;
            // qDebug()<< "A_Star_obstacle_cir" << x_f<<y_f<<r_f;
            _planner.gridMap.addObstacle(x_f, y_f, r_f);
        }
    }
}

void MainWindow::obsGrid2HeuGrid(A_Star &_planner){
    //最初其实是想把障碍物和惩罚做到一张地图里面的，但是考虑到需要的遍历次数和处理有点多这里就拆分为三个地图
    _planner.gridMap.fstHeuGridMap = _planner.gridMap.obsMap;   //将障碍物地图直接赋给两个启发式地图，其实障碍物信息不重要，主要是创建一张对应大小的地图，否则查找时会指空
    _planner.gridMap.sedHeuGridMap = _planner.gridMap.obsMap;
    _planner.gridMap.setHeuristicMap(_planner.gridMap.fstPenaltyRadius,_planner.gridMap.fstPenaltyValue,_planner.gridMap.fstHeuGridMap,_planner.gridMap.obsMap);  //根据障碍物地图进行膨胀
    _planner.gridMap.setHeuristicMap(_planner.gridMap.sedPenaltyRadius,_planner.gridMap.sedPenaltyValue,_planner.gridMap.sedHeuGridMap,_planner.gridMap.obsMap);
}

void MainWindow::gridVisualize(Eigen::MatrixXf Map,QColor color,Grid_map gridMap){  //再一次切身体会到自己写得有多史
    for(int i = 0; i < Map.cols();i++){
        for(int j = 0; j < Map.rows();j++){
            if(Map(j, i))
            {
                float w = gridMap.square_side * 0.9;  //显示栅格之间需要留一点空隙，否则就是一坨色块
                float h = gridMap.square_side * 0.9;
                float x = gridMap.gridCoordTrans(i, j).x() - gridMap.square_side/2;
                float y = gridMap.gridCoordTrans(i, j).y() - gridMap.square_side/2;
                QGraphicsRectItem *grid_item = new QGraphicsRectItem();
                grid_item->setPen(Qt::NoPen);
                grid_item->setBrush(QBrush(color));
                grid_item->setRect(x, y, w, h);
                graphicsScene->gridItems.push_back(grid_item);
                graphicsScene->addItem(grid_item);
            }
        }
    }
}

void MainWindow::obsReDraw(){
    for(QGraphicsItem* item : graphicsScene->obstacleItemList){
        item->setOpacity(0.6);
        item->setZValue(2);
    }
}

void MainWindow::pathVisualize(std::vector<const Node*> _waypoint,float square_side){
    for(const Node* _wayPoint : _waypoint){
        float w = square_side * 0.9;
        float h = square_side * 0.9;
        float x = _wayPoint->coordinate.x() * square_side;
        float y = _wayPoint->coordinate.y() * square_side;
        QGraphicsRectItem *grid_item = new QGraphicsRectItem();
        grid_item->setPen(Qt::NoPen);
        grid_item->setBrush(QBrush(QColor(204, 255, 153)));
        grid_item->setRect(x, y, w, h);
        grid_item->setOpacity(0.7);
        graphicsScene->gridItems.push_back(grid_item);
        graphicsScene->addItem(grid_item);
    }
}

void MainWindow::corridorVisualize(std::vector<RealCorridor> flightCorridor){
    for(auto it = flightCorridor.begin(); it != flightCorridor.end() ; it++){
        float w = abs(it->TopLeft.x() - it->BottomRight.x());
        float h = abs(it->TopLeft.y() - it->BottomRight.y());
        float x = it->TopLeft.x();
        float y = it->TopLeft.y();

        QGraphicsRectItem *corridor_item = new QGraphicsRectItem();
        corridor_item->setPen(QPen(QColor(51, 255, 255),2));
        corridor_item->setRect(x, y, w, h);
        graphicsScene->corridorItems.push_back(corridor_item);
        graphicsScene->addItem(corridor_item);
    }
}

void MainWindow::trajPointVisualize(std::vector<Eigen::Vector2f> trajPoint,float gridSize){
    for(auto it = trajPoint.begin(); it != trajPoint.end() ; it++){

        QGraphicsEllipseItem *trajPoint_item = new QGraphicsEllipseItem();

        trajPoint_item->setPen(Qt::NoPen);
        trajPoint_item->setBrush(QBrush(QColor(255, 255, 102)));

        trajPoint_item->setRect(it->x() * gridSize, it->y() * gridSize, 10, 10);
        graphicsScene->trajectoryPointItems.push_back(trajPoint_item);
        graphicsScene->addItem(trajPoint_item);
    }
}

void MainWindow::drawCurve(std::vector<Eigen::Vector2f>& _controlPoints,std::vector<Eigen::Vector2f>& _wayPoints,int order, float percision){
    QPointF sPos, ePos;
    sPos = QPointF();
    ePos = QPointF();

    int pathNum = _wayPoints.size() - 1;

    for(int i = 0; i < pathNum; i++){
        float sPosX = 0;
        float sPosY = 0;

        float u = 0;
        while(u <= 1){
            sPosX = 0;
            sPosY = 0;

            for(int j = 0; j <= order; j++){
                int C = factorial(order)/(factorial(j)*factorial(order-j));
                sPosX += _controlPoints[i*(order+1)+j].x()*pow(u,j)*C*pow(1-u,order-j);
                sPosY += _controlPoints[i*(order+1)+j].y()*pow(u,j)*C*pow(1-u,order-j);
            }
            sPos.setX(sPosX);
            sPos.setY(sPosY);
            if(ePos.x() == 0 && ePos.y() == 0){     //处理起点
                ePos =  sPos;
            }

            QGraphicsLineItem* l = new QGraphicsLineItem(sPos.x(),sPos.y(),ePos.x(),ePos.y());
            l->setPen(QPen(Qt::blue,2));

            graphicsScene->bezierCurveItems.push_back(l);
            graphicsScene->addItem(l);

            ePos =  sPos;
            u += percision;
        }
    }

    // for(Eigen::Vector2f w:_wayPoints){
    //     graphicsScene->addItem(new QGraphicsEllipseItem(w.x()-2.5,w.y()-2.5,5,5));
    // }

}

int factorial(int n) {
    if (n < 0) return 0; // 负数无阶乘
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
};

MainWindow::~MainWindow()
{
    delete ui;
    // 怪事，之前退出时总是报错进程崩溃，在析构加了一行Qdebug后再也没出现过
}
