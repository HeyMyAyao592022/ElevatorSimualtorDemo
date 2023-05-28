#pragma once
#include <QTime>  
#include <QVector>
#include <QDebug>
#include <QString>
#include <QMessageBox>

/// 电梯总数
#define ELEVATOR_NUM 10
/// 最高楼层为20楼
#define MAX_FLOOR 40
/// 一开始乘客绑定的ID
#define DEAULT_ELEVATOR_ID -1

enum PASSENGER_STATUS
{
	WAITING = 0,               //等待
	DISPATCHED_ELEVATOR = 1,  // 分配到电梯
	IN_ELEVATOR = 2,          // 在电梯内
	OUT_ELEVATOR = 3          // 出电梯
};

enum ELEVATOR_DIRECTION
{
	UP = 0,//上升
	DOWN = 1,//下降
	STOP = 2   // 电梯停止
};

enum SCHEME_TYPE
{
    PEAK = 0, // 高峰期
    SLACK, // 低峰期
};

/// 电梯类型
enum ELEVATOR_TYPE{
    TYPE01,
    TYPE23,
    TYPE45,
    TYPE67,
    TYPE89
};
