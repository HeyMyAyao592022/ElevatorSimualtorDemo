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
    /// qml ����
    QQmlApplicationEngine* qmlEngine;
    /// ��ȡʵ��
    static QSharedPointer<DispatchingSystem> instance;
    // �Ƿ�����ģ��
    bool flagStart;
    // ���� WaittingQueue ��Ҫ�����߳�ͬ�����������
    QMutex waitingQueueMutex;
    /// ������
    Control* control;
    /// view model
    QSharedPointer<ViewModel> viewmodel;
    /// �ȴ�����
    WaitingQueue* waitingDequeue;
    /// �������ɳ˿�
    QTimer generateTimer;
    /// ��¼�Ѿ����ɵĳ˿�
    int generateNumber;
    /// ʱ��
    QDateTime startTime;
    QDateTime endTime;
public:
    //���캯��
    DispatchingSystem(QObject *parent = nullptr);

    /// ��ȡʵ��
    static QSharedPointer<DispatchingSystem> getInstance();

    //��������
    ~DispatchingSystem();

    //��������
    /// ���̵߳Ĳ�����������������
    void run();

    /// ��ȡ qml ����
    QQmlApplicationEngine* getQmlEngine();

private:
    /// ��ӳ˿�
    void appendPassenegr();

    /// ���ɳ˿�
    void generatePassenger();

    /// �����źźͲ�
    void initSlots();

    /// ��ʼ������
    void iniElevators();

    /// ����ֹͣ
    void startSystem();
    void stopSystem();
};
