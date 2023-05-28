#include "waitingqueue.h"

WaitingQueue::WaitingQueue()
{

}

const QMap<int, QVector<Passenger *> > &WaitingQueue::waitingMap() const
{
    return this->map;
}
