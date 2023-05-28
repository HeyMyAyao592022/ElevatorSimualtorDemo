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
 * @brief The �������� struct
 */
struct ElevatorType
{
    // ������ݷ�Χ
    QMap<int,bool> range;
    int typeID;
    QString describe;
    // ����
    ElevatorType() {}
    explicit ElevatorType(int type): typeID(type) {}
    // ����
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
 * ��ȡ���ݵķ�ʽ������ static const QVector<Elevator*>* getElevators(QObject* parent) ������
 *                ��Ϊ��������ֻ��һ�����������ĸ��������ȡ���ĵ������鶼��ͬ��Ϊ�˸ɴ�д���˵���ģʽ��
 * ���ݵ����Զ������ get/set ���� ����Ϊ�޸ĵ��ݵ�����ʱ������Ҫ�����źŲ���֪ͨ ui ����ͬ�����£���˽��������Ժͷ����źŵĲ�������װ�� set �С�
 */
class Elevator : public QObject
{
    /// qt��
    Q_OBJECT
    /// ������ע�ᵽqml�У�����ʵ�����ݰ�
    Q_PROPERTY(int id READ getID NOTIFY paramChanged)
    Q_PROPERTY(int type READ getType NOTIFY paramChanged)
    Q_PROPERTY(int currentFloor READ getCurrentFloor NOTIFY currentFloorChanged)
    // ����
    Q_PROPERTY(int direction READ getDirection NOTIFY directionChanged)
    Q_PROPERTY(int movingState READ getCurrentMovingState NOTIFY movingStateChanged)
    // ��������
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
    /// ���Ը��£�֪ͨ Ui
    void paramChanged();
    void currentFloorChanged();
    /// ����
    void directionChanged();
    void movingStateChanged();
    /// ��������
    void passengerNumberChanged();
    void runningProgressChanged();
    /// װ��ж�����źţ����ݸ�ui���������ui
    void loadPassengerSignal(int passengerID, int elID, int consumeTime);
    void removePassengerFromQueue(Passenger* passenger);
    void unLoadPassengerSignal(int num);
    void passengerExitSignal(int elID, int psgID, int consumeTime);
    /// �󶨳˿�
    void connectPassenger(int psgID, int elID);
    /// ������¥����¥��֮�伴��������ź�
    void isRunning(int progress);
    /// ��ʼ����һ������
    void approachNew(int newFloor, int id);
    /// @author tu
    void PlanMinFloorChanged();
    void PlanMaxFloorChanged();
    /// ���ݼ�������
    void keepMoving();
    /// ����
    void describeChanged();
public:
    /**
     * @brief �����Ǿ�̬��������Ҫ���� ui.
     */

    /// ��ȡ��������
    static QVector<Elevator*>* getElevators(QObject* parent = nullptr);
    /// @brief �� qml ʹ�õģ�ϣ����Ҫ����
    static QList<QObject*> getElevatorsList();

    /// ��ȡ��������
    static QVector<ElevatorType>& getElevatorTypes();
    static void iniDefaultElevatorTypes();

    /// ��������
    static void bindElTypes(int el, int type);
    static void setTypesNum(int num);
    static void appTypeRange(int typeID, int floor);
    static void setTypeDescribe(int id, QString str);

    /// @brief setElevatorInterval ���õ������µ�ʱ��
    static void setElevatorInterval(int interval);

    /// ��������������
    static void setMaxNumber(int num);
    static int getMaxNumber();

    /// ��������һ�������ʱ��
    static int getRunningInterval();
    static void setRunningInterval(int newRunningInterval);

    /// ��������
    static int getElevatorNumber();
    static void setElevatorNumber(int newElevatorNumber);

    /**
     * @brief �����ǹ��ڵ��ݵ�һЩ����
     */

    /// �������
    Elevator(QObject* parent, ELEVATOR_TYPE type, int id);
    Elevator(QObject* parent, int id);
    /// ��������
    ~Elevator();

    ///�ϳ˿�
    void loadPassenger(Passenger*);
    /// ��Ҫ�����ݵĳ˿Ͷ���������У����Ŷӽ������.
    void loadPassenger();
    /// ���³˿�
    void unLoadPassenger();
    /// ��ʾ�ò��г˿����µ���
    bool hasUnloaded(int floor);

    /**
     * @brief addOrder ���������ݽ����� floor �㷢���ź�
     * @return �Ƿ���ӳɹ������ܸò��޷���������Ѿ���Ա��
     */
    /// ����ⲿ����
    bool addOutOrder(Passenger* passenger);
    /// �Ƿ��ܵ��� floor ��
    bool ableToReach(int floor);
    /// ÿ S�봥��
    void running();

    /**
     * @brief ���������Ե� get/set ����
     */

    /// �����еĳ˿�������ʾ�����ڵĳ˿�����
    int getPassengerNumber();

    /// �������з���
    int getDirection() const;
    ELEVATOR_DIRECTION getElevatorDirection() const;
    void setDirection(ELEVATOR_DIRECTION newCurrentDirection);

    /// ����״̬
    int getMovingState() const;
    ELEVATOR_DIRECTION getCurrentMovingState() const;
    void setCurrentMovingState(ELEVATOR_DIRECTION newCurrentMoveDirection);

    /// ��������
    int getType() const;
    void setType(int newType);
    QString getTypeDescribe() const;

    /// id
    int getID() const;

    /// ��ǰ¥��
    int getCurrentFloor() const;
    void setCurrentFloor(int newCurrentFloor);
    void resetCurrentFloor();

    /// ���н���
    double getRunningProgress() const;
    void setRunningProgress();

    /// current floor++/--,
    /// ������ current floor changed �ź�
    void runUp();
    void runDown();

    /// max&min
    int getPlanMaxFloor() const;
    int getPlanMinFloor() const;
    void setPlanMaxFloor(int value);
    void setPlanMinFloor(int value);

    /// ��ȡ�ܾ���
    Q_INVOKABLE int getTotalDistance() const;
    Q_INVOKABLE int getConsume() const;

    /// ��ȡ����
    Q_INVOKABLE void setDescribe(QString str);
    Q_INVOKABLE QString getDescribe() const;

private:
    /// ��ʼ��
    void initElevator();

    /// ����Ƿ��������
    void checkKeepMoving();
private:
    /// ��������
    static QVector<Elevator*> elevators;
    static QMutex mutex;

    /// ��ȡ��������
    static QVector<ElevatorType> elevatorTypes;
    /// �󶨵�������
    /// <����id��type>
    static QMap<int, int> bindElevatorTypes;
    /// ������ɫ
    static QVector<QColor> colors;

    /// ����¥���ʱ����
    static int runningInterval;
    /// ��ʱ���Ĵ������ (��)
    /// static const float timerInterval;
    /// ����������
    static int maxNumber;
    /// ��������
    static int elevatorNumber;

    /// ��������
    double runningProgress;
    /// ��������
    int type;
    // ��ǰ����¥��
    int currentFloor;
    // �ƻ��ﵽ�����¥��
    int planMaxFloor;
    // �ƻ��ﵽ�����¥��
    int planMinFloor;
    // ��ǰ����Ŀ�����з���
    ELEVATOR_DIRECTION direction;
    // ��ǰ�������з���
    ELEVATOR_DIRECTION movingState;
    /// ���� id
    int id;
    /// �˿�����
    int passengerNumber;
    /// ����
    QString typeDescribe;
    /// �����й���
    int totalDistance;
    /// �ɵ����ÿһ���У�����һ���˿Ͷ���,
    /// ������Ի����������жϵ��ݵķ�Χ��
    QMap<int,QQueue<Passenger*>> passengerQueue;
    /// �����ݵĳ˿Ͷ���
    /// QQueue<Passenger*> enterQueue;
    /// ָ��ϣ�orders[s]==true ������s����Ҫͣ�����ؿ�ж��
    QMap<int,int> inOrders;
    QMap<int,QVector<Passenger*>> outerOrders;
};
