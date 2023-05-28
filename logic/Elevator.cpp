#include "Elevator.h"

/// 初始化
int Elevator::runningInterval = 2000;
QVector<Elevator *> Elevator::elevators;
QMutex Elevator::mutex;
/// const float Elevator::timerInterval = 0.5;
int Elevator::maxNumber = 10;
QVector<ElevatorType> Elevator::elevatorTypes = QVector<ElevatorType>();
int Elevator::elevatorNumber = 10;
QMap<int, int> Elevator::bindElevatorTypes = QMap<int, int>();

Elevator::Elevator(QObject *parent, ELEVATOR_TYPE type, int id)
    : QObject(parent), runningProgress(0), type(type), currentFloor(1), planMaxFloor(-1), planMinFloor(41), direction(ELEVATOR_DIRECTION::STOP), movingState(ELEVATOR_DIRECTION::STOP), id(id), passengerNumber(0), typeDescribe(""), totalDistance(0)
{
    /// 初始化
    initElevator();
}

Elevator::Elevator(QObject *parent, int id)
    : QObject(parent), runningProgress(0), currentFloor(1), planMaxFloor(-1), planMinFloor(41), direction(ELEVATOR_DIRECTION::STOP), movingState(ELEVATOR_DIRECTION::STOP), id(id), passengerNumber(0)
{
    /// 初始化
    initElevator();
}

Elevator::~Elevator()
{
}

/**
 * @brief Elevator::当前的总方向
 */
int Elevator::getDirection() const
{
    return direction;
}

ELEVATOR_DIRECTION Elevator::getElevatorDirection() const
{
    return direction;
}

void Elevator::setDirection(ELEVATOR_DIRECTION newCurrentDirection)
{
    // qDebug()<<QString("in El: El%1 dir change to %2").arg(id).arg(newCurrentDirection);
    direction = newCurrentDirection;
    emit directionChanged();
}

/**
 * @brief Elevator::当前电梯的状态
 */
ELEVATOR_DIRECTION Elevator::getCurrentMovingState() const
{
    return movingState;
}

int Elevator::getMovingState() const
{
    return movingState;
}

void Elevator::setCurrentMovingState(ELEVATOR_DIRECTION newCurrentMoveDirection)
{
    /// 修改运行方向
    movingState = newCurrentMoveDirection;
    emit movingStateChanged();
}

/**
 * @brief Elevator::运行速度
 */
int Elevator::getRunningInterval()
{
    return runningInterval;
}

void Elevator::setRunningInterval(int newRunningInterval)
{
    runningInterval = newRunningInterval;
}

/**
 * @brief Elevator::电梯类型
 */
int Elevator::getType() const
{
    return type;
}

// int Elevator::getElevatorType() const
//{
//     return type;
// }

void Elevator::setType(int newType)
{
    type = newType;
}

// QString Elevator::getTypeDescribe() const;
//{
//     return typeDescribe;
// }

/// id
int Elevator::getID() const
{
    return id;
}

/**
 * @brief Elevator::LoadPassenger 装货
 * @param passenger
 */
void Elevator::loadPassenger(Passenger *passenger)
{
    Q_UNUSED(passenger);
}

/**
 * @brief Elevator::loadPassenger
 * 装载乘客
 * @param queue 要进入电梯的乘客队列
 */
void Elevator::loadPassenger()
{
    auto &enterQueue = outerOrders[currentFloor];
    /// 遍历载入队列
    while (!enterQueue.empty())
    {
        // 出队
        auto passenger = enterQueue.front();
        enterQueue.pop_front();
        // qDebug()<<QString("El%1 is loading psg%2 in floor %3").arg(id).arg(passenger->getPassengerID()).arg(currentFloor);
        /// 根据乘客上下电梯时间，让当前子线程休眠
        int waitingTime = passenger->requireTime();
        /// 发出信号
        emit loadPassengerSignal(passenger->getPassengerID(), this->getID(), waitingTime);
        QThread::msleep(waitingTime * 1000);
        /// 添加到队列中，
        passengerQueue[passenger->getTo()].append(passenger);
        ++passengerNumber;
        /// 在 ui 列表中移除
        emit removePassengerFromQueue(passenger);
        /// 在命令集中加入指令.
        inOrders[passenger->getTo()] += 1;
    }
    emit passengerNumberChanged();
}

/**
 * @brief Elevator::UnloadPassenger
 * 卸客
 */
void Elevator::unLoadPassenger()
{
    /// 将要在该层下电梯的乘客全部清除
    int numUnLoad = 0;

    auto &currentQueue = passengerQueue[currentFloor];
    numUnLoad = currentQueue.size();
    /// 遍历队列
    while (!currentQueue.empty())
    {
        /// 等待,线程休眠
        auto passenger = currentQueue.front();
        int waitingTime = passenger->requireTime();
        emit passengerExitSignal(id, passenger->getPassengerID(), waitingTime);
        QThread::msleep(waitingTime * 1000);
        /// 出队
        currentQueue.pop_front();
        --passengerNumber;
        /// 更新指令集
        inOrders[currentFloor] -= 1;
    }
    Passenger::setFinishedNumber(numUnLoad);
    /// 发出信号，通知 ui 更新
    emit passengerNumberChanged();
    emit unLoadPassengerSignal(numUnLoad);
}

/**
 * @brief Elevator::hasUnloaded 该层有乘客下电梯
 * @param floor
 * @return
 */
bool Elevator::hasUnloaded(int floor)
{
    if (!passengerQueue.contains(floor))
        return false;
    return !passengerQueue[floor].empty();
}

/// 获得当前电梯上的乘客数，InElevatorPassengers的长度
int Elevator::getPassengerNumber()
{
    return passengerNumber;
}

/**
 * @brief Elevator::当前楼层
 */
int Elevator::getCurrentFloor() const
{
    return currentFloor;
}

void Elevator::setCurrentFloor(int newCurrentFloor)
{
    if (currentFloor == newCurrentFloor)
        return;
    currentFloor = newCurrentFloor;
    emit currentFloorChanged();
}

void Elevator::resetCurrentFloor()
{
    setCurrentFloor(1);
}

/**
 * @brief plan min
 */
int Elevator::getPlanMinFloor() const
{
    return planMinFloor;
}

void Elevator::setPlanMaxFloor(int value)
{
    this->planMaxFloor = value;
    emit PlanMaxFloorChanged();
}

void Elevator::setPlanMinFloor(int value)
{
    this->planMinFloor = value;
    emit PlanMinFloorChanged();
}

/// 获取总距离
int Elevator::getTotalDistance() const
{
    return totalDistance;
}

int Elevator::getConsume() const
{
    return totalDistance * runningInterval / 100;
}

/// 描述
void Elevator::setDescribe(QString str)
{
    typeDescribe = str;
}

QString Elevator::getDescribe() const
{
    return typeDescribe;
}

/**
 * @brief plan max
 */
int Elevator::getPlanMaxFloor() const
{
    return planMaxFloor;
}

/**
 * @brief Elevator::running
 * 电梯运行
 */
void Elevator::running()
{
    /// @brief 到达新楼层，更新 floor

    if (movingState == ELEVATOR_DIRECTION::UP)
        /// 方向向上，楼层增加
        /// ++currentFloor;
        runUp(); /// 在这个函数中，current floor ++,并发出信号
    else if (movingState == ELEVATOR_DIRECTION::DOWN)
        /// 方向向上，楼层减少
        runDown(); /// 在这个函数中，current floor --,并发出信号

    /// @brief 判断是否需要载客卸客

    /// 外部有乘客要进入，或者内部有乘客要出来
    bool enterFlag = outerOrders.contains(currentFloor) && !outerOrders[currentFloor].empty();
    bool exitFlag = !passengerQueue[currentFloor].empty();
    if (enterFlag || exitFlag)
    {
        /// 若需要载客卸客,则电梯运行状态为 stop
        this->setCurrentMovingState(ELEVATOR_DIRECTION::STOP);
        /// 在子线程中执行载客卸客操作
        QtConcurrent::run([=]()
                          {
            /// 先进行出电梯操作.
            if(exitFlag)
                unLoadPassenger();
            /// 再执行入电梯操作.
            if(enterFlag)
                loadPassenger();
            /// 判断是否需要继续运行
            checkKeepMoving();
            /// 发出信号，重新启动定时器
            if(direction != ELEVATOR_DIRECTION::STOP){
                /// 让电梯重新启动.
                setCurrentMovingState(getElevatorDirection());
                emit keepMoving();
            } });
    }
    else
    {
        /// 检查
        checkKeepMoving();
    }

    /// @brief 若不需要载客卸客，电梯继续运行
    if (movingState != ELEVATOR_DIRECTION::STOP)
    {
        /// 让电梯继续运行
        emit keepMoving();
    }
}

/**
 * @brief Elevator::setElevatorInterval
 * 设置电梯上下时间
 * @param interval
 */
void Elevator::setElevatorInterval(int interval)
{
    Elevator::runningInterval = interval;
}

/**
 * @brief Elevator::最大搭载人数
 */
void Elevator::setMaxNumber(int num)
{
    // maxNumber = num;
    maxNumber = num;
}

int Elevator::getMaxNumber()
{
    return Elevator::maxNumber;
}

/**
 * @brief Elevator::initElevator
 * 初始化
 */
void Elevator::initElevator()
{
    /// 根据 type 初始化乘客队列
    int typeIndex = bindElevatorTypes[id];
    auto &range = elevatorTypes[typeIndex].range;

    /// 绑定范围
    setType(typeIndex);
    typeDescribe = elevatorTypes[typeIndex].describe;
    // qDebug()<<"in El: e"<<id<<" d:"<<typeDescribe;
    for (auto i = range.begin(); i != range.end(); i++)
    {
        passengerQueue.insert(i.key(), QQueue<Passenger *>());
        // qDebug()<<"in El: el"<<id<<" binds "<<i.key();
    }

    /// 绑定信号和槽
    connect(this, &Elevator::keepMoving, this, [=]()
            {
        /// 运行前发出信号
        switch (movingState) {
        case UP:
            emit approachNew(currentFloor + 1, id);
            break;
        case DOWN:
            emit approachNew(currentFloor - 1, id);
            break;
        case STOP:
            break;
        }
        QTimer::singleShot(runningInterval,this,&Elevator::running); });

    connect(this, &Elevator::removePassengerFromQueue, this, [=](Passenger *passenger)
            {
        auto queue = PassengerList::getInstance();
        queue->removePassenger(passenger); });
}

/// 检查是否继续
void Elevator::checkKeepMoving()
{
    /// 如果电梯已经到了计划中的最高层/最底层,
    /// 或者已经到达范围极限，那就修改电梯方向.
    if (direction == ELEVATOR_DIRECTION::UP && planMaxFloor == currentFloor)
    {
        /// 电梯已经空了
        if (passengerNumber == 0)
        {
            setDirection(ELEVATOR_DIRECTION::STOP);
            setCurrentMovingState(ELEVATOR_DIRECTION::STOP);
        }
        /// 未空
        else
        {
            setDirection(ELEVATOR_DIRECTION::DOWN);
            setCurrentMovingState(ELEVATOR_DIRECTION::DOWN);
            /// 确定最低目标楼层
            /// 扫描内部乘客
            planMinFloor = 41;
            for (auto i = passengerQueue.begin(); i != passengerQueue.end(); i++)
            {
                if (!i.value().empty() && planMinFloor > i.key())
                {
                    planMinFloor = i.key();
                }
            }
            /// 扫描外部乘客
            for (auto i = outerOrders.begin(); i != outerOrders.end(); i++)
            {
                if (!i.value().empty() && planMinFloor > i.key())
                {
                    planMinFloor = i.key();
                }
            }
            setPlanMinFloor(planMinFloor);
        }
    }
    else if (direction == ELEVATOR_DIRECTION::DOWN && planMinFloor == currentFloor)
    {
        /// 电梯已经空了
        if (passengerNumber == 0)
        {
            setDirection(ELEVATOR_DIRECTION::STOP);
            setCurrentMovingState(ELEVATOR_DIRECTION::STOP);
        }
        /// 未空
        else
        {
            setDirection(ELEVATOR_DIRECTION::UP);
            setCurrentMovingState(ELEVATOR_DIRECTION::UP);
            /// 确定最高目标楼层
            /// 扫描内部电梯
            planMaxFloor = -1;
            for (auto i = passengerQueue.begin(); i != passengerQueue.end(); i++)
            {
                if (!i.value().empty() && planMaxFloor < i.key())
                    planMaxFloor = i.key();
            }
            /// 扫描外部电梯
            for (auto i = outerOrders.begin(); i != outerOrders.end(); i++)
            {
                if (!i.value().empty() && planMaxFloor < i.key())
                    planMaxFloor = i.key();
            }
            setPlanMaxFloor(planMaxFloor);
        }
    }
    else if (direction == ELEVATOR_DIRECTION::STOP)
    {
        /// 扫描内部电梯
        int planFloor = 41;
        for (auto i = passengerQueue.begin(); i != passengerQueue.end(); i++)
        {
            if (!i.value().empty() && planFloor < i.key())
                planMaxFloor = i.key();
        }
    }
}

/**
 * @brief Elevator::getElevatorNumber
 * @return
 */
int Elevator::getElevatorNumber()
{
    return elevatorNumber;
}

void Elevator::setElevatorNumber(int newElevatorNumber)
{
    elevatorNumber = newElevatorNumber;
}

/**
 * @brief Elevator::添加外部命令：接乘客
 * @param passenger
 * @return
 */
bool Elevator::addOutOrder(Passenger *passenger)
{
    int floor = passenger->getFrom();
    /// 满载或者无法到达目标层，则返回 false
    if (!ableToReach(floor) && getPassengerNumber() >= maxNumber)
        return false;
    /// 给 map 添加 value
    if (!outerOrders.contains(floor))
        outerOrders[floor] = QVector<Passenger *>();
    outerOrders[floor].append(passenger);
    /// 如果当前电梯停止，那么电梯的运行方向将由乘客的位置决定.
    if (direction == ELEVATOR_DIRECTION::STOP)
    {
        /// 修改电梯运行方向
        ELEVATOR_DIRECTION to;
        floor > currentFloor ? to = ELEVATOR_DIRECTION::UP : to = ELEVATOR_DIRECTION::DOWN;
        setDirection(to);
        setCurrentMovingState(to);
        /// 让电梯动起来
        emit keepMoving();
    }
    /// 更新 max & min
    if (direction == ELEVATOR_DIRECTION::UP && planMaxFloor < floor)
    {
        setPlanMaxFloor(floor);
    }
    else if (direction == ELEVATOR_DIRECTION::DOWN && planMinFloor > floor)
    {
        setPlanMinFloor(floor);
    }
    /// 绑定乘客
    passenger->setElevatorID(id);
    /// 发出信号
    emit connectPassenger(passenger->getPassengerID(), id);
    return true;
}

bool Elevator::ableToReach(int floor)
{
    // qDebug()<<QString("in El: El%1 able to reach floor%2 ? -> %3").arg(id).arg(floor).arg(passengerQueue.contains(floor));
    return passengerQueue.contains(floor);
}

/**
 * @brief Elevator::运行进度
 * @return
 */
double Elevator::getRunningProgress() const
{
    return runningProgress;
}

void Elevator::setRunningProgress()
{
    runningProgress += 1.0;
    emit runningProgressChanged();
}

/**
 * @brief Elevator::getElevators
 * 获取电梯数组
 * @return
 */
QVector<Elevator *> *Elevator::getElevators(QObject *parent)
{
    if (Elevator::elevators.empty())
    {
        mutex.lock();
        if (elevatorTypes.empty())
        {
            iniDefaultElevatorTypes();
        }
        if (elevators.empty())
        {
            for (int i = 0; i < Elevator::elevatorNumber; i++)
            {
                Elevator::elevators.append(new Elevator(parent, i));
            }
        }
        mutex.unlock();
    }
    return &(Elevator::elevators);
}

void Elevator::runUp()
{
    ++currentFloor;
    ++totalDistance;
    emit currentFloorChanged();
}

void Elevator::runDown()
{
    --currentFloor;
    ++totalDistance;
    emit currentFloorChanged();
}

/**
 * @brief Elevator::getElevators
 * 得到 qlist，用于注册
 * @return
 */
QList<QObject *> Elevator::getElevatorsList()
{
    QList<QObject *> list;
    int size = Elevator::elevators.size();
    for (int i = 0; i < size; i++)
    {
        list.append(elevators[i]);
    }
    return list;
}

/**
 * @brief Elevator::getElevatorTypes
 * @return
 */
QVector<ElevatorType> &Elevator::getElevatorTypes()
{
    if (elevatorTypes.empty())
    {
        iniDefaultElevatorTypes();
    }
    return elevatorTypes;
}

void Elevator::iniDefaultElevatorTypes()
{
    for (int i = 0; i <= 4; i++)
    {
        elevatorTypes.append(ElevatorType(i));
    }

    for (int i = 0, j = 0; i < elevatorNumber; j++)
    {
        bindElevatorTypes.insert(i++, j);
        bindElevatorTypes.insert(i++, j);
    }

    /// @brief type 0
    for (int i = 1; i <= MAX_FLOOR; i++)
        elevatorTypes[0].append(i);

    /// @brief type 1
    /// 1层
    elevatorTypes[1].append(1);
    /// 25~40
    for (int i = 25; i <= MAX_FLOOR; i++)
        elevatorTypes[1].append(i);

    /// @brief type 2
    /// 1~25
    for (int i = 1; i <= 25; i++)
        elevatorTypes[2].append(i);

    /// @brief type 3
    /// 1层
    elevatorTypes[3].append(1);
    /// 2~40 偶数
    for (int i = 2; i <= MAX_FLOOR; i += 2)
        elevatorTypes[3].append(i);

    /// @brief type 4
    /// 1~39 奇数
    for (int i = 1; i <= MAX_FLOOR; i += 2)
        elevatorTypes[4].append(i);
}

/// @brief types

void Elevator::setTypesNum(int num)
{
    mutex.lock();
    elevatorTypes.clear();
    for (int i = 0; i < num; i++)
    {
        elevatorTypes.append(ElevatorType(i));
    }
    mutex.unlock();
}

void Elevator::bindElTypes(int el, int type)
{
    bindElevatorTypes[el] = type;
}

void Elevator::appTypeRange(int typeID, int floor)
{
    elevatorTypes[typeID].append(floor);
}

void Elevator::setTypeDescribe(int id, QString str)
{
    elevatorTypes[id].describe = str;
}
