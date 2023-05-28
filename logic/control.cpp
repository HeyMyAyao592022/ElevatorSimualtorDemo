#include "control.h"

/*~��������ԭ�����ֵ���
1����վ������վԭ����������£�һ̨�����ڻ�վ������Ϊ���ݣ���һ̨����
���ͣ����¥�㣬�˵��ݳ�Ϊ�����ݡ���AΪ������ BΪ���ݣ�
�˿����ɺ��1 2 3 4ĳһ�ֵ��ݣ��ٸ��������з���ȷ��Ҫ���ػ��ݻ��������ݡ�
��1����������A������������ʱ������Ϸ������ٻ��ź����ɵ���Aȥ��ɣ�����
��վ��B���ݲ���Ӧ�����A�����·������ٻ��ź����ɵ���B�ش���źŲ�������
������A������������ʱ������Ϸ������ٻ��ź����ɵ���Bȥ��ɣ����A������
�������ٻ��ź����ɵ���A�ش���źŲ�������
//��2�����������A�������У�����¥���ٻ��źŶ࣬������B���߱�������������
��40s���ɵ����������ٻ��ź���Ȼ���ڣ�����B���ݷ�����
��3��������̨���ݽ������С������������з���һ�£����ɽϽ�������Ӧ��
��4��������̨���ݽ������С������������з���һ�£�A����B���£���A�Ϸ�
���������ٻ��ź���AӦ��A�·����������ٻ��ź���BӦ����B�Ϸ���������
�ٻ��ź���AӦ��B�·����������ٻ��ź���BӦ��
*/

Control::Control(QObject *parent)
    : QObject(parent), waitingQueue(nullptr)
{
    /// ��ʼ���ȴ�����
    waitingQueue = PassengerList::getInstance().get();
    /// ��ʼ���źźͲ�
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
 * ��ʼ���źźͲ�
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
 * @brief Control::ѡ������վ�ͻ�վ
 * @return
 */
QPair<Elevator *, Elevator *> Control::targetElevator(int from, int to)
{
    Elevator *elA = nullptr;
    Elevator *elB = nullptr;

    int maxNum = Elevator::getMaxNumber();

    foreach (Elevator *el, *elevators)
    {
        // �����ܵ���ò㣬��δ����
        if (el->ableToReach(from) && el->ableToReach(to) && el->getPassengerNumber() + 1 <= maxNum)
        {
            elA == nullptr ? elA = el : elB = el;
            // ������
            if (elB)
                break;
        }
    }

    return qMakePair(elA, elB);
}

void Control::connectElPeak(Passenger *passenger)
{
    // ���ȵ���ֹͣ�ĵ���
    int maxNum = Elevator::getMaxNumber();
    foreach (Elevator *el, *elevators)
    {
        // �жϵ����Ƿ��ܵ���Ŀ��¥�㣬��δ����
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
    // ���û��ֹͣ�ĵ��ݣ������ȵ��ýϽ���
    connectElSlack(passenger);
}

void Control::connectElSlack(Passenger *passenger)
{
    // ��������
    int from = passenger->getFrom();
    int to = passenger->getTo();
    static int maxNum = Elevator::getMaxNumber();
    // ��Ϊ 3 ��������˿��ڵ������з����ϣ�����ֹͣ, �˿��ڵ������еķ�����
    bool flag = true;
    // ���� �˿��ڵ������з����ϣ�����ֹͣ ����£�����ĵ���
    Elevator *closest = nullptr;
    int closestDistance = 0;
    // ���� �˿��ڵ������еķ����� ����£�Ŀ�����¥��/���¥������ĵ���
    Elevator *retrograde = nullptr;
    int retrogradeDistance = 0;
    // ��������
    foreach (Elevator *el, *elevators)
    {
        // �жϵ����Ƿ��ܵ���Ŀ��¥�㣬��δ����
        if (!el->ableToReach(from) || !el->ableToReach(to) || el->getPassengerNumber() + 1 > maxNum)
            continue;
        // �˿��Ƿ��ڵ����Ϸ�
        bool aboveEl = passenger->getFrom() > el->getCurrentFloor();
        int currentDistance = abs(el->getCurrentFloor() - from);
        // �ж�
        switch (el->getElevatorDirection())
        {
        case UP:
            // �˿��ڵ������з����ϣ����Ǹõ���
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
            // �˿��ڵ������з����ϣ����Ǹõ���
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
            // ����ֹͣ��Ҳ���Ǹõ���
            flag = true;
            break;
        }
        // �ж� �˿��ڵ������з����ϣ�����ֹͣ ������Ƿ�Ϊ�����
        // ( closest ��Ϊ�� && el ��˿͸��� ) || closest Ϊ��
        if (flag)
        {
            if ((closest && closestDistance > currentDistance) || !closest)
            {
                closest = el;
                closestDistance = currentDistance;
            }
        }
        // �ж� �˿��ڵ������еķ����� ������Ƿ�Ϊ�����
        else
        {
            if ((retrograde && retrogradeDistance > currentDistance) || !retrograde)
            {
                retrograde = el;
                retrogradeDistance = currentDistance;
            }
        }
    }
    // ����������ȷ�����ȵĵ���
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
    // ��������
    QPair<Elevator *, Elevator *> targetEl = targetElevator(passenger->getFrom(), passenger->getTo());
    // �Ȼ�ȡ����
    auto elevatorA = targetEl.first;
    auto elevatorB = targetEl.second;
    // �п�
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
    // B �ڻ���
    if (dirB == ELEVATOR_DIRECTION::STOP)
    {
        switch (dirA)
        {
        // Ŀ���������A�����з������ϲ���B�ڻ���
        case ELEVATOR_DIRECTION::UP:
            // �˿���A�Ϸ�
            if (aboveA)
            {
                // A���س˿�
                elevatorA->addOutOrder(passenger);
            }
            // �˿���A�·�
            else
            {
                // B���س˿�
                elevatorB->addOutOrder(passenger);
            }
            break;
            // Ŀ���������A�����з������²���B�ڻ���
        case ELEVATOR_DIRECTION::DOWN:
            // �˿���A�Ϸ�
            if (aboveA)
            {
                // B���س˿�
                elevatorB->addOutOrder(passenger);
            }
            // �˿���A�·�
            else
            {
                // A���س˿�
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
    // A B���з���һ��
    else if (dirA == dirB)
    {
        switch (dirA)
        {
        case ELEVATOR_DIRECTION::UP:
            // �˿��� A��B �Ϸ�
            if (aboveA && aboveB)
                break;
            // �˿��� A �·���B �Ϸ�
            else if (!aboveA && aboveB)
            {
                elevatorB->addOutOrder(passenger);
                return;
            }
            // �˿��� B �·���A �Ϸ�
            else if (aboveA && !aboveB)
            {
                elevatorA->addOutOrder(passenger);
                return;
            }
            break;
        case ELEVATOR_DIRECTION::DOWN:
            // �˿��� A��B �·�
            if (!aboveA && !aboveB)
                break;
            // �˿��� A �·���B �Ϸ�
            else if (!aboveA && aboveB)
            {
                elevatorA->addOutOrder(passenger);
                return;
            }
            // �˿��� B �·���A �Ϸ�
            else if (aboveA && !aboveB)
            {
                elevatorB->addOutOrder(passenger);
                return;
            }
            break;
        default:
            return;
        }
        // �˿ͳ���λ���� A��Bǰ�����������ȥ�ӳ˿�
        // A��˿ͽϽ�
        if (abs(passenger->getFrom() - elevatorA->getCurrentFloor()) < abs(passenger->getFrom() - elevatorB->getCurrentFloor()))
        {
            // A���س˿�
            elevatorA->addOutOrder(passenger);
        }
        // B��˿ͽϽ�
        else
        {
            elevatorB->addOutOrder(passenger);
        }
    }
    // A B���з���һ��.A����B����
    else if (dirA == ELEVATOR_DIRECTION::UP && dirB == ELEVATOR_DIRECTION::DOWN)
    {
        // �˿���A�Ϸ��ҷ�������
        if (aboveA && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            // A���س˿�
            elevatorA->addOutOrder(passenger);
        }
        // �˿���A�·��ҷ�������
        else if (!aboveA && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            elevatorB->addOutOrder(passenger);
        }
        // �˿���B�Ϸ��ҷ�������
        if (aboveB && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            // A���س˿�
            elevatorA->addOutOrder(passenger);
        }
        // �˿���B�·��ҷ�������
        else if (!aboveB && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            elevatorB->addOutOrder(passenger);
        }
    }
    // A B���з���һ��.A����B����
    else if (dirA == ELEVATOR_DIRECTION::DOWN && dirB == ELEVATOR_DIRECTION::UP)
    {
        // �˿���B�Ϸ��ҷ�������
        if (aboveB && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            // A���س˿�
            elevatorA->addOutOrder(passenger);
        }
        // �˿���B�·��ҷ�������
        else if (!aboveB && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            elevatorB->addOutOrder(passenger);
        }
        // �˿���A�Ϸ��ҷ�������
        if (aboveA && dirPassenger == ELEVATOR_DIRECTION::DOWN)
        {
            // A���س˿�
            elevatorA->addOutOrder(passenger);
        }
        // �˿���A�·��ҷ�������
        else if (!aboveA && dirPassenger == ELEVATOR_DIRECTION::UP)
        {
            elevatorB->addOutOrder(passenger);
        }
    }
}
