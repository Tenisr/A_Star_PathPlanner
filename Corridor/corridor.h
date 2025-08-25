#ifndef CORRIDOR_H
#define CORRIDOR_H
#include "Eigen/Dense"
#include "grid_map.h"
#include <iostream>


//因为栅格地图的坐标都是用浮点实现的，所以这里不需要realCorridor，哪怕有优化点也可以取小数，把自己气笑了
struct GridCorridor{
    Eigen::Vector2f TopLeft;
    Eigen::Vector2f BottomRight;
};

struct RealCorridor{
    Eigen::Vector2f TopLeft;
    Eigen::Vector2f BottomRight;
};

class Corridor
{
public:
    std::vector<GridCorridor> gridCorridorlist;   //飞行走廊
    std::vector<RealCorridor> realCorridorlist;
    std::vector<Eigen::Vector2f> optTrajPoint;      //优化后的轨迹点，即飞行走廊的途经点，这里没有用Node，因为不需要父节点
    Corridor(Grid_map _map);
    void getFlightCorridor(std::vector<const Node*> _waypoint);
    void corridorDebug();

private:

    Grid_map map;//这里map也是需要传入的，因为需要用到grid的私有方法checkpoint
    GridCorridor gridCorridorSwell(Eigen::Vector2f swellPoint);
    bool checkPointInRect(Eigen::Vector2f coord, GridCorridor corridor);    //之前调用的Grid的方法，但是那个方法根本不是点在矩阵内的判定，并且还有bug
    void transGrid2Real();

};



#endif // CORRIDOR_H
