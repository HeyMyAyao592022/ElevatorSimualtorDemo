#include "control.h"

/*~并联调度原则，四种电梯
1、基站和自由站原则：正常情况下，一台电梯在基站待命作为基梯，另一台留在
最后停靠的楼层，此电梯称为自由梯。（A为自有梯 B为基梯）
乘客生成后绑定1 2 3 4某一种电梯，再根据其运行方向确定要搭载基梯还是自由梯。
（1）、当电梯A正在向上运行时，如果上方出现召唤信号则由电梯A去完成，而在
基站的B电梯不予应答，如果A电梯下方出现召唤信号则由电梯B回答该信号并发车。
当电梯A正在向下运行时，如果上方出现召唤信号则由电梯B去完成，如果A电梯下
方出现召唤信号则由电梯A回答该信号并发车。
//（2）、如果电梯A正在运行，其他楼层召唤信号多，而基梯B不具备发车条件，经
过40s（可调整）后若召唤信号仍然存在，则令B电梯发车。
（3）、若两台电梯皆在运行。且两电梯运行方向一致，则由较近电梯响应。
（4）、若两台电梯皆在运行。且两电梯运行方向不一致，A向上B向下，若A上方
出现向上召唤信号则A应答，A下方出现向下召唤信号则B应答；若B上方出现向下
召唤信号则A应答，B下方出现向下召唤信号则B应答。
*/

Control::Control(QObject *parent)
    : QObject(parent), waitingQueue(nullptr)
{
    /// 初始化等待队列
    waitingQueue = PassengerList::getInstance().get();
    /// 初始化信号和槽
    initSlots();
}

/**
 * @brief Control::doDispatching
 */
void Control::doDispatching(Passenger *passenger)
{
    switch (schemeType)
    {
    case PEAK:
        connectElPeak(passenger);
        break;
    case SLACK:
        connectElSlack(passenger);
        break;
    default:
        break;
    }
}

/**
 * @brief Control::initSlots
 * 初始化信号和槽
 */
void Control::initSlots()
{
}

/**
 * @brief Control::iniEls
 */
void Control::iniEls()
{
    elevators = Elevator::getElevators(this);
    if (elevators->size() < 5)
        schemeType = SCHEME_TYPE::SLACK;
    else
        schemeType = SCHEME_TYPE::PEAK;
}

/**
 * @brief Control::选择自由站和基站
 * @return
 */
QPair<Elevator *, Elevator *> Control::targetElevator(int from, int to)
{
    Elevator *elA = nullptr;
    Elevator *elB = nullptr;

    int maxNum = Elevator::getMaxNumber();

    foreach (Elevator *el, *elevators)
    {
        // 电梯能到达该层，且未超载
        if (el->ableToReach(from) && el->ableToReach(to) && el->getPassengerNumber() + 1 <= maxNum)
        {
            elA == nullptr ? elA = el : elB = el;
            // 挑完了
            if (elB)
                break;
        }
    }

    return qMakePair(elA, elB);
}

void Control::connectElPeak(Passenger *passenger)
{
    // 优先调用停止的电梯
    int maxNum = Elevator::getMaxNumber();
    foreach (Elevator *el, *elevators)
    {
        // 判断电梯是否能到达目标楼层，且未超载
        if (!el->ableToReach(passenger->getFrom()) || !el->ableToReach(passenger->getTo()) || el->getPassengerNumber() + 1 > maxNum)
        {
            continue;
        }
        if (el->getDirection() == ELEVATOR_DIRECTION::STOP)
        {
            el->addOutOrder(passenger);
            return;
        }
    }
    // 如果没有停止的电梯，则优先调用较近的
    connectElSlack(passenger);
}

void Control::connectElSlack(Passenger *passenger)
{
    // 声明变量
    int from = passenger->getFrom();
    int to = passenger->getTo();
    static int maxNum = Elevator::getMaxNumber();
    // 分为 3 种情况：乘客在电梯运行方向上，电梯停止, 乘客在电梯运行的反方向
    bool flag = true;
    // 代表 乘客在电梯运行方向上，电梯停止 情况下，最近的电梯
    Elevator *closest = nullptr;
    int closestDistance = 0;
    // 代表 乘客在电梯运行的反方向 情况下，目标最高楼层/最低楼层最近的电梯
    Elevator *retrograde = nullptr;
    int retrogradeDistance = 0;
    // 遍历电梯
    foreach (Elevator *el, *elevators)
    {
        // 判断电梯是否能到达目标楼层，且未超载
        if (!el->ableToReach(from) || !el->ableToReach(to) || el->getPassengerNumber() + 1 > maxNum)
            continue;
        // 乘客是否在电梯上方
        bool aboveEl = passenger->getFrom() > el->getCurrentFloor();
        int currentDistance = abs(el->getCurrentFloor() - from);
        // 判断
        switch (el->getElevatorDirection())
        {
        case UP:
            // 乘客在电梯运行方向上，则考虑该电梯
            if (aboveEl)
            {
                flag = true;
                break;
            }
            else
            {
                flag = false;
                currentDistance = abs(el->getPlanMaxFloor() - from);
                break;
            }
        case DOWN:
            // 乘客在电梯运行方向上，则考虑该电梯
            if (!aboveEl)
            {
                flag = true;
                break;
            }
            else
            {
                flag = false;
                currentDistance = abs(el->getPlanMinFloor() - from);
                break;
            }
        case STOP:
            // 电梯停止，也考虑该电梯
            flag = true;
            break;
        }
        // 判断 乘客在电梯运行方向上，电梯停止 情况下是否为最近的
        // ( closest 不为空 && el 离乘客更近 ) || closest 为空
        if (flag)
        {
            if ((closest && closestDistance > currentDistance) || !closest)
            {
                closest = el;
                closestDistance = currentDistance;
            }
        }
        // 判断 乘客在电梯运行的反方向 情况下是否为最近的
        else
        {
            if ((retrograde && retrogradeDistance > currentDistance) || !retrograde)
            {
                retrograde = el;
                retrogradeDistance = currentDistance;
            }
        }
    }
    // 遍历结束，确定调度的电梯
    if (closest)
    {
        closest->addOutOrder(passenger);
    }
    else if (retrograde)
    {
        retrograde->addOutOrder(passenger);
    }
    else
    {
        QTimer::singleShot(10000, this, [=]()
                           {
            if(passenger->getElevatorID() == DEAULT_ELEVATOR_ID)
            {
                connectElSlack(passenger);
            } });
    }
}

/**
 * @brief Control::connectElevator
 */
void Control::connectElevator(Passenger *passenger)
{
    // 电梯种类
    QPair<Elevator *, Elevator *> targetEl = targetElevator(passenger->getFrom(), passenger->getTo());
    // 先获取变量
    auto elevatorA = targetEl.first;
    auto elevatorB = targetEl.second;
    // 判空
    if (!elevatorA || !elevatorB)
    {
        if (elevatorA)
        {
            elevatorA->addOutOrder(passenger);
        }
        return;
    }
    // direction
    auto dirA = elevatorA->getElevatorDirection();
    auto dirB = elevatorB->getElevatorDirection();
    auto dirPassenger = passenger->getPassengerDirection();
    // flag
    bool aboveA = passenger->getFrom() > elevatorA->getCurrentFloor();
    bool aboveB = passenger->getFrom() > elevatorB->getCurrentFloor();
    // B 在基地
    if (dirB == ELEVATOR_DIRECTION::STOP)
    {
        switch (dirA)
        {
        // 目标种类电梯A梯运行方向向上并且B在基地
        case ELEVATOR_DIRECTION::UP:
            // 乘客在A上方
            if (aboveA)
            {
                // A搭载乘客
                elevatorA->addOutOrder(passenger);
            }
            // 乘客在A下方
            else
            {
                // B搭载乘客
                elevatorB->addOutOrder(passenger);
            }
            break;
            // 目标种类电梯A梯运行方向向下并且B在基地
        case ELEVATOR_DIRECTION::DOWN:
            // 乘客在A上方
            if (aboveA)
            {
                // B搭载乘客
                elevatorB->addOutOrder(passenger);
            }
            // 乘客在A下方
            else
            {
                // A搭载乘客
                elevatorA->addOutOrder(passenger);
            }
            break;
        case ELEVATOR_DIRECTION::STOP:
            elevatorA->addOutOrder(passenger);
            break;
        default:
            break;
        }
    }
    // A B运行方向一致
    else if (dirA == dirB)
    {
        switch (dirA)
        {
        case ELEVATOR_DIRECTION::UP:
            // 乘客在 A、B 上方
            if (aboveA && aboveB)
                break;
            // 乘客在 A 下方，B 上方
            else if (!aboveA && aboveB)
            {
                elevatorB->addOutOrder(passenger);
                return;
            }
            // 乘客在 B 下方，A 上方
            else if (aboveA && !aboveB)
            {
                elevatorA->addOutOrder(passenger);
                return;
            }
            break;
        case ELEVATOR_DIRECTION::DOWN:
            // 乘客在 A、B 下方
            if (!aboveA && !aboveB)
                break;
            // 乘客在 A 下方，B 上方
            else if (!aboveA && aboveB)
            {
                elevatorA->addOutOrder(passenger);
                return;
            }
            // 乘客在 B 下方，A 上方
            else if (aboveA && !aboveB)
            {
                elevatorB->addOutOrder(passenger);
                return;
            }
            break;
        default:
            return;
        }
        // 乘客出现位置在 A、B前方，让最近的去接乘客
        // A离乘客较近
        if (abs(passenger->getFrom() - elevatorA->getCurrentFloor()) < abs(passenger->getFrom() - elevatorB->getCurrentFloor()))
        {
            // A搭载乘客
            elevatorA->addOutOrder(passenger);
        }
        // B离乘客较近
        else
        {
            elevatorB->addOutOrder(passenger);
        }
    }
    // A B运行方向不一致.A向上B向下
    else if (dirA == ELEVATOR_DIRECTION::UP && dirB == ELEVATOR_DIRECTION::DOWN)
    {
        // 乘客在A上方且方向向上
        if (aboveA && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            // A搭载乘客
            elevatorA->addOutOrder(passenger);
        }
        // 乘客在A下方且方向向下
        else if (!aboveA && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            elevatorB->addOutOrder(passenger);
        }
        // 乘客在B上方且方向向下
        if (aboveB && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            // A搭载乘客
            elevatorA->addOutOrder(passenger);
        }
        // 乘客在B下方且方向向上
        else if (!aboveB && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            elevatorB->addOutOrder(passenger);
        }
    }
    // A B运行方向不一致.A向下B向上
    else if (dirA == ELEVATOR_DIRECTION::DOWN && dirB == ELEVATOR_DIRECTION::UP)
    {
        // 乘客在B上方且方向向上
        if (aboveB && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            // A搭载乘客
            elevatorA->addOutOrder(passenger);
        }
        // 乘客在B下方且方向向下
        else if (!aboveB && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            elevatorB->addOutOrder(passenger);
        }
        // 乘客在A上方且方向向下
        if (aboveA && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            // A搭载乘客
            elevatorA->addOutOrder(passenger);
        }
        // 乘客在A下方且方向向上
        else if (!aboveA && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            elevatorB->addOutOrder(passenger);
        }
    }
}
