#ifndef A_STAR_H
#define A_STAR_H
#include "Eigen/Dense"
#include <memory>
#include <set>
#include <iostream>
#include <cmath>
#include <queue>
#include "grid_map.h"
#include <QGraphicsItem>

class A_Star
{
public:
    A_Star(float _width, float _height, float _squareSide, QGraphicsEllipseItem *_startPoint,QGraphicsEllipseItem *_endPoint,float _pointRadius, std::vector<float> _heuPara);

    Node startGridPoint, endGridPoint;
    Node startRealPoint,endRealPoint;
    Grid_map gridMap;

    std::vector<Node> get_neighbors(const Node &node);
    const Node* check_node(Node _node);
    std::vector<const Node*> waypoint;    //最佳路径，基于栅格坐标
    void path_plan();
    void add_obstacle(float _x, float _y, float _w, float _h);//添加矩形
    void add_obstacle(float _x, float _y, float _r); //添加圆形
    float heuristic_Function(Node _minnode);
    std::vector<std::pair<float, float>> heuristicPara;

private:
    std::set<Node, CompareNode> openList;    //因为要查找最值，所以用了map，前键后值
    // std::priority_queue <Node> openList; //优先队列，这里没有用，因为不能遍历，赵学长的处理方法是把closeList pro（open+close）改为二维容器直接查询，以空间换时间
    std::vector<std::vector <Node>> closeList;
    std::vector<std::vector <bool>> searchedList;

};

#endif // A_STAR_H
