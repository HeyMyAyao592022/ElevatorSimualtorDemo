#include "Passenger.h"

/// 初始化
int Passenger::passengerNumber = 50;
int Passenger::finishedNumber = 0;
QMutex Passenger::mutex;

Passenger::Passenger(QObject *parent, int id)
	: QObject(parent)
    , currentStatue(PASSENGER_STATUS::WAITING)
    , passengerID(id)
    , elevatorID(DEAULT_ELEVATOR_ID)
{
    initFromAndTo();
}

Passenger::~Passenger()
{

}

/**
 * @brief from get set
 */
int Passenger::getFrom() const
{
    return from;
}

void Passenger::setFrom(int newFrom)
{
    from = newFrom;
}

/**
 * @brief to get set
 */
int Passenger::getTo() const
{
    return to;
}

void Passenger::setTo(int newTo)
{
    to = newTo;
}

/**
 * @brief passenger id get set
 */
int Passenger::getPassengerID() const
{
    return passengerID;
}

void Passenger::setPassengerID(int newPassengerID)
{
    passengerID = newPassengerID;
}

/**
 * @brief elevator id get set
 */
int Passenger::getElevatorID() const
{
    return elevatorID;
}

void Passenger::setElevatorID(int newElevatorID)
{
    elevatorID = newElevatorID;
    emit elIDChanged();
}

/**
 * @brief Passenger::waitingTime
 * 总共等待时间
 * @return
 */
const QTime& Passenger::waitingTime() const
{
    return clock;
}

/**
 * @brief Passenger::name
 * 返回名字的 string
 * @return
 */
const QString Passenger::name() const
{
    return QString("%1").arg(this->passengerID,2,10,QLatin1Char('0'));
}

/**
 * @brief Passenger::initFromAndTo
 * 初始化
 */
void Passenger::initFromAndTo()
{
    QTime time;
    time = QTime::currentTime();//利用系统时间生成随机数种子
    qsrand(time.msec() + time.second() * 1000);
    from = qrand() % (MAX_FLOOR-10) + 10;  // 随机起点10~40层
    to = qrand() % 10 + 1;//随机终点1~10层
    passengerDirection = (from < to ? ELEVATOR_DIRECTION::UP : ELEVATOR_DIRECTION::DOWN);//乘客方向
}

/**
 * @brief Passenger::requireTime
 * 上下电梯需要的时间
 * @return
 */
int Passenger::requireTime() const
{
    return 2;
}

/**
 * @brief Passenger::已完成的乘客数量
 */
int Passenger::getFinishedNumber()
{
    return finishedNumber;
}

void Passenger::setFinishedNumber(int newFinishedNumber)
{
    mutex.lock();
    finishedNumber += newFinishedNumber;
    mutex.unlock();
}

void Passenger::addFinishedNumber(int num)
{
    mutex.lock();
    finishedNumber += num;
    if(finishedNumber >= passengerNumber)
    mutex.unlock();
}

/**
 * @brief 乘客数量
 */
int Passenger::getPassengerNumber()
{
    return passengerNumber;
}

void Passenger::setPassengerNumber(int newPassengerNumber)
{
    passengerNumber = newPassengerNumber;
    //qDebug()<<"psg n "<<passengerNumber;
}

/**
 * @brief 乘电梯次数
 */
int Passenger::getTakingTime() const
{
    return takingTime;
}

void Passenger::setTakingTime(int newTakingTime)
{
    takingTime = newTakingTime;
}

/**
 * @brief Passenger::getPassengerDirection
 * @return
 */
ELEVATOR_DIRECTION Passenger::getPassengerDirection() const
{
    ///qDebug()<<QString("passenger %1 is moving %2").arg(passengerID).arg(passengerDirection);
    return passengerDirection;
}

