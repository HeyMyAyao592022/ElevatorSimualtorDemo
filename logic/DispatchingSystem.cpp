#include "DispatchingSystem.h"

/// 初始化
QSharedPointer<DispatchingSystem> DispatchingSystem::instance;

//构造函数
DispatchingSystem::DispatchingSystem(QObject *parent)
    : QThread(parent)
    , qmlEngine(nullptr)
    , flagStart(false)
    , waitingDequeue(nullptr)
    , generateNumber(0)
{
    /// qml
    qmlEngine = new QQmlApplicationEngine;

    /// 初始化 view model
    this->viewmodel = ViewModel::getInstance();
    /// 控制类
    this->control = new Control(this);
    /// 初始化等待队列
    this->waitingDequeue = PassengerList::getWaitingQueue();
    /// 测试...
    generateTimer.setInterval(8000);
    /// 绑定
    initSlots();
}

/**
 * @brief DispatchingSystem::getInstance
 * 获取单例
 * @return
 */
QSharedPointer<DispatchingSystem> DispatchingSystem::getInstance()
{
    if(instance.isNull()){
        instance = QSharedPointer<DispatchingSystem>(new DispatchingSystem());
    }
    return instance;
}

//析构函数
DispatchingSystem::~DispatchingSystem()
{
}

/**
 * @brief DispatchingSystem::run
 * 子线程
 * 在子线程中，不断执行 doDispatching 函数，
 * 为乘客选择电梯，
 */
void DispatchingSystem::run()
{
}

/**
 * @brief DispatchingSystem::获取 qml 引擎
 * @return
 */
QQmlApplicationEngine *DispatchingSystem::getQmlEngine()
{
    return this->qmlEngine;
}

/**
 * @brief DispatchingSystem::generatePassenger
 * 生成乘客
 */
void DispatchingSystem::generatePassenger()
{
    static int maxNum = Passenger::getPassengerNumber();
    if(generateNumber < maxNum){
        //qDebug()<<"max n: "<<maxNum<<" gn"<<generateNumber;
        appendPassenegr();
        generateNumber++;
    }
    else{
        generateTimer.stop();
    }
}

/**
 * @brief DispatchingSystem::appendPassenger
 * 添加乘客
 */
void DispatchingSystem::appendPassenegr()
{
    /// id
    static int id = 1;
    auto passenger = new Passenger(nullptr, id);
    /// id 递增
    ++id;
    /// 添加到队列中
    this->waitingDequeue->appendPassenger(passenger);
    /// 发出信号
    emit generateNewPassenger(passenger->getPassengerID()
                              ,passenger->getFrom()
                              ,passenger->getTo());
    /// 执行调度
    control->doDispatching(passenger);
}

/**
 * @brief DispatchingSystem::initSlots
 * 连接信号和槽
 */
void DispatchingSystem::initSlots()
{
    /// 设置定时器
    connect(&generateTimer,&QTimer::timeout,this,&DispatchingSystem::generatePassenger);

    /// 启动
    ViewModel* viewModelPtr = this->viewmodel.get();
    connect(viewModelPtr,&ViewModel::startSimulating,this,&DispatchingSystem::startSystem);
    /// 初始化电梯
    connect(viewModelPtr,&ViewModel::iniElsSignal,this,&DispatchingSystem::iniElevators);
    /// 完成初始化电梯
    connect(this,&DispatchingSystem::finishInEls,viewModelPtr,&ViewModel::emitFinishIniElSignal);

    /// 生成新乘客
    connect(this,&DispatchingSystem::generateNewPassenger,this->viewmodel.get(),&ViewModel::generateNewPassenger);
}

/**
 * @brief DispatchingSystem::初始化电梯
 */
void DispatchingSystem::iniElevators()
{
    /// 加载电梯
    QVector<Elevator*>* els = Elevator::getElevators();
    /// 初始化电梯
    foreach(Elevator* el, *els){
        if(el)
            connect(el,&Elevator::unLoadPassengerSignal,this,[=](){
                if(Passenger::getFinishedNumber() >= Passenger::getPassengerNumber()){
                    stopSystem();
                }
            });
    }
    //qDebug()<<"in sys: after conn el signal";
    /// 绑定电梯信号
    viewmodel->iniEls();
    control->iniEls();
    //qDebug()<<"in sys: ini els in vm and ctrl";
    /// 载入qml
    QList<QObject*> elevators = Elevator::getElevatorsList();
    qmlEngine->rootContext()->setContextProperty("elevators", QVariant::fromValue(elevators));
    //qDebug("in sys: finish load els in qml");
    /// 发出信号
    emit finishInEls();
}

void DispatchingSystem::startSystem()
{
    /// 开始生成乘客
    QTimer::singleShot(2000, this, [=](){
        generatePassenger();
    });
    this->generateTimer.start();
    /// 时间
    startTime = QDateTime::currentDateTime();
}

void DispatchingSystem::stopSystem()
{
    qDebug()<<"system stops";
    /// 时间
    endTime = QDateTime::currentDateTime();
    QTime dis;
    dis.setHMS(0,0,0,0);
    QString t = dis.addSecs(startTime.secsTo(endTime)).toString("mm:ss");
    viewmodel->setTotalTime(t);
    /// 发出结束信号
    viewmodel->emitFinishSignal();
}
