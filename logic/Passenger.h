#pragma once

#include "Utilities.h"

#include <QObject>
#include <QtGlobal>
#include <QTime>
#include <QMutex>

class PassengerData;
class Control;
class Elevator;
class DispatchingSystem;

/**
 * @brief The Passenger class
 * �Ѿ����˿���ע�ᵽ qt Ԫϵͳ�У�
 * ��ȡ�˿͵ķ�ʽ�ǣ��� dispatching �е��� generate passenger ������
 *                  ��Ϊ�ȴ����е���Ϣ�����Զ�ͬ���� ui �У��Ѿ����˿����ɵĴ��붼��װ���ˡ�
 * Ϊ�˿�������� get/set �������˴���Ϊ�˱���������ֱ���޸ĳ˿͵����ԣ�
 *                            ���³˿͵�����ʱ�����뷢���źţ�����֪ͨ ui ����ͬ�����£�����޸ĳ˿����Ա���ͨ�� set ������
 * ���ó˿͵���������ʹ���˾�̬���������г˿͹���ͬһ����ֵ����ui�п���ͨ�����������̬������Ϊȫ���˿��������ݡ�
 */
class Passenger : public QObject
{
	Q_OBJECT
    Q_PROPERTY(int from READ getFrom WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(int to READ getTo WRITE setTo NOTIFY toChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(int takingTime READ getTakingTime NOTIFY takingTimeChanged)
    Q_PROPERTY(int elID READ getElevatorID NOTIFY elIDChanged)
signals:
    void fromChanged();
    void toChanged();
    void nameChanged();
    void takingTimeChanged();
    void elIDChanged();
    void finishedSimulate();
private:
    /// �˿�����
    static int passengerNumber;
    /// �Ѿ���ɵĳ˿�����
    static int finishedNumber;
    /// ���ܴ���ͬʱ���� setFinishedNumber ��������������
    static QMutex mutex;

    //����¥��
    int from;
    //Ŀ��¥��
    int to;
    //�˿�״̬0�ȴ�,1�������,2�ڵ�����,3������
    PASSENGER_STATUS currentStatue;
    //�˿ͷ���0,1,2
    ELEVATOR_DIRECTION passengerDirection;
    // ��ʶ��ǰ�˿�
    int passengerID;
    // ��ǰ�˿�Ҫ�����ĵ���ID��Ĭ��Ϊ-1�������˿ͻ�û���ϵ���301
    int elevatorID;
    /// �ܵȴ�ʱ��
    QTime clock;
    /// �˵��ݴ���
    int takingTime;
public:
    /**
     * @brief ��̬�������������ó˿�����
     */

    /// ��ȡ/���ó˿�����
    static int getPassengerNumber();
    static void setPassengerNumber(int newPassengerNumber);

    /// ��ȡ/������ɵĳ˿�����
    static int getFinishedNumber();
    static void setFinishedNumber(int newFinishedNumber);
    static void addFinishedNumber(int num);

    /**
     * @brief �˿͸��㷨��һЩ����
     */
    Passenger(QObject *parent, int id);
    ~Passenger();
    /// get set from
    int getFrom() const;
    void setFrom(int newFrom);
    /// get set to
    int getTo() const;
    void setTo(int newTo);
    /// get set id
    int getPassengerID() const;
    void setPassengerID(int newPassengerID);
    /// get set elevator id
    int getElevatorID() const;
    void setElevatorID(int newElevatorID);
    /// ��ȡ���µ��ݵ�ʱ��
    int requireTime() const;
    /// �ȴ�ʱ��
    const QTime& waitingTime() const;
    /// ����
    const QString name() const;
    /// �˵��ݴ���
    int getTakingTime() const;
    void setTakingTime(int newTakingTime);
    /// ����
    ELEVATOR_DIRECTION getPassengerDirection() const;

private:
    void initFromAndTo();
};
