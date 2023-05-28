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
 * 已经将乘客类注册到 qt 元系统中，
 * 获取乘客的方式是：在 dispatching 中调用 generate passenger 方法。
 *                  因为等待队列的信息将会自动同步在 ui 中，已经将乘客生成的代码都封装好了。
 * 为乘客类添加了 get/set 方法：此处是为了避免其他类直接修改乘客的属性，
 *                            更新乘客的属性时，必须发出信号，才能通知 ui 进行同步更新，因此修改乘客属性必须通过 set 方法。
 * 设置乘客的总数量：使用了静态方法，所有乘客共享同一个数值。在ui中可以通过调用这个静态方法来为全部乘客设置数据。
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
    /// 乘客数量
    static int passengerNumber;
    /// 已经完成的乘客数量
    static int finishedNumber;
    /// 可能存在同时访问 setFinishedNumber 的情况，避免出错
    static QMutex mutex;

    //所在楼层
    int from;
    //目标楼层
    int to;
    //乘客状态0等待,1分配电梯,2在电梯内,3出电梯
    PASSENGER_STATUS currentStatue;
    //乘客方向0,1,2
    ELEVATOR_DIRECTION passengerDirection;
    // 标识当前乘客
    int passengerID;
    // 当前乘客要乘坐的电梯ID，默认为-1，新增乘客还没有上电梯301
    int elevatorID;
    /// 总等待时间
    QTime clock;
    /// 乘电梯次数
    int takingTime;
public:
    /**
     * @brief 静态方法，用于设置乘客数据
     */

    /// 获取/设置乘客数量
    static int getPassengerNumber();
    static void setPassengerNumber(int newPassengerNumber);

    /// 获取/设置完成的乘客数量
    static int getFinishedNumber();
    static void setFinishedNumber(int newFinishedNumber);
    static void addFinishedNumber(int num);

    /**
     * @brief 乘客给算法的一些方法
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
    /// 获取上下电梯的时间
    int requireTime() const;
    /// 等待时间
    const QTime& waitingTime() const;
    /// 名字
    const QString name() const;
    /// 乘电梯次数
    int getTakingTime() const;
    void setTakingTime(int newTakingTime);
    /// 方向
    ELEVATOR_DIRECTION getPassengerDirection() const;

private:
    void initFromAndTo();
};
