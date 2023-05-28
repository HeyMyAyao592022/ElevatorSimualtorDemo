#pragma once
#include <QTime>  
#include <QVector>
#include <QDebug>
#include <QString>
#include <QMessageBox>

/// ��������
#define ELEVATOR_NUM 10
/// ���¥��Ϊ20¥
#define MAX_FLOOR 40
/// һ��ʼ�˿Ͱ󶨵�ID
#define DEAULT_ELEVATOR_ID -1

enum PASSENGER_STATUS
{
	WAITING = 0,               //�ȴ�
	DISPATCHED_ELEVATOR = 1,  // ���䵽����
	IN_ELEVATOR = 2,          // �ڵ�����
	OUT_ELEVATOR = 3          // ������
};

enum ELEVATOR_DIRECTION
{
	UP = 0,//����
	DOWN = 1,//�½�
	STOP = 2   // ����ֹͣ
};

enum SCHEME_TYPE
{
    PEAK = 0, // �߷���
    SLACK, // �ͷ���
};

/// ��������
enum ELEVATOR_TYPE{
    TYPE01,
    TYPE23,
    TYPE45,
    TYPE67,
    TYPE89
};
