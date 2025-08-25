#include "a_star.h"

// 任务：1.边框不能有障碍物 √
// 2.把openlist改为优先队列 ×
// 3.代码格式化，逗号后面加空格，if和for加大括号 √
// 4.每次开始规划时清空openlist，closelist，searchedlist √
// 5.路径点和障碍物用不同的item类 √
// 6.升级A* √
//  6.1 添加到终点距离，并加上动态加权 √×
//  6.2 实现障碍物膨胀，并添加惩罚值  √
//  6.3 实现收录节点与惩罚值的可视化  √×（收录节点可视化要实现的话估计惩罚值就做不了，又要写一堆状态）
//  6.4 实现参数自定义 √
// 7.不要用coor.x
// 8.添加清空地图  √
// ⑨.写注释，详细注释 √×
// 10.类名大写驼峰，函数和变量名小写驼峰 √

/*Tenisr：可能我比较懒吧，或者觉得做到这里已经学到够多的东西了，其实有很多东西都还是没做的
          比如运行时间，贝塞尔曲线，优化遍历逻辑，注释也没写完
          UI上也有很多地方没处理好，item障碍加透明会产生叠加应该让grid实现等等...
*/
A_Star::A_Star(float _width, float _height, float _squareSide, QGraphicsEllipseItem *_startPoint,QGraphicsEllipseItem *_endPoint, float _pointRadius, std::vector<float> _heuPara)   //这里直接传入起点和终点图形，不然参数很多不好处理
    :gridMap(_width, _height, _squareSide,_heuPara[0],_heuPara[1],_heuPara[2],_heuPara[3])  //组合类显式初始化栅格地图，在构造函数之前
{
    float _startPointx = _startPoint->rect().x() + _pointRadius;
    float _startPointy = _startPoint->rect().y() + _pointRadius;
    float _endPointx = _endPoint->rect().x() + _pointRadius;
    float _endPointy = _endPoint->rect().y() + _pointRadius;
    startRealPoint.coordinate = Eigen::Vector2f(_startPointx,_startPointy);
    endRealPoint.coordinate = Eigen::Vector2f(_endPointx,_endPointy);
    startGridPoint.coordinate = Eigen::Vector2f(floor(_startPointx/_squareSide), floor(_startPointy/_squareSide));//因为item默认坐标左上角，所以这里不能四舍五入，必须取小
    startGridPoint.cost = 0;   //起点的距离默认为0
    startGridPoint.parent = nullptr;   //父节点指空，拉取遍历时作为循环条件
    endGridPoint.coordinate = Eigen::Vector2f(floor(_endPointx/_squareSide), floor(_endPointy/_squareSide));
    endGridPoint.cost = DBL_MAX;   //终点在未收录前默认距离为无穷大
    endGridPoint.parent = nullptr;
}

void A_Star::path_plan(){  //被父节点干沉默了

    waypoint.clear();

    openList.clear();

    closeList = std::vector<std::vector<Node>> (gridMap.grid_rows, std::vector<Node>(gridMap.grid_rows));   //默认行比列多，所以直接覆盖整个栅格地图

    searchedList = std::vector<std::vector<bool>> (gridMap.grid_rows, std::vector<bool>(gridMap.grid_rows, false));

    openList.insert(startGridPoint);

    while(openList.size()){
        Node minNode = *(openList.begin());   //这里返回的是迭代器，再解引用为Node
        closeList[(int)minNode.coordinate.y()][(int)minNode.coordinate.x()] = minNode;
        Node* minNode_ptr = &closeList[(int)minNode.coordinate.y()][(int)minNode.coordinate.x()];
        searchedList[minNode.coordinate.y()][minNode.coordinate.x()] = true;
        openList.erase(minNode);

        if(minNode.coordinate == endGridPoint.coordinate)
        {
            endGridPoint = minNode;
            qDebug()<<"搜索完成\n";
            break;
        }

        for(Node neighborNode : get_neighbors(minNode)){ //*minNode为只读类型的const Node     //需要新增规则，去掉角点的邻接点

            if(gridMap.obsMap((int)neighborNode.coordinate.y(), (int)neighborNode.coordinate.x())){
                continue;
            }
            else if((neighborNode.coordinate - minNode.coordinate).norm() != 1){ //角点判断
                if(gridMap.obsMap((int)neighborNode.coordinate.y(), (int)minNode.coordinate.x()) ||
                   gridMap.obsMap((int)minNode.coordinate.y(), (int)neighborNode.coordinate.x()) ){
                    continue;
                }
            }

            if(searchedList[neighborNode.coordinate.y()][neighborNode.coordinate.x()]){
                continue;
            }

            float heuristicValue = heuristic_Function(minNode);

            float newcost = minNode.cost + (minNode.coordinate - neighborNode.coordinate).norm() + heuristicValue;
            if(check_node(neighborNode) != nullptr){
                if(newcost >= check_node(neighborNode)->cost){
                    continue;
                }
                else if(newcost < check_node(neighborNode)->cost){
                    openList.erase(*check_node(neighborNode));
                }
            }
            neighborNode.cost = newcost;

            neighborNode.parent = minNode_ptr;
            openList.insert(neighborNode);
        }
    }

    const Node* childNode = &endGridPoint; //waypoint从终点遍历回原点，整个容器是倒序的
    while(childNode!= nullptr){
        waypoint.push_back(childNode);
        childNode = childNode->parent;
    }
    std::reverse(waypoint.begin(),waypoint.end());  //翻转元素
}

std::vector<Node> A_Star::get_neighbors(const Node &node){    //边缘检测也要加
    std::vector<Node> neighborNodes;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(j == 0 && i == 0){
                continue;
            }
            Eigen::Vector2f neiborcoord = node.coordinate + Eigen::Vector2f(i, j);
            if(neiborcoord.x() >= gridMap.grid_cols ||
                neiborcoord.y() >= gridMap.grid_rows ||
                neiborcoord.x()< 0||neiborcoord.y() < 0)
            {
                continue;
            } //边缘检测
            neighborNodes.push_back(Node(neiborcoord, DBL_MAX,nullptr));//空指针标记一下
        }
    }
    return neighborNodes;
}

const Node* A_Star::check_node(Node _node){
    for(const Node& s : openList)
        if(_node.coordinate == s.coordinate)
        {
            return &s;
        }
    return nullptr;
}


float A_Star::heuristic_Function(Node _minnode) { //启发值=当前到终点距离+障碍物靠近惩罚
    float heuristicValue = (_minnode.coordinate - endGridPoint.coordinate).norm();    //欧氏距离
    float fstPenaltyValue = gridMap.fstHeuGridMap((int)_minnode.coordinate.y(),(int)_minnode.coordinate.x());
    float sedPenaltyValue = gridMap.sedHeuGridMap((int)_minnode.coordinate.y(),(int)_minnode.coordinate.x());
    if(fstPenaltyValue){   //先行后列
        heuristicValue += fstPenaltyValue;
    }
    else if(sedPenaltyValue){
        heuristicValue += sedPenaltyValue;
    }
    return heuristicValue;
}

Node::Node(Eigen::Vector2f _coordinate, float _cost, Node* _parent){
    coordinate = _coordinate; //这里的坐标是栅格地图坐标
    cost = _cost;
    parent = _parent;   //用于指向父节点
}

void openListLog(){
    // std::cout<<"openList:";
    // for (auto &_node : Dijkstra::openList)
    //     std::cout<<'(' <<_node.coordinate.x()<<','<<_node.coordinate.y() << ')' << _node.cost <<',';
    // std::cout<<std::endl;
}
