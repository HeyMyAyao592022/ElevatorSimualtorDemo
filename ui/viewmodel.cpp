#include "viewmodel.h"

/// 初始化
QMutex ViewModel::mutex;
QSharedPointer<ViewModel> ViewModel::instance;

ViewModel::ViewModel(QObject *parent)
    : QObject(parent)
{
    /// passenger list
    this->passengerList = PassengerList::getInstance();
    /// 连接
    initSlots();
}

/**
 * @brief ViewModel::getInstance
 * @return view model 的智能指针
 */
QSharedPointer<ViewModel> &ViewModel::getInstance()
{
    if (instance.isNull())
    {
        QMutexLocker mutexLocker(&mutex);
        if (instance.isNull())
            instance = QSharedPointer<ViewModel>(new ViewModel());
    }
    return instance;
}

/**
 * @brief ViewModel::emitIniElSignal
 */
void ViewModel::emitIniElSignal()
{
    //qDebug("in vm: emit signal");
    emit iniElsSignal();
}

/**
 * @brief ViewModel::emitStartSignal
 * 通知整个系统开始模拟
 */
void ViewModel::emitStartSignal()
{
    //qDebug()<<"emit start signal";
    emit startSimulating();
}

/// 结束
void ViewModel::emitFinishSignal()
{
    emit finishSimulating();
}

/// 绑定
void ViewModel::bindElTypes(int elID, int typeID)
{
    Elevator::bindElTypes(elID, typeID);
}

/// @brief 设置类型

void ViewModel::setTypesNum(int num)
{
    Elevator::setTypesNum(num);
}

void ViewModel::iniTypes(int typeID, int floor)
{
    Elevator::appTypeRange(typeID, floor);
}

void ViewModel::setTypeDescribe(int id, QString str)
{
    Elevator::setTypeDescribe(id, str);
}

/// @brief 生成新乘客
void ViewModel::emitGenerateSignal(int id, int from, int to)
{
    emit generateNewPassenger(id,from,to);
}

/// @brief 绑定乘客
void ViewModel::emitConnectSiganl(int psgID, int elID)
{
    emit connectPassenger(psgID, elID);
}

/**
 * @brief ViewModel::objectModel
 * 暴露给 qml
 * @return
 */
PassengerList *ViewModel::objectModel()
{
    return passengerList.get();
}

/**
 * @brief ViewModel::获取/设置乘客数量
 */
int ViewModel::getPassengerNumber()
{
    return Passenger::getPassengerNumber();
}

void ViewModel::setPassengerNumber(int num)
{
    if(num != getPassengerNumber()){
        Passenger::setPassengerNumber(num);
        emit passengerNumberChanged();
    }
}

/**
 * @brief ViewModel::电梯运行速度
 */
int ViewModel::getRunningInterval() const
{
    return Elevator::getRunningInterval();
}

void ViewModel::setRunningInterval(int val)
{
    if(val != getRunningInterval()){
        val = val*10;
        Elevator::setElevatorInterval(val);
        emit runningIntervalChanged();
    }
}

/**
 * @brief ViewModel::电梯满载人数
 */
int ViewModel::getElMaxNum() const
{
    return Elevator::getMaxNumber();
}

void ViewModel::setElMaxNum(int num)
{
    Elevator::setMaxNumber(num);
    emit elMaxNumChanged();
}

/**
 * @brief ViewModel::电梯数量
 */
int ViewModel::getElNum() const
{
    return Elevator::getElevatorNumber();
}

void ViewModel::setElNum(int num)
{
    Elevator::setElevatorNumber(num);
    emit elNumChanged();
}

/// 时间
void ViewModel::setTime(QDateTime start, QDateTime end)
{
    times.first = start;
    times.second = end;
}

QString ViewModel::getTotalTime()
{
    return totalTime;
}

void ViewModel::setTotalTime(QString time)
{
    totalTime = time;
}

/**
 * @brief ViewModel::获取完成的数量
 */
int ViewModel::getFinishedNumber()
{
    return Passenger::getFinishedNumber();
}

/**
 * @brief ViewModel::initSlots
 */
void ViewModel::initSlots()
{

}

/**
 * @brief ViewModel::iniEls
 */
void ViewModel::iniEls()
{
    auto elevators = Elevator::getElevators();
    foreach(auto el, *elevators){
        connect(el,&Elevator::unLoadPassengerSignal,this,[=](){
            emit finishedNumberChanged();
        });
    }
}

/**
 * @brief ViewModel::发出信号，完成加载
 */
void ViewModel::emitFinishIniElSignal()
{
    //qDebug("in vm: emit finish signal");
    emit finishIniEls();
}

