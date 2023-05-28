#ifndef WAITINGQUEUE_H
#define WAITINGQUEUE_H

#include "Passenger.h"

#include <QObject>
#include <QMap>
#include <QQueue>
#include <functional>

/**
 * @brief The WaitingQueue class
 * 提供给 control/dispatching system 的接口
 */
class WaitingQueue
{
protected:
    QMap<int,QVector<Passenger*>> map;
public:
    WaitingQueue();

    /// 该层是否有乘客在等待
    virtual bool hasPassengerWaiting(int floor) = 0;

    /// 获取在某层的乘客数量
    virtual int passenerNumber(int floor) = 0;

    /// 添加乘客
    virtual void appendPassenger(Passenger* passenger) = 0;

    /// 乘客走出等待队列，
    /// 删除队列中对应的乘客指针，
    /// 并且返回乘客指针.
    virtual Passenger* popPassenger(int floor) = 0;

    /// 移除乘客
    virtual void removePassenger(Passenger* passenger) = 0;

    /// 用来遍历
    const QMap<int,QVector<Passenger*>>& waitingMap() const;
};

#endif // WAITINGQUEUE_H
