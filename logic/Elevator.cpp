#include "Elevator.h"

/// ��ʼ��
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
    /// ��ʼ��
    initElevator();
}

Elevator::Elevator(QObject *parent, int id)
    : QObject(parent), runningProgress(0), currentFloor(1), planMaxFloor(-1), planMinFloor(41), direction(ELEVATOR_DIRECTION::STOP), movingState(ELEVATOR_DIRECTION::STOP), id(id), passengerNumber(0)
{
    /// ��ʼ��
    initElevator();
}

Elevator::~Elevator()
{
}

/**
 * @brief Elevator::��ǰ���ܷ���
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
 * @brief Elevator::��ǰ���ݵ�״̬
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
    /// �޸����з���
    movingState = newCurrentMoveDirection;
    emit movingStateChanged();
}

/**
 * @brief Elevator::�����ٶ�
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
 * @brief Elevator::��������
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
 * @brief Elevator::LoadPassenger װ��
 * @param passenger
 */
void Elevator::loadPassenger(Passenger *passenger)
{
    Q_UNUSED(passenger);
}

/**
 * @brief Elevator::loadPassenger
 * װ�س˿�
 * @param queue Ҫ������ݵĳ˿Ͷ���
 */
void Elevator::loadPassenger()
{
    auto &enterQueue = outerOrders[currentFloor];
    /// �����������
    while (!enterQueue.empty())
    {
        // ����
        auto passenger = enterQueue.front();
        enterQueue.pop_front();
        // qDebug()<<QString("El%1 is loading psg%2 in floor %3").arg(id).arg(passenger->getPassengerID()).arg(currentFloor);
        /// ���ݳ˿����µ���ʱ�䣬�õ�ǰ���߳�����
        int waitingTime = passenger->requireTime();
        /// �����ź�
        emit loadPassengerSignal(passenger->getPassengerID(), this->getID(), waitingTime);
        QThread::msleep(waitingTime * 1000);
        /// ��ӵ������У�
        passengerQueue[passenger->getTo()].append(passenger);
        ++passengerNumber;
        /// �� ui �б����Ƴ�
        emit removePassengerFromQueue(passenger);
        /// ������м���ָ��.
        inOrders[passenger->getTo()] += 1;
    }
    emit passengerNumberChanged();
}

/**
 * @brief Elevator::UnloadPassenger
 * ж��
 */
void Elevator::unLoadPassenger()
{
    /// ��Ҫ�ڸò��µ��ݵĳ˿�ȫ�����
    int numUnLoad = 0;

    auto &currentQueue = passengerQueue[currentFloor];
    numUnLoad = currentQueue.size();
    /// ��������
    while (!currentQueue.empty())
    {
        /// �ȴ�,�߳�����
        auto passenger = currentQueue.front();
        int waitingTime = passenger->requireTime();
        emit passengerExitSignal(id, passenger->getPassengerID(), waitingTime);
        QThread::msleep(waitingTime * 1000);
        /// ����
        currentQueue.pop_front();
        --passengerNumber;
        /// ����ָ�
        inOrders[currentFloor] -= 1;
    }
    Passenger::setFinishedNumber(numUnLoad);
    /// �����źţ�֪ͨ ui ����
    emit passengerNumberChanged();
    emit unLoadPassengerSignal(numUnLoad);
}

/**
 * @brief Elevator::hasUnloaded �ò��г˿��µ���
 * @param floor
 * @return
 */
bool Elevator::hasUnloaded(int floor)
{
    if (!passengerQueue.contains(floor))
        return false;
    return !passengerQueue[floor].empty();
}

/// ��õ�ǰ�����ϵĳ˿�����InElevatorPassengers�ĳ���
int Elevator::getPassengerNumber()
{
    return passengerNumber;
}

/**
 * @brief Elevator::��ǰ¥��
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

/// ��ȡ�ܾ���
int Elevator::getTotalDistance() const
{
    return totalDistance;
}

int Elevator::getConsume() const
{
    return totalDistance * runningInterval / 100;
}

/// ����
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
 * ��������
 */
void Elevator::running()
{
    /// @brief ������¥�㣬���� floor

    if (movingState == ELEVATOR_DIRECTION::UP)
        /// �������ϣ�¥������
        /// ++currentFloor;
        runUp(); /// ����������У�current floor ++,�������ź�
    else if (movingState == ELEVATOR_DIRECTION::DOWN)
        /// �������ϣ�¥�����
        runDown(); /// ����������У�current floor --,�������ź�

    /// @brief �ж��Ƿ���Ҫ�ؿ�ж��

    /// �ⲿ�г˿�Ҫ���룬�����ڲ��г˿�Ҫ����
    bool enterFlag = outerOrders.contains(currentFloor) && !outerOrders[currentFloor].empty();
    bool exitFlag = !passengerQueue[currentFloor].empty();
    if (enterFlag || exitFlag)
    {
        /// ����Ҫ�ؿ�ж��,���������״̬Ϊ stop
        this->setCurrentMovingState(ELEVATOR_DIRECTION::STOP);
        /// �����߳���ִ���ؿ�ж�Ͳ���
        QtConcurrent::run([=]()
                          {
            /// �Ƚ��г����ݲ���.
            if(exitFlag)
                unLoadPassenger();
            /// ��ִ������ݲ���.
            if(enterFlag)
                loadPassenger();
            /// �ж��Ƿ���Ҫ��������
            checkKeepMoving();
            /// �����źţ�����������ʱ��
            if(direction != ELEVATOR_DIRECTION::STOP){
                /// �õ�����������.
                setCurrentMovingState(getElevatorDirection());
                emit keepMoving();
            } });
    }
    else
    {
        /// ���
        checkKeepMoving();
    }

    /// @brief ������Ҫ�ؿ�ж�ͣ����ݼ�������
    if (movingState != ELEVATOR_DIRECTION::STOP)
    {
        /// �õ��ݼ�������
        emit keepMoving();
    }
}

/**
 * @brief Elevator::setElevatorInterval
 * ���õ�������ʱ��
 * @param interval
 */
void Elevator::setElevatorInterval(int interval)
{
    Elevator::runningInterval = interval;
}

/**
 * @brief Elevator::����������
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
 * ��ʼ��
 */
void Elevator::initElevator()
{
    /// ���� type ��ʼ���˿Ͷ���
    int typeIndex = bindElevatorTypes[id];
    auto &range = elevatorTypes[typeIndex].range;

    /// �󶨷�Χ
    setType(typeIndex);
    typeDescribe = elevatorTypes[typeIndex].describe;
    // qDebug()<<"in El: e"<<id<<" d:"<<typeDescribe;
    for (auto i = range.begin(); i != range.end(); i++)
    {
        passengerQueue.insert(i.key(), QQueue<Passenger *>());
        // qDebug()<<"in El: el"<<id<<" binds "<<i.key();
    }

    /// ���źźͲ�
    connect(this, &Elevator::keepMoving, this, [=]()
            {
        /// ����ǰ�����ź�
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

/// ����Ƿ����
void Elevator::checkKeepMoving()
{
    /// ��������Ѿ����˼ƻ��е���߲�/��ײ�,
    /// �����Ѿ����ﷶΧ���ޣ��Ǿ��޸ĵ��ݷ���.
    if (direction == ELEVATOR_DIRECTION::UP && planMaxFloor == currentFloor)
    {
        /// �����Ѿ�����
        if (passengerNumber == 0)
        {
            setDirection(ELEVATOR_DIRECTION::STOP);
            setCurrentMovingState(ELEVATOR_DIRECTION::STOP);
        }
        /// δ��
        else
        {
            setDirection(ELEVATOR_DIRECTION::DOWN);
            setCurrentMovingState(ELEVATOR_DIRECTION::DOWN);
            /// ȷ�����Ŀ��¥��
            /// ɨ���ڲ��˿�
            planMinFloor = 41;
            for (auto i = passengerQueue.begin(); i != passengerQueue.end(); i++)
            {
                if (!i.value().empty() && planMinFloor > i.key())
                {
                    planMinFloor = i.key();
                }
            }
            /// ɨ���ⲿ�˿�
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
        /// �����Ѿ�����
        if (passengerNumber == 0)
        {
            setDirection(ELEVATOR_DIRECTION::STOP);
            setCurrentMovingState(ELEVATOR_DIRECTION::STOP);
        }
        /// δ��
        else
        {
            setDirection(ELEVATOR_DIRECTION::UP);
            setCurrentMovingState(ELEVATOR_DIRECTION::UP);
            /// ȷ�����Ŀ��¥��
            /// ɨ���ڲ�����
            planMaxFloor = -1;
            for (auto i = passengerQueue.begin(); i != passengerQueue.end(); i++)
            {
                if (!i.value().empty() && planMaxFloor < i.key())
                    planMaxFloor = i.key();
            }
            /// ɨ���ⲿ����
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
        /// ɨ���ڲ�����
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
 * @brief Elevator::����ⲿ����ӳ˿�
 * @param passenger
 * @return
 */
bool Elevator::addOutOrder(Passenger *passenger)
{
    int floor = passenger->getFrom();
    /// ���ػ����޷�����Ŀ��㣬�򷵻� false
    if (!ableToReach(floor) && getPassengerNumber() >= maxNumber)
        return false;
    /// �� map ��� value
    if (!outerOrders.contains(floor))
        outerOrders[floor] = QVector<Passenger *>();
    outerOrders[floor].append(passenger);
    /// �����ǰ����ֹͣ����ô���ݵ����з����ɳ˿͵�λ�þ���.
    if (direction == ELEVATOR_DIRECTION::STOP)
    {
        /// �޸ĵ������з���
        ELEVATOR_DIRECTION to;
        floor > currentFloor ? to = ELEVATOR_DIRECTION::UP : to = ELEVATOR_DIRECTION::DOWN;
        setDirection(to);
        setCurrentMovingState(to);
        /// �õ��ݶ�����
        emit keepMoving();
    }
    /// ���� max & min
    if (direction == ELEVATOR_DIRECTION::UP && planMaxFloor < floor)
    {
        setPlanMaxFloor(floor);
    }
    else if (direction == ELEVATOR_DIRECTION::DOWN && planMinFloor > floor)
    {
        setPlanMinFloor(floor);
    }
    /// �󶨳˿�
    passenger->setElevatorID(id);
    /// �����ź�
    emit connectPassenger(passenger->getPassengerID(), id);
    return true;
}

bool Elevator::ableToReach(int floor)
{
    // qDebug()<<QString("in El: El%1 able to reach floor%2 ? -> %3").arg(id).arg(floor).arg(passengerQueue.contains(floor));
    return passengerQueue.contains(floor);
}

/**
 * @brief Elevator::���н���
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
 * ��ȡ��������
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
 * �õ� qlist������ע��
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
    /// 1��
    elevatorTypes[1].append(1);
    /// 25~40
    for (int i = 25; i <= MAX_FLOOR; i++)
        elevatorTypes[1].append(i);

    /// @brief type 2
    /// 1~25
    for (int i = 1; i <= 25; i++)
        elevatorTypes[2].append(i);

    /// @brief type 3
    /// 1��
    elevatorTypes[3].append(1);
    /// 2~40 ż��
    for (int i = 2; i <= MAX_FLOOR; i += 2)
        elevatorTypes[3].append(i);

    /// @brief type 4
    /// 1~39 ����
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
