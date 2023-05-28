#ifndef PASSENGERLIST_H
#define PASSENGERLIST_H

#include "Passenger.h"
#include "waitingqueue.h"

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QSharedPointer>
#include <QMutex>
#include <QModelIndex>
#include <QMap>
#include <QQueue>

#include <QDebug>

class PassengerList
        : public QAbstractListModel
        , public WaitingQueue
{
    Q_OBJECT
public:
    enum dataRoles{
        ObjectModelRole = Qt::UserRole + 1,
    };

private:
    /// �洢������
    QList<Passenger*> dataList;
    /// ��֮ͬ���Ķ���
    /// QMap<int,QQueue<Passenger*>> passengerInFloors;

    /// ����ģʽ
    static QSharedPointer<PassengerList> instance;
    static QMutex mutex;
    QModelIndex modelIndex;
public:
    PassengerList(QObject* parent = nullptr);

    /// ��ȡʵ��
    static QSharedPointer<PassengerList>& getInstance();
    static WaitingQueue* getWaitingQueue();

    /**
     * @brief ʵ�ֽӿ�
     */
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames()  const override;

    /// @brief ����Ϊ��¶�� qml �ĺ�����ϣ����Ҫ������.
    /// ��
    void insert(int index, Passenger* obj);
    void append(Passenger* obj);
    void push_front(Passenger* obj);
    /// ɾ
    void remove(Passenger* obj);
    void remove(int index);
    void clear();
    /// ��
    void replace(int index, Passenger* obj);
    void set(QList<Passenger*>& list);

    /// ��ȡ����
    Passenger* get(int index);
    QList<Passenger*>& getALL();

    /**
     * @brief ʵ�ֽӿ�
     */
    bool hasPassengerWaiting(int floor) override;
    int passenerNumber(int floor) override;
    void appendPassenger(Passenger* passenger) override;
    Passenger* popPassenger(int floor) override;
    void removePassenger(Passenger* passenger) override;
};

#endif // PASSENGERLIST_H
