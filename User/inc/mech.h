#ifndef _NEW_MECH_H
#define _NEW_MECH_H

#include "DJmotor.h"
#include "Zdrive.h"
#include "beep.h"
#include "cylinder.h"
#include "mathFunc.h"
#include "mech2.h"
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

// CanRxMsg		 RX_message;	// Can1Funtion() �� R1_Task() ���õı���
// volatile uint8_t ChooseFlag;
// int				 Cnt6;

typedef struct {
	int		 SeedTimes;		// �Ѿ��ù�����Ĵ�������һ������Ҫ���棬�ڶ��β���Ҫ�������õ����Ρ�
	long int ReachCnt;		// ����Ƿ�λ�ļ�����
	int		 DuZhuanCnt;	// ����Ƿ�λ�ļ�����
	int		 FirstPut;		// �ȷ��ұߡ�FirstΪ0��ʾ�ұ߻�û�ţ�1��ʾ�ұ߷����ˣ�Firstִ�й��Ĵ�����
	float	 BallPawAngleUp;		// ����Ƕ�
	float	 BallPawAngleDown;		// ץȡ��ĽǶ�
	float	 BallPawAngleSecure;	// ��ĽǶ�
	float	 BallPawtoZeroAngle;
	float	 FireUpDownAngleDown;	 // ���������
	float	 SeedCatchAngle;		 // ȡ��ĽǶ�/����ĽǶ�
	float	 SeedPutAngle;			 // ����ĽǶ�
	float	 SeedStorageGoAngle;	 // ���������ת�߽Ƕ�
	float	 SeedStorageAngle;		 // �����������ĽǶ�
	float	 FireAngleMax;			 // ����֮�󵽴�ĽǶ�
	float	 FireOriginal;			 // �߱� ��ʼλ��

} MechDef;

// 12�����㷢���������
typedef struct {
	float FireSteeringAngle;	// �˵������ת�ǣ�ˮƽ����
	float FireUpDownAngle;		// �˵ظ����ǣ���ֱ����
	float FireArmSpeed;
} FirePlace;

// ����
void MechInit(void);	// ������ʼ��

// void PawDown(DJMOTOR Paw);	  // צ�ӷ���
// void PawCatch(DJMOTORpointer Paw);	  // צ������ȥ

void Can1Funtion(CanRxMsg rx_message);	  // can1�жϴ�����
void XXable(CanRxMsg rx_message);		  // ʹ��or ʧ��
void FindZero(CanRxMsg rx_message);		  // Ѱ��
void BallPaw(CanRxMsg rx_message);
void Seeding(CanRxMsg rx_message);
void YouBack(CanRxMsg rx_message);
void FireInHere(CanRxMsg rx_message);
void SetParameters(CanRxMsg rx_message);
void CanBack(CanRxMsg rx_message);

uint8_t CylinderCalculate(int Paw_, int command);

void DJ_Check_place(DJMOTORpointer DJ);

#endif
