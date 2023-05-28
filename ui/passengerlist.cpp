#include "passengerlist.h"

QSharedPointer<PassengerList> PassengerList::instance;
QMutex PassengerList::mutex;

PassengerList::PassengerList(QObject* parent)
    : QAbstractListModel(parent)
{
    /// dataList.append(new Passenger(nullptr, 0));
    /// qDebug()<<".."<<dataList.size();
}

/**
 * @brief PassengerList::getInstance
 * 单例模式
 * @return
 */
QSharedPointer<PassengerList>& PassengerList::getInstance()
{
    if (instance.isNull())
    {
        QMutexLocker mutexLocker(&mutex);
        if (instance.isNull())
            instance = QSharedPointer<PassengerList>(new PassengerList());
    }
    return instance;
}

/**
 * @brief 接口
 */
WaitingQueue *PassengerList::getWaitingQueue()
{
    return getInstance().get();
}

int PassengerList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return dataList.count();
}

QVariant PassengerList::data(const QModelIndex &index, int role) const
{
    if (index.row() >= 0 && index.row() < dataList.size() ) {
        if (role == ObjectModelRole) {
            QObject *object = dataList.at(index.row());
            return QVariant::fromValue(object);
        }
    }
    return QVariant();
}

QHash<int, QByteArray> PassengerList::roleNames() const
{
    QHash<int, QByteArray>  roleName;
    roleName[ObjectModelRole] = "objectModelRole";
    return  roleName;
}

/**
 * @brief 增
 */
void PassengerList::insert(int index, Passenger *obj)
{
    if (index >= 0  && index <= dataList.size()) {
        mutex.lock();
        beginInsertRows(QModelIndex(), index, index);
        dataList.insert(index, obj);
        endInsertRows();
        mutex.unlock();
    }
}

void PassengerList::append(Passenger *obj)
{
    insert(dataList.size(),obj);
}

void PassengerList::push_front(Passenger *obj)
{
    insert(0,obj);
}

/**
 * @brief 删
 */
void PassengerList::remove(Passenger *obj)
{
    mutex.lock();
    const int index = dataList.indexOf(obj);
    //qDebug()<<QString("in psg list: removing psgr%1 in ui, and its index is %2").arg(obj->getPassengerID()).arg(index);
    if (index >= 0) {
        beginRemoveRows(QModelIndex(), index, index);
        dataList.removeAt(index);
        endRemoveRows();
    }
    mutex.unlock();
}

void PassengerList::remove(int index)
{
    if (index >= 0 && index < dataList.size()) {
        mutex.lock();
        beginRemoveRows(QModelIndex(), index, index);
        QObject* object = dataList.at(index);
        dataList.removeAt(index);
        object->deleteLater();
        endRemoveRows();
        mutex.unlock();
    }
}

void PassengerList::clear()
{
    mutex.lock();
    beginResetModel();
    qDeleteAll(dataList.begin(), dataList.end());
    dataList.clear();
    endResetModel();
    mutex.unlock();
}

/**
 * @brief 改
 */
void PassengerList::replace(int index, Passenger *obj)
{
    if (index >= 0  && index < dataList.size()) {
        mutex.lock();
        dataList.replace(index, obj);
        mutex.unlock();
        emit dataChanged(createIndex(index, 0), createIndex(index, 0));
    }
}

void PassengerList::set(QList<Passenger*> &list)
{
    mutex.lock();
    beginResetModel();
    dataList.swap(list);
    endResetModel();
    mutex.unlock();
}

/**
 * @brief 获取数据
 */
Passenger *PassengerList::get(int index)
{
    if (index >= 0 && index < dataList.size()) {
        mutex.lock();
        auto object =dataList.at(index);
        mutex.unlock();
        return object;
    }
    return nullptr;
}

QList<Passenger*> &PassengerList::getALL()
{
    mutex.lock();
    auto& objectList = dataList;
    mutex.unlock();
    return objectList;
}


/**
 * @brief 实现接口
 */
bool PassengerList::hasPassengerWaiting(int floor)
{
    return this->map.contains(floor);
}

int PassengerList::passenerNumber(int floor)
{
    if(!map.contains(floor))
        return 0;
    return map[floor].size();
}

void PassengerList::appendPassenger(Passenger *passenger)
{
    /// 如果 map 在该层没有对应的数组，那就加上去
    if(!map.contains(passenger->getFrom())){
        map[passenger->getFrom()] = QVector<Passenger*>();
    }
    /// 在 map 中添加这个乘客,
    /// map[passenger->getFrom()]是指某一层的乘客数组
    map[passenger->getFrom()].append(passenger);
    /// 调用 qabstraclistmodel 的接口
    /// 同步添加到 passenger list 的 qlist 中
    this->append(passenger);
}

Passenger *PassengerList::popPassenger(int floor)
{
    if(!map.contains(floor) || map[floor].empty())
        return nullptr;
    auto passenger = map[floor].front();
    /// 调用 qabstraclistmodel 的接口
    remove(passenger);
    /// 在 map 中的乘客也要同步删除
    map[floor].pop_front();
    return passenger;
}

void PassengerList::removePassenger(Passenger *passenger)
{
    int floor = passenger->getFrom();
    if(map.contains(floor))
        map[floor].removeOne(passenger);
    remove(passenger);
}


