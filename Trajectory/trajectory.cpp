#include "trajectory.h"

Trajectory::Trajectory(std::vector<Eigen::Vector2f> _transPoints, Eigen::Vector2f startPoint, Eigen::Vector2f endPoint, float gridSize, std::vector<RealCorridor> realCorridorlist) {
    //路径点初始化,栅格转实际坐标
    wayPoints.push_back(startPoint);
    std::vector<Eigen::Vector2f>::iterator it = _transPoints.begin();
    while(it != _transPoints.end()){
        Eigen::Vector2f transPoint = *it;
        transPoint = transPoint * gridSize + Eigen::Vector2f(gridSize/2,gridSize/2);
        wayPoints.push_back(transPoint);
        it++;
    }
    wayPoints.push_back(endPoint);

    if(wayPoints.size() != 0){
        numPath = wayPoints.size() - 1; //numpath不能小于0！！！
    }
    else{
        numPath = 0;
    }
    std::cout<<"numPath:"<<numPath<<std::endl;

    flightCorridors = realCorridorlist;

    std::vector<float> lengthForPath(numPath);  //必须初始化
    timeForPath.resize(numPath);

    derivative = 4; //优化目标
    order = 5;  //阶数
    totalTime = 50; //总时长

    controlPoints.resize((order+1)*numPath);//控制点初始化

    float totalLength = 0;

    for(int i = 0; i < numPath; i++){   //这里的索引从0开始
        lengthForPath[i] = (wayPoints[i+1] - wayPoints[i]).norm();
        totalLength += lengthForPath[i];

    }

    // std::cout<<"lengthForPath:"<<std::endl;
    // for(uint i = 0; i < lengthForPath.size(); i++){ //注意循环条件是size()不是sizeof()...
    //     std::cout<<lengthForPath[i]<<std::endl;
    // }

    // std::cout<<"totalLength:"<<totalLength<<std::endl;

    for(int i = 0; i < numPath; i++){
        timeForPath[i] = totalTime /  totalLength * lengthForPath[i];
        // std::cout<<"timeForPath"<<timeForPath[i]<<std::endl;
    }

    // std::cout<<"timeForPath:"<<std::endl;
    // for(uint i = 0; i < timeForPath.size(); i++){
    //     std::cout<<timeForPath[i]<<std::endl;
    // }

    getQ();//获取Q矩阵
    getM();
}

void Trajectory::getConstraintsMartrix(qpOASES::real_t*& constraintMartrix){    //指针引用
    const int consRow = 2 * numPath * (order+1);    //约束矩阵列数

    const int consCol =  2*(4* numPath + 2) ;    //约束矩阵行数

    constraintMartrix = new qpOASES::real_t[consRow * consCol]; //申请约束矩阵,xy两个维度所以行数和列数都要翻倍，计算x时y全部为0

    for(int i = 0; i < consRow * consCol ;i++){
        constraintMartrix[i] =  0;   //全部初始化
    }
    //感觉还是有问题，应该记录结束的行号和列号再加就会好很多，这样写得太抽象了，算了写都写了等下一次吧

    //x方向
    //起点约束
    constraintMartrix[consRow*0 + 0] = 1;  //p
    constraintMartrix[consRow*1 + 0] = -1; //v
    constraintMartrix[consRow*1 + 1] = 1;
    constraintMartrix[consRow*2 + 0] = 1; //a
    constraintMartrix[consRow*2 + 1] = -2;
    constraintMartrix[consRow*2 + 2] = 1;
    //终点约束
    constraintMartrix[consRow*3 + (order+1)*numPath-1] = 1;  //p
    constraintMartrix[consRow*4 + (order+1)*numPath-2] = -1; //v
    constraintMartrix[consRow*4 + (order+1)*numPath-1] = 1;
    constraintMartrix[consRow*5 + (order+1)*numPath-3] = 1; //a
    constraintMartrix[consRow*5 + (order+1)*numPath-2] = -2;
    constraintMartrix[consRow*5 + (order+1)*numPath-1] = 1;
    int startAndEndCol = 6;
    //轨迹点约束
    int transCol = numPath-1;
    for(int i = 0; i < transCol; i++){
        int trapointCol = startAndEndCol+i;
        int trapointRow = (order+1)*(1+i);
        constraintMartrix[trapointCol*consRow + trapointRow] = 1;
    }
    //连续性约束
    for(int i = 0; i < transCol; i++){  //p
        int contiPointPCol = startAndEndCol+transCol+3*i;
        int contiPointPRow = (order+1)*i;
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order] = 1; //pi
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order+1] = -1; //pi+1
    }
    for(int i = 0; i < transCol; i++){  //v
        int contiPointPCol = startAndEndCol+transCol+3*i+1;
        int contiPointPRow = (order+1)*i;
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order-1] = -1;    //vi
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order] = 1;
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order+1] = 1;    //vi+1
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order+2] = -1;
    }
    for(int i = 0; i < transCol; i++){  //a
        int contiPointPCol = startAndEndCol+transCol+3*i+2;
        int contiPointPRow = (order+1)*i;
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order-2] = 1;   //ai
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order-1] = -2;
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order] = 1;
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order+1] = -1;   //ai+1
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order+2] = 2;
        constraintMartrix[contiPointPCol*consRow + contiPointPRow + order+3] = -1;
    }


    const int consColX = 4* numPath + 2;
    const int consRowX = numPath * (order+1);


    //y方向
    //起点约束
    constraintMartrix[consRow*consColX + consRowX+0] = 1;  //p
    constraintMartrix[consRow*(consColX+1) + consRowX+0] = -1; //v
    constraintMartrix[consRow*(consColX+1) + consRowX+1] = 1;
    constraintMartrix[consRow*(consColX+2) + consRowX+0] = 1; //a
    constraintMartrix[consRow*(consColX+2) + consRowX+1] = -2;
    constraintMartrix[consRow*(consColX+2) + consRowX+2] = 1;
    //终点约束
    constraintMartrix[consRow*(consColX+3) + consRowX+(order+1)*numPath-1] = 1;  //p
    constraintMartrix[consRow*(consColX+4) + consRowX+(order+1)*numPath-2] = -1; //v
    constraintMartrix[consRow*(consColX+4) + consRowX+(order+1)*numPath-1] = 1;
    constraintMartrix[consRow*(consColX+5) + consRowX+(order+1)*numPath-3] = 1; //a
    constraintMartrix[consRow*(consColX+5) + consRowX+(order+1)*numPath-2] = -2;
    constraintMartrix[consRow*(consColX+5) + consRowX+(order+1)*numPath-1] = 1;
    //轨迹点约束
    for(int i = 0; i < transCol; i++){
        int trapointCol = startAndEndCol+i;
        int trapointRow = (order+1)*(1+i);
        constraintMartrix[(consColX+trapointCol)*consRow + consRowX+trapointRow] = 1;
    }
    //连续性约束
    for(int i = 0; i < transCol; i++){  //p
        int contiPointPCol = startAndEndCol+transCol+3*i;
        int contiPointPRow = (order+1)*i;
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow  + order] = 1; //pi
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow  + order+1] = -1; //pi+1
    }
    for(int i = 0; i < transCol; i++){  //v
        int contiPointPCol = startAndEndCol+transCol+3*i+1;
        int contiPointPRow = (order+1)*i;
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order-1] = -1;    //vi
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order] = 1;
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order+1] = 1;    //vi+1
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order+2] = -1;
    }
    for(int i = 0; i < transCol; i++){  //a
        int contiPointPCol = startAndEndCol+transCol+3*i+2;
        int contiPointPRow = (order+1)*i;
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order-2] = 1;   //ai
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order-1] = -2;
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order] = 1;
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order+1] = -1;   //ai+1
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order+2] = 2;
        constraintMartrix[(consColX+contiPointPCol)*consRow + consRowX+contiPointPRow + order+3] = -1;
    }

    std::cout<<"constraintMartix:"<<std::endl;
    for(int i = 0; i < consCol; i++){
        for(int j = 0; j < consRow; j++){
            std::cout<<constraintMartrix[i*consRow+j]<<' ';
        }
        std::cout<<std::endl;
    }

};
void Trajectory::getHessian(qpOASES::real_t*& hessianMartrix){
    if(hessianMartrix != nullptr){
        delete[] hessianMartrix;
    }

    Eigen::MatrixXd MQM = M.transpose()* Q * M;

    hessianMartrix = new qpOASES::real_t[2*numPath*(order+1)*2*numPath*(order+1)];

    int hessianCols = 2*numPath*(order+1);
    for(int i = 0; i < numPath; i++){
        for(int j = 0; j < order+1; j++){
            int controlPointIndex = i*(order+1)+j;
            int XIndex = numPath*(order+1);
            for(int k = 0; k < 2*numPath*(order+1); k++){
                hessianMartrix[controlPointIndex*hessianCols+k] = MQM(controlPointIndex,k) / std::pow(timeForPath[i],7);
                hessianMartrix[(XIndex+controlPointIndex)*hessianCols+k] = MQM(XIndex+controlPointIndex,k) / std::pow(timeForPath[i],7);
                // hessianMartrix[controlPointIndex*hessianCols+k] = MQM(controlPointIndex,k)* timeForPath[i];
                // hessianMartrix[(XIndex+controlPointIndex)*hessianCols+k] = MQM(XIndex+controlPointIndex,k) *timeForPath[i];
            }
        }

    }

    // for(int i = 0; i < 2*numPath*(order+1); i++){
    //     for(int j = 0; j < 2*numPath*(order+1); j++){
    //         hessianMartrix[i*2*numPath*(order+1)+j] = MQM(i,j);
    //     }
    // }

    std::cout<<"Hessian Martrix:"<<std::endl;
    for(int i = 0; i < 2*numPath*(order+1); i++){
        for(int j = 0; j < 2*numPath*(order+1); j++){
            std::cout<<hessianMartrix[i*2*numPath*(order+1)+j]<<' ';
        }
        std::cout<<std::endl;
    }

};
void Trajectory::getEquationConstraints(qpOASES::real_t*& linnerConstraints){
    //x
    linnerConstraints = new qpOASES::real_t[2*(4*numPath+2)];

    for(int i = 0; i < 2*(4*numPath+2) ; i++){
        linnerConstraints[i] = 0;
    }

    //start p/v/a
    linnerConstraints[0] = (wayPoints.begin())->x();
    linnerConstraints[1] = 0;
    linnerConstraints[2] = 0;
    //end p/v/a
    linnerConstraints[3] =  (wayPoints.end()-1)->x();
    linnerConstraints[4] =  0;
    linnerConstraints[5] =  0;
    //trans p
    for(int i = 0; i < numPath-1; i++){
        linnerConstraints[6+i] = wayPoints[i+1].x();
    }

    int constrainXCol = 4*numPath+2;

    //y
    linnerConstraints[constrainXCol+0] = (wayPoints.begin())->y();
    linnerConstraints[constrainXCol+1] = 0;
    linnerConstraints[constrainXCol+2] = 0;

    linnerConstraints[constrainXCol+3] =  (wayPoints.end()-1)->y();
    linnerConstraints[constrainXCol+4] =  0;
    linnerConstraints[constrainXCol+5] =  0;
    for(int i = 0; i < numPath-1; i++){
        linnerConstraints[constrainXCol+6+i] = wayPoints[i+1].y();
    }


    std::cout<<"equationConstraints:"<<std::endl;
    for(int i = 0; i < 2*(4*numPath+2); i++){   //如果是静态数组的话可用(int)sizeof(linnerConstraints)/sizeof(linnerConstraints[0])查询元素个数
        std::cout<<linnerConstraints[i]<<' '<<std::endl;;
    }
};
void Trajectory::getQ(){    //这里把时间系数写到Q里面

    Q.resize(2*numPath*(order+1),2*numPath*(order+1));  //P为xy顺序排列
    Q.setZero();//全部置零，否则没有被qi覆盖到的元素会没被初始化
    switch(derivative){
    case 3:{

    }
        case 4:{
            for(int i = 0; i < numPath; i++)    //索引从0开始
            {
                Eigen::MatrixXd Qi = Eigen::MatrixXd::Zero(order+1,order+1);    //Eigen::MatrixXd(order+1, order+1) 创建临时对象，
                    //在这个对象上静态函数调用.Zero()，并不会修改已有对象,而是返回一个新的矩阵
                    //所以这里应该直接调用MatrixXd的方法构造

                int c, r = 0;
                for(r = 4; r < order+1 ; r++){  //从(4,4)开始
                    for(c = 4; c < order+1 ; c++){
                        double temp1 = factorial(r)/factorial(r-4);
                        double temp2 = factorial(c)/factorial(c-4);
                        double temp = std::pow(timeForPath[i], r + c - 7) / (r-4 + c-4 + 1);
                        Qi(r,c) = temp * temp1 * temp2 / std::pow(timeForPath[i],7);

                    }
                }
                // std::cout<<"Q" << i << ":\n"<<Qi<<std::endl;
                Q.block(i*(order+1),i*(order+1),order+1,order+1) = Qi;
            }
            Q.block(numPath*(order+1),numPath*(order+1),numPath*(order+1),numPath*(order+1)) = Q.block(0,0,numPath*(order+1),numPath*(order+1));

            // Eigen::IOFormat compactFormat(Eigen::StreamPrecision, 1);
            // std::cout<<"QMatrix:\n"<<Q.format(compactFormat)<<std::endl;
            break;
        }

        default:{
            qDebug()<<"优化目标不存在！";
            break;
        }
    }
};
void Trajectory::getM(){
    M.resize(2*numPath*(order+1),2*numPath*(order+1));  //P为xy顺序排列
    M.setZero();
    Eigen::MatrixXd Mi = Eigen::MatrixXd::Zero(order+1,order+1);
    if(order <= (int)M_MatrixList.size()){
        Mi = M_MatrixList[order];
    }
    else{
        qDebug()<<"优化阶数过高！未找到对应的映射矩阵！\n";
        return;
    }
    for(int i = 0; i < numPath; i++){
        M.block(i*(order+1),i*(order+1),order+1,order+1) = Mi;
    }
    M.block(numPath*(order+1),numPath*(order+1),numPath*(order+1),numPath*(order+1)) = M.block(0,0,numPath*(order+1),numPath*(order+1));
    Eigen::IOFormat compactFormat(Eigen::StreamPrecision, 1);
    // std::cout<<"MMatrix:\n"<<M.format(compactFormat)<<std::endl;
};

void Trajectory::solveProblem(){
    int nV = 2 * numPath * ( order + 1 ),
        nC = 2 * ( 4 * numPath + 2 ); //变量数nV，约束数nC

    qpOASES::QProblem solver(nV, nC);

    qpOASES::real_t* H = nullptr; //海森矩阵
    getHessian(H);

    qpOASES::real_t* A = nullptr; //约束矩阵
    getConstraintsMartrix(A);

    qpOASES::real_t* g = new qpOASES::real_t[nV];   //梯度向量
    for(int i = 0; i < nV; i++){
        g[i]  = 0;
    }

    qpOASES::real_t* lb = new qpOASES::real_t[nV];  //针对变量的约束向量下界
    // for(int i = 0; i < nV; i++){
    //     lb[i]  = -DBL_MAX;
    // }
    for(int i = 0; i < numPath; i++){
        for(int j = 0; j < order+1; j++){
            lb[i * (order+1) + j] = flightCorridors[i].TopLeft.x();
            lb[nV/2 + i * (order+1) + j] = flightCorridors[i].TopLeft.y();
        }
    }
    qpOASES::real_t* ub = new qpOASES::real_t[nV];  //针对变量的约束向量上界
    // for(int i = 0; i < nV; i++){
    //     ub[i]  = DBL_MAX;
    // }
    for(int i = 0; i < numPath; i++){
        for(int j = 0; j < order+1; j++){
            ub[i * (order+1) + j] = flightCorridors[i].BottomRight.x();
            ub[nV/2 + i * (order+1) + j] = flightCorridors[i].BottomRight.y();
        }
    }

    qpOASES::real_t* lbA = nullptr; //约束下边界
    getEquationConstraints(lbA);

    qpOASES::real_t* ubA = new qpOASES::real_t[nC]; //约束上边界
    for (int i = 0; i < nC; ++i)
        ubA[i] = lbA[i];

    qpOASES::int_t nWSR = 10000000;   //最大重试次数

    qpOASES::real_t* xOpt = new qpOASES::real_t[nV];

    auto errorcode = solver.init(H,g,A,lb,ub,lbA,ubA,nWSR);
    if(errorcode == qpOASES::SUCCESSFUL_RETURN)
    {
        solver.getPrimalSolution(xOpt);

        // std::cout << "the soloution is: " << *xOpt << std::endl;

        for(int i = 0; i < nV/2; i++){
            controlPoints[i].x() = xOpt[i];
            controlPoints[i].y() = xOpt[i+nV/2];
        }
    }

    else{
        qDebug() << "init failed" << errorcode;
        if (errorcode == qpOASES::RET_MAX_NWSR_REACHED) {
            qDebug() << "RET_MAX_NWSR_REACHED";
        }
        if (errorcode == qpOASES::RET_INIT_FAILED) {
            qDebug() << "RET_INIT_FAILED";
        }
        if (errorcode == qpOASES::RET_INVALID_ARGUMENTS) {
            qDebug() << "RET_INVALID_ARGUMENTS ";
        }
        if (errorcode == qpOASES::RET_INIT_FAILED_INFEASIBILITY) {
            qDebug() << "RET_INIT_FAILED_INFEASIBILITY";
        }
    }

    std::cout<<"controlPoints:"<<std::endl;
    for(int i = 0; i < (int)controlPoints.size(); i++){
        std::cout<<controlPoints[i].x()<<','<<controlPoints[i].y()<<std::endl;
    }

    delete[] H;
    delete[] A;
    delete[] g;
    delete[] ub;
    delete[] lb;
    delete[] lbA;
    delete[] ubA;
    delete[] xOpt;
}
