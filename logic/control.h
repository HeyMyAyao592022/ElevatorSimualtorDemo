#ifndef CONTROL_H
#define CONTROL_H

#include "Utilities.h"
#include "Passenger.h"
#include "waitingqueue.h"
#include "Elevator.h"
#include "passengerlist.h"

#include <QObject>
#include <QVector>
#include <QList>
#include <QPair>
#include <QTimer>
#include <QSharedPointer>

/*~��������ԭ�����ֵ���
1����վ������վԭ����������£�һ̨�����ڻ�վ������Ϊ���ݣ���һ̨����
���ͣ����¥�㣬�˵��ݳ�Ϊ�����ݡ���AΪ������ BΪ���ݣ�
�˿����ɺ��1 2 3 4ĳһ�ֵ��ݣ��ٸ��������з���ȷ��Ҫ���ػ��ݻ��������ݡ�
��1����������A������������ʱ������Ϸ������ٻ��ź����ɵ���Aȥ��ɣ�����
��վ��B���ݲ���Ӧ�����A�����·������ٻ��ź����ɵ���B�ش���źŲ�������
������A������������ʱ������Ϸ������ٻ��ź����ɵ���Bȥ��ɣ����A������
�������ٻ��ź����ɵ���A�ش���źŲ�������
//��2�����������A�������У�����¥���ٻ��źŶ࣬������B���߱�������������
��40s���ɵ����������ٻ��ź���Ȼ���ڣ�����B���ݷ�����
��3��������̨���ݽ������С������������з���һ�£����ɽϽ�������Ӧ��
��4��������̨���ݽ������С������������з���һ�£�A����B���£���A�Ϸ�
���������ٻ��ź���AӦ��A�·����������ٻ��ź���BӦ����B�Ϸ���������
�ٻ��ź���AӦ��B�·����������ٻ��ź���BӦ��
*/

/**
 * @brief The Control class
 * ��Ҫ������ճ˿ͷ��������󣬲�����ݷ���ָ��
 */
class Control
        : public QObject
{
    Q_OBJECT
public:
signals:
    void elevatorRunning(int id);
private:
    /// ������
    const QVector<Elevator*>* elevators;
    /// �˿͵ȴ�����
    WaitingQueue* waitingQueue;
public:
    explicit Control(QObject *parent = nullptr);
    /// ��ʼ������
    void iniEls();
    /// ִ�е��Ȳ���
    void doDispatching(Passenger* passenger);
    /// ��
    void connectElevator(Passenger* passenger);
private:
    void initSlots();
    /// ���ݵ�������󶨵���
    QPair<Elevator*,Elevator*> targetElevator(int from, int to);

    /// 2�ֲ���
    void connectElPeak(Passenger* passenger);
    void connectElSlack(Passenger* passenger);

    /// ����ģʽ
    SCHEME_TYPE schemeType;
};

#endif // CONTROL_H
