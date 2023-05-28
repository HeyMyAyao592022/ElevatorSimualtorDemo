#ifndef WAITINGQUEUE_H
#define WAITINGQUEUE_H

#include "Passenger.h"

#include <QObject>
#include <QMap>
#include <QQueue>
#include <functional>

/**
 * @brief The WaitingQueue class
 * �ṩ�� control/dispatching system �Ľӿ�
 */
class WaitingQueue
{
protected:
    QMap<int,QVector<Passenger*>> map;
public:
    WaitingQueue();

    /// �ò��Ƿ��г˿��ڵȴ�
    virtual bool hasPassengerWaiting(int floor) = 0;

    /// ��ȡ��ĳ��ĳ˿�����
    virtual int passenerNumber(int floor) = 0;

    /// ��ӳ˿�
    virtual void appendPassenger(Passenger* passenger) = 0;

    /// �˿��߳��ȴ����У�
    /// ɾ�������ж�Ӧ�ĳ˿�ָ�룬
    /// ���ҷ��س˿�ָ��.
    virtual Passenger* popPassenger(int floor) = 0;

    /// �Ƴ��˿�
    virtual void removePassenger(Passenger* passenger) = 0;

    /// ��������
    const QMap<int,QVector<Passenger*>>& waitingMap() const;
};

#endif // WAITINGQUEUE_H
