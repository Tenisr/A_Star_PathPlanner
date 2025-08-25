#include "grid_map.h"

Grid_map::Grid_map(float _width, float _height, float _side, float _fPR,float _fPV,float _sPR, float _sPV){
    square_side = _side;
    grid_cols = (int)(_width/_side);//向下取整，没什么大问题
    grid_rows = (int)(_height/_side);
    fstPenaltyRadius = _fPR;
    fstPenaltyValue = _fPV;
    sedPenaltyRadius = _sPR;
    sedPenaltyValue = _sPV;
    qDebug()<<"square_side"<<square_side<<"grid_cols"<<grid_cols<<"grid_rows"<<grid_rows;

    obsMap = Eigen::MatrixXf::Zero(grid_rows, grid_cols);
}

void Grid_map::addObstacle(float _x, float _y, float _w, float _h){//遍历，后面可以改
    for(int i = 0; i < obsMap.cols();i++){
        for(int j = 0; j < obsMap.rows();j++){
            if(checkGridIntersectRect(gridCoordTrans(i, j), _x, _y, _w, _h))
                obsMap(j, i) = 1;//先行后列，行为纵坐标
        }
    }
}

void Grid_map::addObstacle(float _x, float _y, float _r){
    for(int i = 0; i < obsMap.cols();i++){
        for(int j = 0; j < obsMap.rows();j++){
            if(checkPointInCir(gridCoordTrans(i, j), _x, _y, _r))
                obsMap(j, i) = true;
        }
    }
}

bool Grid_map::checkGridIntersectRect(const Eigen::Vector2f& Point_coord, float Rect_x, float Rect_y, float Rect_w, float Rect_h){    //明显有问题，当障碍物小于栅格时无法检测到

    if(abs(Point_coord.x() - Rect_x) < (Rect_w + square_side) / 2  && abs(Point_coord.y() - Rect_y) < (Rect_h + square_side) / 2)
        return true;
    else
        return false;
}

bool Grid_map::checkPointInCir(Eigen::Vector2f Point_coord, float Cir_x, float Cir_y, float Cir_r){
    if(pow(Point_coord.x()-Cir_x, 2) + pow(Point_coord.y()-Cir_y, 2) < pow(Cir_r, 2))
        return true;
    else{
        Eigen::Vector2f Point_tf = Point_coord + Eigen::Vector2f(-square_side/2, -square_side/2);
        Eigen::Vector2f Point_tr = Point_coord + Eigen::Vector2f(square_side/2, -square_side/2);
        Eigen::Vector2f Point_bf = Point_coord + Eigen::Vector2f(-square_side/2, square_side/2);
        Eigen::Vector2f Point_tb = Point_coord + Eigen::Vector2f(square_side/2, square_side/2);
        if(pow(Point_tf.x()-Cir_x, 2) + pow(Point_tf.y()-Cir_y, 2) < pow(Cir_r, 2)||
            pow(Point_tr.x()-Cir_x, 2) + pow(Point_tr.y()-Cir_y, 2) < pow(Cir_r, 2)||
            pow(Point_bf.x()-Cir_x, 2) + pow(Point_bf.y()-Cir_y, 2) < pow(Cir_r, 2)||
            pow(Point_tb.x()-Cir_x, 2) + pow(Point_tb.y()-Cir_y, 2) < pow(Cir_r, 2)
            )
            return true;
        else
            return false;
    }
}

void Grid_map::setHeuristicMap(float _radius, float _penalty,Eigen::MatrixXf &heuMap,Eigen::MatrixXf &obsMap){
    float radius = round(_radius/square_side);  //半径四舍五入
    for(int i = 0; i <= obsMap.cols() - radius ;i++){  //这里卷积核的处理必须要加等于条件否则会漏掉边界
        for(int j = 0; j <= obsMap.rows() - radius;j++){
            auto kernal = obsMap.block(j,i,radius,radius);   //卷积核默认为正方形，这里返回的是对原矩阵的引用，注意为auto类型，如果使用Eigen::MatrixXf会成为值拷贝
            auto kernal_copy = heuMap.block(j,i,radius,radius); //从来没感觉自己写得这么史过
            if((kernal.array() == 1).any()){
                kernal_copy.setConstant(_penalty);//设置惩罚值
            }
        }
    }
}

Eigen::Vector2f Grid_map::gridCoordTrans(int grid_col, int grid_row){    //这里返回的是中心坐标
    return Eigen::Vector2f((grid_col+0.5f)*square_side, (grid_row+0.5f)*square_side);
}
