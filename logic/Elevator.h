#pragma once

#include "Passenger.h"
#include "Utilities.h"
#include "passengerlist.h"

#include <QObject>
#include <QTimer>
#include <QMap>
#include <QVector>
#include <QList>
#include <QQueue>
#include <QMutex>
#include <QtConcurrent/QtConcurrent>

/**
 * @brief The 电梯种类 struct
 */
struct ElevatorType
{
    // 代表电梯范围
    QMap<int,bool> range;
    int typeID;
    QString describe;
    // 构造
    ElevatorType() {}
    explicit ElevatorType(int type): typeID(type) {}
    // 操作
    void append(int index){
        range[index] = true;
    }
    void remove(int index){
        if(range.contains(index))
            range.remove(index);
    }
    bool withinRange(int floor) const{
        return range.contains(floor);
    }
};

/**
 * @brief The Elevator class
 * 获取电梯的方式：调用 static const QVector<Elevator*>* getElevators(QObject* parent) 函数，
 *                因为电梯数组只有一个，无论在哪个类里面获取到的电梯数组都相同，为此干脆写成了单例模式。
 * 电梯的属性都设计了 get/set 函数 ，因为修改电梯的属性时，必须要发出信号才能通知 ui 进行同步更新，因此将更改属性和发出信号的操作都封装在 set 中。
 */
class Elevator : public QObject
{
    /// qt宏
    Q_OBJECT
    /// 将属性注册到qml中，即可实现数据绑定
    Q_PROPERTY(int id READ getID NOTIFY paramChanged)
    Q_PROPERTY(int type READ getType NOTIFY paramChanged)
    Q_PROPERTY(int currentFloor READ getCurrentFloor NOTIFY currentFloorChanged)
    // 方向
    Q_PROPERTY(int direction READ getDirection NOTIFY directionChanged)
    Q_PROPERTY(int movingState READ getCurrentMovingState NOTIFY movingStateChanged)
    // 公有属性
    Q_PROPERTY(int passengerNumber READ getPassengerNumber NOTIFY passengerNumberChanged)
    //Q_PROPERTY(double runningProgress READ getRunningProgress NOTIFY runningProgressChanged)
    ///  @author tu
    ///  max&min
    Q_PROPERTY(int planMaxFloor READ getPlanMaxFloor NOTIFY PlanMaxFloorChanged)
    Q_PROPERTY(int planMinFloor READ getPlanMinFloor NOTIFY PlanMinFloorChanged)
    /// describe
    Q_PROPERTY(QString describe READ getDescribe NOTIFY describeChanged)
public:
signals:
    void updateParam(int floor);
    /// 属性更新，通知 Ui
    void paramChanged();
    void currentFloorChanged();
    /// 方向
    void directionChanged();
    void movingStateChanged();
    /// 公有属性
    void passengerNumberChanged();
    void runningProgressChanged();
    /// 装货卸货的信号，传递给ui，将会更新ui
    void loadPassengerSignal(int passengerID, int elID, int consumeTime);
    void removePassengerFromQueue(Passenger* passenger);
    void unLoadPassengerSignal(int num);
    void passengerExitSignal(int elID, int psgID, int consumeTime);
    /// 绑定乘客
    void connectPassenger(int psgID, int elID);
    /// 电梯在楼层与楼层之间即发出这个信号
    void isRunning(int progress);
    /// 开始向新一层运行
    void approachNew(int newFloor, int id);
    /// @author tu
    void PlanMinFloorChanged();
    void PlanMaxFloorChanged();
    /// 电梯继续运行
    void keepMoving();
    /// 描述
    void describeChanged();
public:
    /**
     * @brief 以下是静态函数，主要面向 ui.
     */

    /// 获取电梯数组
    static QVector<Elevator*>* getElevators(QObject* parent = nullptr);
    /// @brief 给 qml 使用的，希望不要调用
    static QList<QObject*> getElevatorsList();

    /// 获取电梯类型
    static QVector<ElevatorType>& getElevatorTypes();
    static void iniDefaultElevatorTypes();

    /// 设置类型
    static void bindElTypes(int el, int type);
    static void setTypesNum(int num);
    static void appTypeRange(int typeID, int floor);
    static void setTypeDescribe(int id, QString str);

    /// @brief setElevatorInterval 设置电梯上下的时间
    static void setElevatorInterval(int interval);

    /// 设置最大搭载人数
    static void setMaxNumber(int num);
    static int getMaxNumber();

    /// 电梯上下一层所需的时间
    static int getRunningInterval();
    static void setRunningInterval(int newRunningInterval);

    /// 电梯数量
    static int getElevatorNumber();
    static void setElevatorNumber(int newElevatorNumber);

    /**
     * @brief 以下是关于电梯的一些方法
     */

    /// 构造电梯
    Elevator(QObject* parent, ELEVATOR_TYPE type, int id);
    Elevator(QObject* parent, int id);
    /// 析构电梯
    ~Elevator();

    ///上乘客
    void loadPassenger(Passenger*);
    /// 将要进电梯的乘客都放入队列中，再排队进入电梯.
    void loadPassenger();
    /// 放下乘客
    void unLoadPassenger();
    /// 表示该层有乘客想下电梯
    bool hasUnloaded(int floor);

    /**
     * @brief addOrder 添加命令，电梯将会在 floor 层发出信号
     * @return 是否添加成功（可能该层无法到达，或者已经满员）
     */
    /// 添加外部命令
    bool addOutOrder(Passenger* passenger);
    /// 是否能到达 floor 层
    bool ableToReach(int floor);
    /// 每 S秒触发
    void running();

    /**
     * @brief 以下是属性的 get/set 函数
     */

    /// 电梯中的乘客数，表示电梯内的乘客数量
    int getPassengerNumber();

    /// 电梯运行方向
    int getDirection() const;
    ELEVATOR_DIRECTION getElevatorDirection() const;
    void setDirection(ELEVATOR_DIRECTION newCurrentDirection);

    /// 电梯状态
    int getMovingState() const;
    ELEVATOR_DIRECTION getCurrentMovingState() const;
    void setCurrentMovingState(ELEVATOR_DIRECTION newCurrentMoveDirection);

    /// 电梯类型
    int getType() const;
    void setType(int newType);
    QString getTypeDescribe() const;

    /// id
    int getID() const;

    /// 当前楼层
    int getCurrentFloor() const;
    void setCurrentFloor(int newCurrentFloor);
    void resetCurrentFloor();

    /// 运行进度
    double getRunningProgress() const;
    void setRunningProgress();

    /// current floor++/--,
    /// 并触发 current floor changed 信号
    void runUp();
    void runDown();

    /// max&min
    int getPlanMaxFloor() const;
    int getPlanMinFloor() const;
    void setPlanMaxFloor(int value);
    void setPlanMinFloor(int value);

    /// 获取总距离
    Q_INVOKABLE int getTotalDistance() const;
    Q_INVOKABLE int getConsume() const;

    /// 获取描述
    Q_INVOKABLE void setDescribe(QString str);
    Q_INVOKABLE QString getDescribe() const;

private:
    /// 初始化
    void initElevator();

    /// 检查是否继续运行
    void checkKeepMoving();
private:
    /// 电梯数组
    static QVector<Elevator*> elevators;
    static QMutex mutex;

    /// 获取电梯类型
    static QVector<ElevatorType> elevatorTypes;
    /// 绑定电梯类型
    /// <电梯id，type>
    static QMap<int, int> bindElevatorTypes;
    /// 电梯颜色
    static QVector<QColor> colors;

    /// 上下楼层的时间间隔
    static int runningInterval;
    /// 定时器的触发间隔 (秒)
    /// static const float timerInterval;
    /// 最大搭载人数
    static int maxNumber;
    /// 电梯数量
    static int elevatorNumber;

    /// 辅助运算
    double runningProgress;
    /// 电梯类型
    int type;
    // 当前所在楼层
    int currentFloor;
    // 计划达到的最高楼层
    int planMaxFloor;
    // 计划达到的最低楼层
    int planMinFloor;
    // 当前电梯目标运行方向
    ELEVATOR_DIRECTION direction;
    // 当前电梯运行方向
    ELEVATOR_DIRECTION movingState;
    /// 电梯 id
    int id;
    /// 乘客数量
    int passengerNumber;
    /// 描述
    QString typeDescribe;
    /// 总运行过程
    int totalDistance;
    /// 可到达的每一层中，都有一个乘客队列,
    /// 这个属性还可以拿来判断电梯的范围。
    QMap<int,QQueue<Passenger*>> passengerQueue;
    /// 进电梯的乘客队列
    /// QQueue<Passenger*> enterQueue;
    /// 指令集合，orders[s]==true 代表在s层需要停下来载客卸货
    QMap<int,int> inOrders;
    QMap<int,QVector<Passenger*>> outerOrders;
};
