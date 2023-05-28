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
    // �������
    Q_PROPERTY(int finishedNumber READ getFinishedNumber NOTIFY finishedNumberChanged)
    // �˿�����
    Q_PROPERTY(int passengerNumber READ getPassengerNumber WRITE setPassengerNumber NOTIFY passengerNumberChanged)
    // ���������ٶ�
    Q_PROPERTY(int elevatorInterval READ getRunningInterval WRITE setRunningInterval NOTIFY runningIntervalChanged)
    // ��������
    Q_PROPERTY(int elNum READ getElNum WRITE setElNum NOTIFY elNumChanged)
    // ������������
    Q_PROPERTY(int elMaxNum READ getElMaxNum WRITE setElMaxNum NOTIFY elMaxNumChanged)
public:
signals:
    /// ��ʼ������
    void iniElsSignal();
    void finishIniEls();
    /// ��ʼ����
    void startSimulating();
    /// ����
    void finishSimulating();
    // ����
    void passengerNumberChanged();
    void finishedNumberChanged();
    void runningIntervalChanged();
    void elNumChanged();
    void elMaxNumChanged();
    /// �����³˿�
    void generateNewPassenger(int id, int from, int to);
    /// �󶨳˿�
    void connectPassenger(int psgID, int elID);
private:
    /// ����ͬһ��ʵ��
    static QSharedPointer<ViewModel> instance;
    /// �̰߳�ȫ
    static QMutex mutex;
    /// �˿͵ȴ�����
    QSharedPointer<PassengerList> passengerList;
    /// ʱ��
    QPair<QDateTime, QDateTime> times;
    QString totalTime;
public:
    explicit ViewModel(QObject *parent = nullptr);

    /**
     * @brief getInstance ����ģʽ
     * @param parent
     * @return
     */
    static QSharedPointer<ViewModel>& getInstance();

    /**
     * @brief emitIniElSignal
     * �� qml ���ã���������ᷢ���źţ�֪ͨϵͳ��ʼ������
     */
    Q_INVOKABLE void emitIniElSignal();
    void iniEls();

    /// @brief ��ɼ��ص���
    void emitFinishIniElSignal();

    /**
     * @brief emitStartSignal
     * �� qml ���ã���������ᷢ���źţ�֪ͨ����ϵͳ��ʼ����
     */
    Q_INVOKABLE void emitStartSignal();

    /// @brief �����ź�
    void emitFinishSignal();

    /// @brief �󶨵���
    Q_INVOKABLE void bindElTypes(int elID, int typeID);

    /// @brief ��������
    Q_INVOKABLE void setTypesNum(int num);
    Q_INVOKABLE void iniTypes(int typeID, int floor);
    Q_INVOKABLE void setTypeDescribe(int id, QString str);

    /// @brief �����³˿�
    void emitGenerateSignal(int id, int from, int to);

    /// @brief �󶨳˿�
    void emitConnectSiganl(int psgID, int elID);

    /**
     * @brief objectModel
     * ��¶�� qml ��
     * @return
     */
    Q_INVOKABLE PassengerList* objectModel();

    /// ��ȡ�˿�����
    int getPassengerNumber();
    /// ���ó˿�����
    void setPassengerNumber(int num);

    /// ��ȡ��ɵĳ˿�����
    int getFinishedNumber();

    /// ��ȡ��������һ���ʱ��
    int getRunningInterval() const;
    /// ���õ�������һ���ʱ��
    void setRunningInterval(int val);

    /// ������������
    int getElMaxNum() const;
    void setElMaxNum(int num);

    /// ��������
    int getElNum() const;
    void setElNum(int num);

    /// ʱ��
    void setTime(QDateTime start, QDateTime end);
    Q_INVOKABLE QString getTotalTime();
    void setTotalTime(QString time);
private:
    void initSlots();
};

#endif // VIEWMODEL_H
