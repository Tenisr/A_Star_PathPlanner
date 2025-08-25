#ifndef TRAJECTORY_H
#define TRAJECTORY_H
#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <qpOASES.hpp>
#include <QDebug>
#include <float.h>
#include "bezier.h"
#include "corridor.h"


class Trajectory
{
public:
    Trajectory(std::vector<Eigen::Vector2f> _transPoints, Eigen::Vector2f startPoint, Eigen::Vector2f endPoint, float gridSize, std::vector<RealCorridor> realCorridorlist);
    std::vector<Eigen::Vector2f> wayPoints; //路径点
    std::vector<Eigen::Vector2f> controlPoints; //控制点
    int order; //阶数
    std::vector<float> timeForPath;//每段路径所消耗的时间
    void solveProblem();

private:

    int numPath;
    float totalTime;    //预计时间
    int derivative; //minimumsnap or minimumjerk
    std::vector<RealCorridor> flightCorridors;
    Eigen::MatrixXd Q;  //Q矩阵，赵学长给的Q实际上为Qi，且不包括时间系数，这里我想处理得直观一点就直接当做优化目标中的Q
    Eigen::MatrixXd M;  //M矩阵，事先写入内存


    void getConstraintsMartrix(qpOASES::real_t*& constraintMartrix);
    void getEquationConstraints(qpOASES::real_t*& linnerConstraints);
    void getQ();   //就工程应用而言Q并不需要很高的阶数，所以直接暴力枚举就可以了
    void getM();
    void getHessian(qpOASES::real_t*& hessianMartrix);  //获取海森矩阵，由于不用管系数其实就是分块汇总的Q矩阵
};

int factorial(int n);

#endif // TRAJECTORY_H
