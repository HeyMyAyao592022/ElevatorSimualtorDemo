#include "viewmodel.h"

/// ��ʼ��
QMutex ViewModel::mutex;
QSharedPointer<ViewModel> ViewModel::instance;

ViewModel::ViewModel(QObject *parent)
    : QObject(parent)
{
    /// passenger list
    this->passengerList = PassengerList::getInstance();
    /// ����
    initSlots();
}

/**
 * @brief ViewModel::getInstance
 * @return view model ������ָ��
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
 * ֪ͨ����ϵͳ��ʼģ��
 */
void ViewModel::emitStartSignal()
{
    //qDebug()<<"emit start signal";
    emit startSimulating();
}

/// ����
void ViewModel::emitFinishSignal()
{
    emit finishSimulating();
}

/// ��
void ViewModel::bindElTypes(int elID, int typeID)
{
    Elevator::bindElTypes(elID, typeID);
}

/// @brief ��������

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

/// @brief �����³˿�
void ViewModel::emitGenerateSignal(int id, int from, int to)
{
    emit generateNewPassenger(id,from,to);
}

/// @brief �󶨳˿�
void ViewModel::emitConnectSiganl(int psgID, int elID)
{
    emit connectPassenger(psgID, elID);
}

/**
 * @brief ViewModel::objectModel
 * ��¶�� qml
 * @return
 */
PassengerList *ViewModel::objectModel()
{
    return passengerList.get();
}

/**
 * @brief ViewModel::��ȡ/���ó˿�����
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
 * @brief ViewModel::���������ٶ�
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
 * @brief ViewModel::������������
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
 * @brief ViewModel::��������
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

/// ʱ��
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
 * @brief ViewModel::��ȡ��ɵ�����
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
 * @brief ViewModel::�����źţ���ɼ���
 */
void ViewModel::emitFinishIniElSignal()
{
    //qDebug("in vm: emit finish signal");
    emit finishIniEls();
}

