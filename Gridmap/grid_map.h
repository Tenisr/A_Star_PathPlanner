#ifndef GRID_MAP_H
#define GRID_MAP_H
#include "Eigen/Dense"
// #include <OsqpEigen/OsqpEigen.h>
#include "float.h"
#include <QDebug>
#include "math.h"

//为了实现通用，把Node类改到了gridMap下，这样Astar和Corridor都可以使用（赵学长在Astar和Corridor下分别用了两套记录节点的对象...），不过好像耦合就加深了...

class Node{
public:
    Node(Eigen::Vector2f _coordinate = Eigen::Vector2f(0, 0),  float _cost = DBL_MAX, Node* _parent = nullptr);
    Eigen::Vector2f coordinate; //这里的坐标是栅格地图坐标
    float cost;
    const Node* parent;   //用于指向父节点（const修饰无法修改值），为什么要用智能指针呢
private:
         // bool operator < (const Node & a) const
         // {
         //     return cost <a.cost;
         // }
};

struct CompareNode{ //对于比较对象还没弄太明白
    bool operator()(const Node& a,  const Node& b) const {
        if(a.cost != b.cost)
            return a.cost < b.cost; // 按照距离升序排序，由于集合具有互异性，如果相同这里不会加入, 当然这里不会出现坐标相同的点
        else if(a.coordinate.x() != b.coordinate.x() )
            return a.coordinate.x() < b.coordinate.x();
        else
            return a.coordinate.y() < b.coordinate.y();
    }
};

class Grid_map
{
public:
    Grid_map(float _width, float _height, float _side, float _fPR,float _fPV,float _sPR, float _sPV);
    void addObstacle(float _x, float _y, float _w, float _h);//添加矩形
    void addObstacle(float _x, float _y, float _r); //添加圆形
    void setHeuristicMap(float radius, float penalty,Eigen::MatrixXf &heuMap,Eigen::MatrixXf &obsMap);   //添加惩罚，这里的radius为核半径，单位是栅格

    Eigen::Vector2f gridCoordTrans(int grid_cols, int grid_rows);

    float square_side;
    Eigen::MatrixXf obsMap;//障碍物动态矩阵
    float fstPenaltyRadius;
    float fstPenaltyValue;
    float sedPenaltyRadius;
    float sedPenaltyValue;
    Eigen::MatrixXf fstHeuGridMap;//一阶惩罚矩阵
    Eigen::MatrixXf sedHeuGridMap;//二阶惩罚矩阵


    int grid_cols;//列数
    int grid_rows;//行数


private:

    bool checkGridIntersectRect(const Eigen::Vector2f &Point_coord, float Rect_x, float Rect_y, float Rect_w, float Rect_h);
    bool checkPointInCir(Eigen::Vector2f Point_coord, float Cir_x, float Cir_y, float Cir_r);

};

#endif // GRID_MAP_H
