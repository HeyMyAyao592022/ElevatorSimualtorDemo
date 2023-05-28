#pragma once

#include "Passenger.h"
#include "Elevator.h"
#include "Utilities.h"
#include "control.h"
#include "viewmodel.h"
#include "waitingqueue.h"
#include "passengerlist.h"

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include <QDateTime>
#include <QSharedPointer>
#include <QtQml/QQmlEngine>
#include <QQmlApplicationEngine>

/**
 * @brief The DispatchingSystem class
 */
class DispatchingSystem
        : public QThread
{
	Q_OBJECT
public:
signals:
    void generateNewPassenger(int id, int from, int to);
    void finishInEls();
private:
    /// qml 引擎
    QQmlApplicationEngine* qmlEngine;
    /// 获取实例
    static QSharedPointer<DispatchingSystem> instance;
    // 是否启动模拟
    bool flagStart;
    // 操作 WaittingQueue 需要进行线程同步，避免出错
    QMutex waitingQueueMutex;
    /// 控制类
    Control* control;
    /// view model
    QSharedPointer<ViewModel> viewmodel;
    /// 等待队列
    WaitingQueue* waitingDequeue;
    /// 用于生成乘客
    QTimer generateTimer;
    /// 记录已经生成的乘客
    int generateNumber;
    /// 时间
    QDateTime startTime;
    QDateTime endTime;
public:
    //构造函数
    DispatchingSystem(QObject *parent = nullptr);

    /// 获取实例
    static QSharedPointer<DispatchingSystem> getInstance();

    //析构函数
    ~DispatchingSystem();

    //调度运行
    /// 子线程的操作都将运行在这里
    void run();

    /// 获取 qml 引擎
    QQmlApplicationEngine* getQmlEngine();

private:
    /// 添加乘客
    void appendPassenegr();

    /// 生成乘客
    void generatePassenger();

    /// 连接信号和槽
    void initSlots();

    /// 初始化电梯
    void iniElevators();

    /// 开启停止
    void startSystem();
    void stopSystem();
};
