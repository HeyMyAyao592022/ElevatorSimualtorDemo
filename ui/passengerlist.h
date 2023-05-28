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
    /// 存储的数据
    QList<Passenger*> dataList;
    /// 与之同步的队列
    /// QMap<int,QQueue<Passenger*>> passengerInFloors;

    /// 单例模式
    static QSharedPointer<PassengerList> instance;
    static QMutex mutex;
    QModelIndex modelIndex;
public:
    PassengerList(QObject* parent = nullptr);

    /// 获取实例
    static QSharedPointer<PassengerList>& getInstance();
    static WaitingQueue* getWaitingQueue();

    /**
     * @brief 实现接口
     */
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames()  const override;

    /// @brief 以下为暴露给 qml 的函数，希望不要随便调用.
    /// 增
    void insert(int index, Passenger* obj);
    void append(Passenger* obj);
    void push_front(Passenger* obj);
    /// 删
    void remove(Passenger* obj);
    void remove(int index);
    void clear();
    /// 改
    void replace(int index, Passenger* obj);
    void set(QList<Passenger*>& list);

    /// 获取数据
    Passenger* get(int index);
    QList<Passenger*>& getALL();

    /**
     * @brief 实现接口
     */
    bool hasPassengerWaiting(int floor) override;
    int passenerNumber(int floor) override;
    void appendPassenger(Passenger* passenger) override;
    Passenger* popPassenger(int floor) override;
    void removePassenger(Passenger* passenger) override;
};

#endif // PASSENGERLIST_H
