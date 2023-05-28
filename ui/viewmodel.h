#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include "Passenger.h"
#include "passengerlist.h"
#include "Elevator.h"

#include <QObject>
#include <QMutex>
#include <QVector>
#include <QList>
#include <QPair>
#include <QDateTime>
#include <QSharedPointer>
#include <QSharedDataPointer>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class ViewModel : public QObject
{
    Q_OBJECT
    // 完成人数
    Q_PROPERTY(int finishedNumber READ getFinishedNumber NOTIFY finishedNumberChanged)
    // 乘客总数
    Q_PROPERTY(int passengerNumber READ getPassengerNumber WRITE setPassengerNumber NOTIFY passengerNumberChanged)
    // 电梯运行速度
    Q_PROPERTY(int elevatorInterval READ getRunningInterval WRITE setRunningInterval NOTIFY runningIntervalChanged)
    // 电梯数量
    Q_PROPERTY(int elNum READ getElNum WRITE setElNum NOTIFY elNumChanged)
    // 电梯满载人数
    Q_PROPERTY(int elMaxNum READ getElMaxNum WRITE setElMaxNum NOTIFY elMaxNumChanged)
public:
signals:
    /// 初始化电梯
    void iniElsSignal();
    void finishIniEls();
    /// 开始运行
    void startSimulating();
    /// 结束
    void finishSimulating();
    // 属性
    void passengerNumberChanged();
    void finishedNumberChanged();
    void runningIntervalChanged();
    void elNumChanged();
    void elMaxNumChanged();
    /// 生成新乘客
    void generateNewPassenger(int id, int from, int to);
    /// 绑定乘客
    void connectPassenger(int psgID, int elID);
private:
    /// 共享同一个实例
    static QSharedPointer<ViewModel> instance;
    /// 线程安全
    static QMutex mutex;
    /// 乘客等待队列
    QSharedPointer<PassengerList> passengerList;
    /// 时间
    QPair<QDateTime, QDateTime> times;
    QString totalTime;
public:
    explicit ViewModel(QObject *parent = nullptr);

    /**
     * @brief getInstance 单例模式
     * @param parent
     * @return
     */
    static QSharedPointer<ViewModel>& getInstance();

    /**
     * @brief emitIniElSignal
     * 给 qml 调用，这个函数会发出信号，通知系统初始化电梯
     */
    Q_INVOKABLE void emitIniElSignal();
    void iniEls();

    /// @brief 完成加载电梯
    void emitFinishIniElSignal();

    /**
     * @brief emitStartSignal
     * 给 qml 调用，这个函数会发出信号，通知整个系统开始运行
     */
    Q_INVOKABLE void emitStartSignal();

    /// @brief 结束信号
    void emitFinishSignal();

    /// @brief 绑定电梯
    Q_INVOKABLE void bindElTypes(int elID, int typeID);

    /// @brief 设置类型
    Q_INVOKABLE void setTypesNum(int num);
    Q_INVOKABLE void iniTypes(int typeID, int floor);
    Q_INVOKABLE void setTypeDescribe(int id, QString str);

    /// @brief 生成新乘客
    void emitGenerateSignal(int id, int from, int to);

    /// @brief 绑定乘客
    void emitConnectSiganl(int psgID, int elID);

    /**
     * @brief objectModel
     * 暴露给 qml 的
     * @return
     */
    Q_INVOKABLE PassengerList* objectModel();

    /// 获取乘客总数
    int getPassengerNumber();
    /// 设置乘客总数
    void setPassengerNumber(int num);

    /// 获取完成的乘客数量
    int getFinishedNumber();

    /// 获取电梯上下一层的时间
    int getRunningInterval() const;
    /// 设置电梯上下一层的时间
    void setRunningInterval(int val);

    /// 电梯满载人数
    int getElMaxNum() const;
    void setElMaxNum(int num);

    /// 电梯数量
    int getElNum() const;
    void setElNum(int num);

    /// 时间
    void setTime(QDateTime start, QDateTime end);
    Q_INVOKABLE QString getTotalTime();
    void setTotalTime(QString time);
private:
    void initSlots();
};

#endif // VIEWMODEL_H
