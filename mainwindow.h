#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <iostream>
#include "scene.h"
#include "a_star.h"
#include "obstacle.h"
#include "corridor.h"
#include "trajectory.h"




QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Scene *graphicsScene;  //这里的场景是属于Mainwindow的
    float sceneMaxSize; // 显示窗口最大边的长度

    void signalConnect();
    void onStratPlanButtonClicked();
    void onClearButtonClicked();
    void obsScene2Grid(A_Star &_planner);
    void obsGrid2HeuGrid(A_Star &_planner);
    void gridVisualize(Eigen::MatrixXf Map,QColor color,Grid_map gridMap);
    void obsReDraw();
    void pathVisualize(std::vector<const Node*> _waypoint,float square_side);
    void corridorVisualize(std::vector<RealCorridor> flightCorridor);
    void trajPointVisualize(std::vector<Eigen::Vector2f> trajPoint,float gridSize);
    void drawCurve(std::vector<Eigen::Vector2f>& _controlPoints,std::vector<Eigen::Vector2f>& _wayPoints,int order, float percision);

};
#endif // MAINWINDOW_H
