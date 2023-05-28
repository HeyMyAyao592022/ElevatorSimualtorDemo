#include "DispatchingSystem.h"

/// ��ʼ��
QSharedPointer<DispatchingSystem> DispatchingSystem::instance;

//���캯��
DispatchingSystem::DispatchingSystem(QObject *parent)
    : QThread(parent)
    , qmlEngine(nullptr)
    , flagStart(false)
    , waitingDequeue(nullptr)
    , generateNumber(0)
{
    /// qml
    qmlEngine = new QQmlApplicationEngine;

    /// ��ʼ�� view model
    this->viewmodel = ViewModel::getInstance();
    /// ������
    this->control = new Control(this);
    /// ��ʼ���ȴ�����
    this->waitingDequeue = PassengerList::getWaitingQueue();
    /// ����...
    generateTimer.setInterval(8000);
    /// ��
    initSlots();
}

/**
 * @brief DispatchingSystem::getInstance
 * ��ȡ����
 * @return
 */
QSharedPointer<DispatchingSystem> DispatchingSystem::getInstance()
{
    if(instance.isNull()){
        instance = QSharedPointer<DispatchingSystem>(new DispatchingSystem());
    }
    return instance;
}

//��������
DispatchingSystem::~DispatchingSystem()
{
}

/**
 * @brief DispatchingSystem::run
 * ���߳�
 * �����߳��У�����ִ�� doDispatching ������
 * Ϊ�˿�ѡ����ݣ�
 */
void DispatchingSystem::run()
{
}

/**
 * @brief DispatchingSystem::��ȡ qml ����
 * @return
 */
QQmlApplicationEngine *DispatchingSystem::getQmlEngine()
{
    return this->qmlEngine;
}

/**
 * @brief DispatchingSystem::generatePassenger
 * ���ɳ˿�
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
 * ��ӳ˿�
 */
void DispatchingSystem::appendPassenegr()
{
    /// id
    static int id = 1;
    auto passenger = new Passenger(nullptr, id);
    /// id ����
    ++id;
    /// ��ӵ�������
    this->waitingDequeue->appendPassenger(passenger);
    /// �����ź�
    emit generateNewPassenger(passenger->getPassengerID()
                              ,passenger->getFrom()
                              ,passenger->getTo());
    /// ִ�е���
    control->doDispatching(passenger);
}

/**
 * @brief DispatchingSystem::initSlots
 * �����źźͲ�
 */
void DispatchingSystem::initSlots()
{
    /// ���ö�ʱ��
    connect(&generateTimer,&QTimer::timeout,this,&DispatchingSystem::generatePassenger);

    /// ����
    ViewModel* viewModelPtr = this->viewmodel.get();
    connect(viewModelPtr,&ViewModel::startSimulating,this,&DispatchingSystem::startSystem);
    /// ��ʼ������
    connect(viewModelPtr,&ViewModel::iniElsSignal,this,&DispatchingSystem::iniElevators);
    /// ��ɳ�ʼ������
    connect(this,&DispatchingSystem::finishInEls,viewModelPtr,&ViewModel::emitFinishIniElSignal);

    /// �����³˿�
    connect(this,&DispatchingSystem::generateNewPassenger,this->viewmodel.get(),&ViewModel::generateNewPassenger);
}

/**
 * @brief DispatchingSystem::��ʼ������
 */
void DispatchingSystem::iniElevators()
{
    /// ���ص���
    QVector<Elevator*>* els = Elevator::getElevators();
    /// ��ʼ������
    foreach(Elevator* el, *els){
        if(el)
            connect(el,&Elevator::unLoadPassengerSignal,this,[=](){
                if(Passenger::getFinishedNumber() >= Passenger::getPassengerNumber()){
                    stopSystem();
                }
            });
    }
    //qDebug()<<"in sys: after conn el signal";
    /// �󶨵����ź�
    viewmodel->iniEls();
    control->iniEls();
    //qDebug()<<"in sys: ini els in vm and ctrl";
    /// ����qml
    QList<QObject*> elevators = Elevator::getElevatorsList();
    qmlEngine->rootContext()->setContextProperty("elevators", QVariant::fromValue(elevators));
    //qDebug("in sys: finish load els in qml");
    /// �����ź�
    emit finishInEls();
}

void DispatchingSystem::startSystem()
{
    /// ��ʼ���ɳ˿�
    QTimer::singleShot(2000, this, [=](){
        generatePassenger();
    });
    this->generateTimer.start();
    /// ʱ��
    startTime = QDateTime::currentDateTime();
}

void DispatchingSystem::stopSystem()
{
    qDebug()<<"system stops";
    /// ʱ��
    endTime = QDateTime::currentDateTime();
    QTime dis;
    dis.setHMS(0,0,0,0);
    QString t = dis.addSecs(startTime.secsTo(endTime)).toString("mm:ss");
    viewmodel->setTotalTime(t);
    /// ���������ź�
    viewmodel->emitFinishSignal();
}
