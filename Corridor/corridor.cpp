#include "corridor.h"

Corridor::Corridor(Grid_map _map):map(_map){

}

GridCorridor Corridor::gridCorridorSwell(Eigen::Vector2f swellPoint){       //飞行走廊的膨胀是基于栅格地图进行的，本来是写的整型，后来发现给的栅格坐标都是浮点，所以就先用浮点（史山这一块）

    GridCorridor gridCorridor = { Eigen::Vector2f(swellPoint.x(),swellPoint.y()), Eigen::Vector2f(swellPoint.x(),swellPoint.y())};

    //膨胀顺序：上左下右
    bool upBlocked = false;
    bool downBlocked = false;
    bool leftBlocked = false;
    bool rightBlocked = false;

    // Eigen::Matrix2i* intersectPoint;

    while(!upBlocked || !downBlocked || !leftBlocked || !rightBlocked) {    //没有阻挡或者未到边界->继续膨胀

        int newY = gridCorridor.TopLeft.y() - 1;
        if(upBlocked == false &&
            gridCorridor.TopLeft.y() > 0 &&
            map.obsMap.block(newY,gridCorridor.TopLeft.x(),1,gridCorridor.BottomRight.x() - gridCorridor.TopLeft.x() + 1).isZero()){    //注意越界

            gridCorridor.TopLeft.y()--;
        }
        else{
            upBlocked = true;
        }

        int newX = gridCorridor.TopLeft.x() - 1;
        if(leftBlocked == false &&
            gridCorridor.TopLeft.x() > 0 &&
            map.obsMap.block(gridCorridor.TopLeft.y(),newX,gridCorridor.BottomRight.y() - gridCorridor.TopLeft.y() + 1,1).isZero()){

            gridCorridor.TopLeft.x()--;
        }
        else{
            leftBlocked = true;
        }

        newY = gridCorridor.BottomRight.y() + 1;
        if(downBlocked == false &&
            gridCorridor.BottomRight.y() < map.grid_rows - 1 && //防止越界
            map.obsMap.block(newY,gridCorridor.TopLeft.x(),1,gridCorridor.BottomRight.x() - gridCorridor.TopLeft.x() + 1).isZero()){

            gridCorridor.BottomRight.y()++;
        }
        else{
            downBlocked = true;
        }

        newX = gridCorridor.BottomRight.x() + 1;
        if(rightBlocked == false &&
            gridCorridor.BottomRight.x() < map.grid_cols - 1 &&
            map.obsMap.block(gridCorridor.TopLeft.y(),newX,gridCorridor.BottomRight.y() - gridCorridor.TopLeft.y() + 1,1).isZero()){

            gridCorridor.BottomRight.x()++;
        }
        else{
            rightBlocked = true;
        }
    }
    return gridCorridor;
}

void Corridor::getFlightCorridor(std::vector<const Node*> _waypoint){   //用迭代器推，在优化点进行栅格膨胀，膨胀结束后进行遍历查找下一个优化点，毕竟飞行走廊的主要任务就是选点和加约束

    std::vector<const Node*>::iterator it = _waypoint.begin();

    // trajPoint.push_back((*it) -> coordinate);    //轨迹点是约束点，而这里的起点和终点是基于栅格坐标的，会有误差
    while(it != _waypoint.end() - 1){

        Eigen::Vector2f swellPoint = (*it)->coordinate;//这里直接取不在当前飞行走廊的下一个点进行膨胀，因为膨胀的逻辑是基于整数的
        GridCorridor swellCorridor = gridCorridorSwell(swellPoint);
        gridCorridorlist.push_back(swellCorridor);

        while(checkPointInRect((*it)->coordinate, swellCorridor) == true ){  //遍历获取下一次膨胀点，直到路径点超出飞行走廊或者到路径终点
            if(it != _waypoint.end() - 1){
                it++;
            }
            else{   //到终点直接退出循环
                transGrid2Real();
                qDebug()<<"飞行走廊搜索完成！\n";
                return;
            }
        }

        const Node* preNode = *(it-1);  //获取交点

        optTrajPoint.push_back(((*it) -> coordinate + preNode->coordinate)/2);
    }
}

bool Corridor::checkPointInRect(Eigen::Vector2f coord, GridCorridor corridor){      //基于栅格地图的点在矩阵判定
    if(coord.x() <= corridor.BottomRight.x() && coord.x() >= corridor.TopLeft.x() &&    //取等于是因为栅格的中心肯定在飞行走廊内
        coord.y() <= corridor.BottomRight.y() && coord.y() >= corridor.TopLeft.y()){
        return true;
    }
    else{
        return false;
    }
}

void Corridor::transGrid2Real(){
    for(int i = 0; i < (int)gridCorridorlist.size(); i++){
        Eigen::Vector2f realTopLeft = gridCorridorlist[i].TopLeft * map.square_side;
        Eigen::Vector2f realBottomRight = (gridCorridorlist[i].BottomRight + Eigen::Vector2f().Ones()) * map.square_side;
        realCorridorlist.push_back({realTopLeft, realBottomRight});
    }
}

void Corridor::corridorDebug(){
    std::cout<< "优化路径点：" << std::endl;
    for (auto it = optTrajPoint.cbegin(); it != optTrajPoint.cend(); ++it) {
        std::cout << (*it).x() << " "<< (*it).y()<< std::endl;
    }
    std::cout<< "飞行走廊角点：" << std::endl;
    for(RealCorridor it : realCorridorlist){
        std::cout<<"TopLeft:"<< it.TopLeft.x()<<" "<< it.TopLeft.y()<<" "
                  <<"BottomRight:"<< it.BottomRight.x()<<" "<< it.BottomRight.y()<<std::endl;
    }
}


