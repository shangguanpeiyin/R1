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

// CanRxMsg		 RX_message;	// Can1Funtion() 和 R1_Task() 共用的变量
// volatile uint8_t ChooseFlag;
// int				 Cnt6;

typedef struct {
	int		 SeedTimes;		// 已经拿过秧苗的次数。第一次拿需要储存，第二次不需要，不能拿第三次。
	long int ReachCnt;		// 电机是否到位的计数器
	int		 DuZhuanCnt;	// 电机是否到位的计数器
	int		 FirstPut;		// 先放右边。First为0表示右边还没放，1表示右边放完了（First执行过的次数）
	float	 BallPawAngleUp;		// 放球角度
	float	 BallPawAngleDown;		// 抓取球的角度
	float	 BallPawAngleSecure;	// 球的角度
	float	 BallPawtoZeroAngle;
	float	 FireUpDownAngleDown;	 // 俯仰角最低
	float	 SeedCatchAngle;		 // 取秧的角度/放秧的角度
	float	 SeedPutAngle;			 // 存秧的角度
	float	 SeedStorageGoAngle;	 // 存秧机构，转走角度
	float	 SeedStorageAngle;		 // 存放秧苗机构的角度
	float	 FireAngleMax;			 // 踢完之后到达的角度
	float	 FireOriginal;			 // 踢臂 初始位置

} MechDef;

// 12个定点发射参数储存
typedef struct {
	float FireSteeringAngle;	// 此地踢射回转角（水平朝向）
	float FireUpDownAngle;		// 此地俯仰角（垂直朝向）
	float FireArmSpeed;
} FirePlace;

// 函数
void MechInit(void);	// 机构初始化

// void PawDown(DJMOTOR Paw);	  // 爪子放下
// void PawCatch(DJMOTORpointer Paw);	  // 爪子拿上去

void Can1Funtion(CanRxMsg rx_message);	  // can1中断处理函数
void XXable(CanRxMsg rx_message);		  // 使能or 失能
void FindZero(CanRxMsg rx_message);		  // 寻零
void BallPaw(CanRxMsg rx_message);
void Seeding(CanRxMsg rx_message);
void YouBack(CanRxMsg rx_message);
void FireInHere(CanRxMsg rx_message);
void SetParameters(CanRxMsg rx_message);
void CanBack(CanRxMsg rx_message);

uint8_t CylinderCalculate(int Paw_, int command);

void DJ_Check_place(DJMOTORpointer DJ);

#endif
