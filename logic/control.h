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

/*~并联调度原则，四种电梯
1、基站和自由站原则：正常情况下，一台电梯在基站待命作为基梯，另一台留在
最后停靠的楼层，此电梯称为自由梯。（A为自有梯 B为基梯）
乘客生成后绑定1 2 3 4某一种电梯，再根据其运行方向确定要搭载基梯还是自由梯。
（1）、当电梯A正在向上运行时，如果上方出现召唤信号则由电梯A去完成，而在
基站的B电梯不予应答，如果A电梯下方出现召唤信号则由电梯B回答该信号并发车。
当电梯A正在向下运行时，如果上方出现召唤信号则由电梯B去完成，如果A电梯下
方出现召唤信号则由电梯A回答该信号并发车。
//（2）、如果电梯A正在运行，其他楼层召唤信号多，而基梯B不具备发车条件，经
过40s（可调整）后若召唤信号仍然存在，则令B电梯发车。
（3）、若两台电梯皆在运行。且两电梯运行方向一致，则由较近电梯响应。
（4）、若两台电梯皆在运行。且两电梯运行方向不一致，A向上B向下，若A上方
出现向上召唤信号则A应答，A下方出现向下召唤信号则B应答；若B上方出现向下
召唤信号则A应答，B下方出现向下召唤信号则B应答。
*/

/**
 * @brief The Control class
 * 主要负责接收乘客发出的请求，并向电梯发出指令
 */
class Control
        : public QObject
{
    Q_OBJECT
public:
signals:
    void elevatorRunning(int id);
private:
    /// 电梯组
    const QVector<Elevator*>* elevators;
    /// 乘客等待队列
    WaitingQueue* waitingQueue;
public:
    explicit Control(QObject *parent = nullptr);
    /// 初始化电梯
    void iniEls();
    /// 执行调度策略
    void doDispatching(Passenger* passenger);
    /// 绑定
    void connectElevator(Passenger* passenger);
private:
    void initSlots();
    /// 根据电梯种类绑定电梯
    QPair<Elevator*,Elevator*> targetElevator(int from, int to);

    /// 2种策略
    void connectElPeak(Passenger* passenger);
    void connectElSlack(Passenger* passenger);

    /// 策略模式
    SCHEME_TYPE schemeType;
};

#endif // CONTROL_H
