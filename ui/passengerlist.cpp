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
 * ����ģʽ
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
 * @brief �ӿ�
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
 * @brief ��
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
 * @brief ɾ
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
 * @brief ��
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
 * @brief ��ȡ����
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
 * @brief ʵ�ֽӿ�
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
    /// ��� map �ڸò�û�ж�Ӧ�����飬�Ǿͼ���ȥ
    if(!map.contains(passenger->getFrom())){
        map[passenger->getFrom()] = QVector<Passenger*>();
    }
    /// �� map ���������˿�,
    /// map[passenger->getFrom()]��ָĳһ��ĳ˿�����
    map[passenger->getFrom()].append(passenger);
    /// ���� qabstraclistmodel �Ľӿ�
    /// ͬ����ӵ� passenger list �� qlist ��
    this->append(passenger);
}

Passenger *PassengerList::popPassenger(int floor)
{
    if(!map.contains(floor) || map[floor].empty())
        return nullptr;
    auto passenger = map[floor].front();
    /// ���� qabstraclistmodel �Ľӿ�
    remove(passenger);
    /// �� map �еĳ˿�ҲҪͬ��ɾ��
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


