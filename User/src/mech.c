#include "mech.h"

FirePlace Here[13];	   // 12个定点发射地 的参数，0为debug下发射
MechDef	  Mech;

uint8_t CylinderControl;

CanRxMsg		 RX_message;	// Can1Funtion() 和 R1_Task() 共用的变量
volatile uint8_t ChooseFlag;
int				 Cnt6;

uint8_t SeedPaw1Cylinder;	 // 左。取秧气缸的位（倒数第几位）
uint8_t SeedPaw2Cylinder;

void MechInit()
{
	Cnt6			= 0;

	CylinderControl = 0;
	ChooseFlag		= 0;

	Mech.SeedTimes	= 0;	// 已经拿过秧苗的次数。第一次拿需要储存，第二次不需要，不能拿第三次。
	Mech.ReachCnt	= 0;	// 电机是否到位的计数器
	Mech.DuZhuanCnt = 0;	// 电机是否堵转的计数器
	Mech.FirstPut	= 0;
	// 这里当然不是0，具体参数是多少待搓
	// 各种参数待搓
	Mech.BallPawAngleUp			   = 496;	 // 球的角度（要抬多少）
	Mech.BallPawAngleDown		   = 113;	 // 抓取球的角度（抓球的时候的角度）（左负右正）
	Mech.BallPawAngleSecure		   = 316.8;
	Mech.SeedCatchAngle			   = -21;	 // 取秧的角度
	Mech.SeedPutAngle			   = -81;	 // 放秧的角度
	Mech.SeedStorageAngle		   = 71;	 // 存放秧苗机构的角度
	Mech.SeedStorageGoAngle		   = 36;	 // 存放机构安全角度
	Mech.BallPawtoZeroAngle		   = 0;

	Mech.FireAngleMax			   = 0;
	Mech.FireOriginal			   = 0;
	Mech.FireUpDownAngleDown	   = 0;	   // 俯仰角最低

	BallPaw1.SteeringAngle		   = 447.79;	 // 左边取球回转角
	BallPaw2.SteeringAngle		   = -451.76;	 // 右边取球回转角

	// 初始化PVT模式的参数
	// 待搓
	FireArm.PVTMode.ReachTime[0]   = 0;
	FireArm.PVTMode.TargetSpeed[0] = 0;
	FireArm.PVTMode.TargetAngle[0] = 0;

	FireArm.PVTMode.ReachTime[1]   = 0;
	FireArm.PVTMode.TargetAngle[1] = 0;

	FireArm.PVTMode.ReachTime[2]   = 0;
	FireArm.PVTMode.TargetAngle[2] = 0;
	FireArm.PVTMode.TargetSpeed[2] = 0;	   // 这里是0，不用搓

	{	 // 初始化12个发射点，参数待搓
		// 待搓
		Here[0].FireSteeringAngle  = 0;	   // 0为debug下发射
		Here[0].FireUpDownAngle	   = 0;
		Here[0].FireArmSpeed	   = 0;

		Here[1].FireSteeringAngle  = 0;
		Here[1].FireUpDownAngle	   = 0;
		Here[1].FireArmSpeed	   = 0;

		Here[2].FireSteeringAngle  = 0;
		Here[2].FireUpDownAngle	   = 0;
		Here[2].FireArmSpeed	   = 0;

		Here[3].FireSteeringAngle  = 0;
		Here[3].FireUpDownAngle	   = 0;
		Here[3].FireArmSpeed	   = 0;

		Here[4].FireSteeringAngle  = 0;
		Here[4].FireUpDownAngle	   = 0;
		Here[4].FireArmSpeed	   = 0;

		Here[5].FireSteeringAngle  = 0;
		Here[5].FireUpDownAngle	   = 0;
		Here[5].FireArmSpeed	   = 0;

		Here[6].FireSteeringAngle  = 0;
		Here[6].FireUpDownAngle	   = 0;
		Here[6].FireArmSpeed	   = 0;

		Here[7].FireSteeringAngle  = 0;
		Here[7].FireUpDownAngle	   = 0;
		Here[7].FireArmSpeed	   = 0;

		Here[8].FireSteeringAngle  = 0;
		Here[8].FireUpDownAngle	   = 0;
		Here[8].FireArmSpeed	   = 0;

		Here[9].FireSteeringAngle  = 0;
		Here[9].FireUpDownAngle	   = 0;
		Here[9].FireArmSpeed	   = 0;

		Here[10].FireSteeringAngle = 0;
		Here[10].FireUpDownAngle   = 0;
		Here[10].FireArmSpeed	   = 0;

		Here[11].FireSteeringAngle = 0;
		Here[11].FireUpDownAngle   = 0;
		Here[11].FireArmSpeed	   = 0;

		Here[12].FireSteeringAngle = 0;
		Here[12].FireUpDownAngle   = 0;
		Here[12].FireArmSpeed	   = 0;
	}
	// 初始化电机的气缸，此电机的气缸的控制位是倒数第几位。
	// 待调整
	BallPaw1.Cylinder			   = 6;	   // 左
	BallPaw2.Cylinder			   = 5;
	SeedPaw1Cylinder			   = 8;
	SeedPaw2Cylinder			   = 7;

	// 初始化电机寻零速度，不可都用一个速度，因为寻零方向可能不一样
	SeedPaw.Limit.ZeroSpeed		   = 500;	   // √
	SeedStorage.Limit.ZeroSpeed	   = -1500;	   // √
	BallPaw1.Limit.ZeroSpeed	   = 500;
	BallPaw2.Limit.ZeroSpeed	   = 500;

	// PVT踢射参数
	// 待搓，如果需要可以简化语句
	FireArm.PVTMode.ReachTime[0]   = 0;	   // 加速阶断
	FireArm.PVTMode.TargetAngle[0] = 0;
	FireArm.PVTMode.TargetSpeed[0] = 0;
	FireArm.PVTMode.TargetSpeed[1] = 0;

	FireArm.PVTMode.ReachTime[1]   = 0;	   // 匀速阶断
	FireArm.PVTMode.TargetAngle[1] = 0;

	FireArm.PVTMode.ReachTime[2]   = 0;	   // 减速阶断
	FireArm.PVTMode.TargetAngle[2] = 0;
	FireArm.PVTMode.TargetSpeed[2] = 0;	   // 0 不用改

	// 都用位置模式，初始化直接设置，不必再次修改了
	SeedPaw.mode				   = DJ_POSITION;
	SeedStorage.mode			   = DJ_POSITION;
	BallPaw1.mode				   = DJ_POSITION;
	BallPaw2.mode				   = DJ_POSITION;
	FireSteering.mode			   = DJ_POSITION;
	FireUpDown.mode				   = DJ_POSITION;
	FireArm.mode				   = Zdrive_Position;
}

// 使能、失能
void XXable(CanRxMsg rx_message)
{
	if (rx_message.Data[1] & 1)	   // 取最后一位，seeding
	{
		SeedPaw.enable	   = true;
		SeedStorage.enable = true;
	} else {
		SeedPaw.enable	   = false;
		SeedStorage.enable = false;
	}

	if (rx_message.Data[1] >> 1 & 1)	// 取倒数第二位，ball
	{
		BallPaw1.enable = true;
		BallPaw2.enable = true;
	} else {
		BallPaw1.enable = false;
		BallPaw2.enable = false;
	}

	if (rx_message.Data[1] >> 2 & 1)	// 取倒数第三位，fire
	{
		FireSteering.enable = true;
		FireUpDown.enable	= true;
		FireArm.enable		= true;
	} else {
		FireSteering.enable = false;
		FireUpDown.enable	= false;
		FireArm.enable		= false;
	}

	CanBack(rx_message);	// 以下是反馈报文
}

// 寻零
void FindZero(CanRxMsg rx_message)
{
	if (rx_message.Data[1] & 1)	   // 取最后一位，seeding
	{
		SeedPaw.enable	   = true;
		SeedStorage.enable = true;

		SeedPaw.begin	   = false;
		SeedStorage.begin  = false;

		SeedPaw.mode	   = DJ_ZERO;
		SeedStorage.mode   = DJ_ZERO;

		SeedPaw.begin	   = true;
		SeedStorage.begin  = true;	  // 寻零成功后自动false
		while (1) {
			if (SeedPaw.begin == false && SeedStorage.begin == false)
				break;
		}
		SeedPaw.mode		   = DJ_POSITION;
		SeedStorage.mode	   = DJ_POSITION;
		// SeedPaw.enable	   = false;
		// SeedStorage.enable = false;

		SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // 伸手拿
		SeedPaw.begin		   = true;

		DJ_Check_place(&SeedPaw);	 // 到位判断

	}	 // 0表示不寻零，没有指令操作

	if (rx_message.Data[1] >> 1 & 1)	// 取倒数第二位，ball
	{
		BallPaw1.begin = false;
		BallPaw2.begin = false;

		BallPaw1.mode  = DJ_ZERO;
		BallPaw2.mode  = DJ_ZERO;

		BallPaw1.begin = true;
		BallPaw2.begin = true;	  // 寻零成功后自动false
		while (1) {
			OSTimeDly(50);
			if (SeedPaw.begin == false && SeedStorage.begin == false)
				break;
		}
		OSTimeDly(2000);

		BallPaw2.mode			= DJ_POSITION;
		BallPaw1.mode			= DJ_POSITION;
		BallPaw1.ValueSet.angle = Mech.BallPawtoZeroAngle;
		BallPaw2.ValueSet.angle = Mech.BallPawtoZeroAngle;
		BallPaw1.begin			= true;
		BallPaw2.begin			= true;	   // 寻零成功后自动false
		DJ_Check_place(&BallPaw1);
		DJ_Check_place(&BallPaw2);	  // 寻零之后爪子收上去（人扶着fire而寻零。注意安全（爪子））

		BallPaw1.begin				  = false;
		BallPaw2.begin				  = false;
		BallPaw1.StatusFlag.isSetZero = 1;
		BallPaw2.StatusFlag.isSetZero = 1;
	}

	if (rx_message.Data[1] >> 2 & 1)	// 取倒数第三位，fire
	{
		FireArm.begin					  = false;
		FireUpDown.begin				  = false;
		FireSteering.begin				  = false;

		FireSteering.StatusFlag.isSetZero = 1;	  // 设置当前位置为0
		FireUpDown.StatusFlag.isSetZero	  = 1;
		Zdrive_SetZero(FireArmId);	  // 设置当前位置为0

		// FireArm.enable		= false;
		// FireUpDown.enable	= false;
		// FireSteering.enable = false;

		FireArm.mode	  = Zdrive_Position;
		FireUpDown.mode	  = DJ_POSITION;
		FireSteering.mode = DJ_POSITION;
	}

	CanBack(rx_message);	// 以下是反馈报文
}

void SetParameters(CanRxMsg rx_message)
{
	Here[0].FireUpDownAngle	  = (rx_message.Data[0] << 8 | rx_message.Data[1]) / 10.0;
	Here[0].FireSteeringAngle = (rx_message.Data[2] << 8 | rx_message.Data[3]) / 10.0;
	Here[0].FireArmSpeed	  = (rx_message.Data[4] << 8 | rx_message.Data[5]);

	CanBack(rx_message);	// 以下是反馈报文
}

void FireInHere(CanRxMsg rx_message)	// 闭环
{
	if (rx_message.Data[1] > 0) {
		// 回转与俯仰      1-12为12个定点发射地
		FireSteering.ValueSet.angle = Here[rx_message.Data[1]].FireSteeringAngle;	 // Here[1-12]
		FireUpDown.ValueSet.angle	= Here[rx_message.Data[1]].FireUpDownAngle;

		FireSteering.begin			= true;
		FireUpDown.begin			= true;

		// 检测回转俯仰到位
		DJ_Check_place(&FireUpDown);
		DJ_Check_place(&FireSteering);

		FireArm.ValueSet.angle = Mech.FireOriginal;
		FireArm.begin		   = true;
		OSTimeDly(5000);

		// Arm用PVT模式启动
		FireArm.PVTMode.TargetSpeed[1] = Here[rx_message.Data[1]].FireArmSpeed;

		FireArm.PVTMode.PVTModeFlag	   = true;	  // 启用PVT模式
		FireArm.begin				   = true;

	} else if (rx_message.Data[1] == 0) {	 // debug调试模式
		// 回转与俯仰      1-12为12个定点发射地
		FireSteering.ValueSet.angle = Here[0].FireSteeringAngle;	// Here[1-12]
		FireUpDown.ValueSet.angle	= Here[0].FireUpDownAngle;

		FireSteering.begin			= true;
		FireUpDown.begin			= true;

		// 检测回转俯仰到位
		DJ_Check_place(&FireUpDown);
		DJ_Check_place(&FireSteering);

		FireArm.ValueSet.angle = Mech.FireOriginal;
		FireArm.begin		   = true;
		OSTimeDly(5000);

		// Arm用PVT模式启动
		FireArm.PVTMode.TargetSpeed[1] = Here[0].FireArmSpeed;

		FireArm.PVTMode.PVTModeFlag	   = true;	  // 启用PVT模式
		FireArm.begin				   = true;
	}

	OSTimeDly(5000);
	// 检测Arm到位（Zdrive）
	if (FireArm.mode == Zdrive_Position) {
		while (1) {
			// 检测是否到位
			if (ABS(FireArm.ValueReal.angle - FireArm.PVTMode.TargetAngle[2]) < Reach_THRESHOLD) {	  // 累计小于门槛后，视为到位
				if (Mech.ReachCnt++ > 100) {
					Mech.ReachCnt	= 0;
					Mech.DuZhuanCnt = 0;

					FireArm.begin	= false;
					break;
				}
			} else
				Mech.ReachCnt = 0;	  // 防止 积累偶然成功

			// 检测是否堵转
			if (ABS(FireArm.ValueReal.angle - FireArm.ValuePre.angle) < DuZhuan_THRESHOLD) {
				if (Mech.DuZhuanCnt++ > 100) {
					Mech.DuZhuanCnt = 0;
					Mech.ReachCnt	= 0;
					AnswerError(IdTransmiter(FireArmId), DuZhuan);
					while (1) {
						Beep.BeepOnNum++;
						OSTimeDly(500);	   // 报错了，蜂鸣器一直响，不用停
					}
				}
			} else
				Mech.DuZhuanCnt = 0;	// 防止 偶然积累
		}
	}

	// 踢射臂回位
	FireArm.ValueSet.angle = 0;	   // 回到零点，防止与其他机构干涉
	FireArm.begin		   = true;

	// 检测Arm到位（Zdrive）
	if (FireArm.mode == Zdrive_Position) {
		while (1) {
			// 检测是否到位
			if (ABS(FireArm.ValueReal.angle - FireArm.ValueSet.angle) < Reach_THRESHOLD) {	  // 累计小于门槛后，视为到位
				if (Mech.ReachCnt++ > 100) {
					Mech.ReachCnt	= 0;
					Mech.DuZhuanCnt = 0;

					FireArm.begin	= false;
					break;
				}
			} else
				Mech.ReachCnt = 0;	  // 防止 积累偶然成功

			// 检测是否堵转
			if (ABS(FireArm.ValueReal.angle - FireArm.ValuePre.angle) < DuZhuan_THRESHOLD) {
				if (Mech.DuZhuanCnt++ > 100) {
					Mech.DuZhuanCnt = 0;
					Mech.ReachCnt	= 0;
					AnswerError(IdTransmiter(FireArmId), DuZhuan);
					while (1) {
						Beep.BeepOnNum++;
						OSTimeDly(500);	   // 报错了，蜂鸣器一直响，不用停
					}
				}
			} else
				Mech.DuZhuanCnt = 0;	// 防止 偶然积累
		}
	}

	CanBack(rx_message);	// 以下是反馈报文
}

void BallPaw(CanRxMsg rx_message)
{
	if (rx_message.Data[1])	   // 1右取球
	{						   // PawCatch(&BallPaw2);

		BallPaw2.ValueSet.angle = Mech.BallPawAngleSecure;
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);

		FireUpDown.ValueSet.angle	= Mech.FireUpDownAngleDown;	   // 俯仰角最低以接球
		FireSteering.ValueSet.angle = BallPaw2.SteeringAngle;	   // 回转角旋转
		FireSteering.begin			= true;
		FireUpDown.begin			= true;
		DJ_Check_place(&FireSteering);	  // 判断到位
		DJ_Check_place(&FireUpDown);	  // 判断到位

		BallPaw2.ValueSet.angle = Mech.BallPawAngleDown;
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);	  // 判断到位

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Close));	// 夹
		OSTimeDly(5000);

		BallPaw2.ValueSet.angle = Mech.BallPawAngleUp;	  // 放球
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Open));	   // 放开
		OSTimeDly(10000);

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Close));	// 夹紧
		OSTimeDly(5000);

		BallPaw2.ValueSet.angle = Mech.BallPawAngleSecure;	  // 回到安全位置
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Open));	   // 放开
		OSTimeDly(10000);

	} else	  // 0左取球
	{
		BallPaw1.ValueSet.angle = Mech.BallPawAngleSecure;
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);

		FireUpDown.ValueSet.angle	= Mech.FireUpDownAngleDown;	   // 俯仰角最低以接球
		FireSteering.ValueSet.angle = BallPaw1.SteeringAngle;	   // 回转角旋转
		FireSteering.begin			= true;
		FireUpDown.begin			= true;
		DJ_Check_place(&FireSteering);	  // 判断到位
		DJ_Check_place(&FireUpDown);	  // 判断到位

		BallPaw1.ValueSet.angle = Mech.BallPawAngleDown;
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);	  // 判断到位

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Close));	// 夹
		OSTimeDly(5000);

		BallPaw1.ValueSet.angle = Mech.BallPawAngleUp;	  // 放球
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Open));	   // 放开
		OSTimeDly(10000);

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Close));	// 紧
		OSTimeDly(5000);

		BallPaw1.ValueSet.angle = Mech.BallPawAngleSecure;	  // 回到安全位置
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Open));	   // 放开
		OSTimeDly(10000);
	}

	CanBack(rx_message);	// 以下是反馈报文
}

// void PawCatch(DJMOTORpointer Paw)	 // 夹球，并拿上来
// {
// 	// Paw.begin					= false;
// 	// FireSteering.begin			= false;
// 	// FireSteering.mode			= DJ_POSITION;
// 	Paw->ValueSet.angle = Mech.BallPawAngleSecure;
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);

// 	FireUpDown.ValueSet.angle	= Mech.FireUpDownAngleDown;	   // 俯仰角最低以接球
// 	FireSteering.ValueSet.angle = Paw->SteeringAngle;		   // 回转角旋转
// 	FireSteering.begin			= true;
// 	FireUpDown.begin			= true;
// 	DJ_Check_place(&FireSteering);	  // 判断到位
// 	DJ_Check_place(&FireUpDown);	  // 判断到位

// 	Paw->ValueSet.angle = Mech.BallPawAngleDown;
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);	// 判断到位

// 	Valve_Ctrl(CylinderCalculate(Paw->Cylinder, Close));	// 夹
// 	OSTimeDly(5000);

// 	Paw->ValueSet.angle = Mech.BallPawAngleUp;	  // 放球
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);

// 	Valve_Ctrl(CylinderCalculate(Paw->Cylinder, Open));	   // 放开
// 	OSTimeDly(5000);

// 	Paw->ValueSet.angle = Mech.BallPawAngleSecure;	  // 回到安全位置
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);
// }

// void PawDown(DJMOTOR Paw)	 // 爪子放下去
// {
// 	 Paw.ValueSet.angle = Mech.BallPawAngleDown;
// 	 Paw.begin		   = true;

// 	  DJ_Check_place(Paw);	// 到位判断
// }

void Seeding(CanRxMsg rx_message)	 // 注意：要形成闭环
{
	if (rx_message.Data[1])	   // 拿秧苗
	{
		if (Mech.SeedTimes == 0) {	  // 第一次拿。拿完之后不松手，如果需要直接放，省时间。

			SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // 伸手拿
			SeedPaw.begin		   = true;

			DJ_Check_place(&SeedPaw);	 // 到位判断

			Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Close));	   // 两手抓
			Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Close));

			SeedStorage.ValueSet.angle = Mech.SeedStorageAngle;	   // 存苗机构 转动调整
			SeedStorage.begin		   = true;
			// OSTimeDly(20000);	 // 一起延时

			CanBack(rx_message);	// 按要求此时反馈原报文

			DJ_Check_place(&SeedStorage);	 // 到位判断

			SeedPaw.ValueSet.angle = Mech.SeedPutAngle;	   //  放上去而储存
			SeedPaw.begin		   = true;
			DJ_Check_place(&SeedPaw);	 // 到位判断

			Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Open));	  // 松开
			Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Open));
			OSTimeDly(5000);

			SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // 返回重新取
			SeedPaw.begin		   = true;
			DJ_Check_place(&SeedPaw);	 // 到位判断

			SeedStorage.ValueSet.angle = Mech.SeedStorageGoAngle;	 // 存苗机构 转动调整
			SeedStorage.begin		   = true;
			DJ_Check_place(&SeedStorage);	 // 到位判断

			Mech.SeedTimes = 1;	   // 已经拿过 1 次
		} else if (Mech.SeedTimes == 1) {
			Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Close));	   // 二次抓取
			Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Close));
			OSTimeDly(5000);

			SeedPaw.ValueSet.angle = Mech.SeedPutAngle;	   // 抬起来
			SeedPaw.begin		   = true;
			DJ_Check_place(&SeedPaw);	 // 到位判断

			CanBack(rx_message);	// 按要求此时反馈原报文

			Mech.SeedTimes = 2;
		}

	} else {						// 放秧苗
		if (Mech.SeedTimes == 2)	// 已经拿过 2 次，放的方式
		{
			if (Mech.FirstPut == 1)	   // 再放左
			{
				Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Open));	  // 左边松开
				OSTimeDly(10000);

				CanBack(rx_message);	// 按要求此时反馈原报文

				SeedPaw.ValueSet.angle = Mech.SeedPutAngle;	   //  转上去而拿取
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Close));	   // 夹紧
				Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Close));
				OSTimeDly(20000);

				SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Mech.FirstPut  = 0;
				Mech.SeedTimes = 1;	   // 此时变成拿过 1 次的状态}

			} else if (Mech.FirstPut == 0) {					 // 先放右秧
				SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // 转到取秧角度
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Open));	  // 右边松开
				OSTimeDly(10000);
				Mech.FirstPut = 1;	  // 右边已经放过

				CanBack(rx_message);	// 按要求此时反馈原报文

				SeedStorage.ValueSet.angle = Mech.SeedPutAngle;	   // 调整存秧机构角度
				SeedStorage.begin		   = true;
				DJ_Check_place(&SeedStorage);
			}

		} else if (Mech.SeedTimes == 1) {	 // 拿过 1 次的放苗方式
			if (Mech.FirstPut == 0)			 // 先放右秧
			{
				SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Open));	  // 右边松开
				OSTimeDly(10000);
				Mech.FirstPut = 1;	  // 右边已经放过

				CanBack(rx_message);	// 按要求此时反馈原报文
			} else {
				Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Open));	  // 松开
				OSTimeDly(10000);

				CanBack(rx_message);	// 按要求此时反馈原报文

				Mech.FirstPut  = 0;
				Mech.SeedTimes = 0;	   // 此时变成拿过 0 次的状态
			}
		}
	}
}
void YouBack(CanRxMsg rx_message)
{
	// Data[?]待定，记得检查修改
	if (rx_message.Data[1] & 1)	   // 取最后一位，seeding
	{
		SeedStorage.begin		   = false;
		SeedStorage.begin		   = false;

		SeedPaw.mode			   = DJ_POSITION;
		SeedStorage.mode		   = DJ_POSITION;

		SeedPaw.ValueSet.angle	   = 0;
		SeedStorage.ValueSet.angle = 0;

		SeedStorage.begin		   = true;
		SeedStorage.begin		   = true;
		OSTimeDly(5000);
	}

	if (rx_message.Data[1] >> 1 & 1)	// 取倒数第二位，ball
	{
		BallPaw1.begin			= false;
		BallPaw2.begin			= false;

		BallPaw1.mode			= DJ_POSITION;
		BallPaw2.mode			= DJ_POSITION;

		BallPaw1.ValueSet.angle = 0;
		BallPaw2.ValueSet.angle = 0;

		BallPaw1.begin			= true;
		BallPaw2.begin			= true;
		OSTimeDly(5000);
	}

	if (rx_message.Data[1] >> 2 & 1)	// 取倒数第三位，fire
	{
		FireArm.begin				= false;
		FireUpDown.begin			= false;
		FireSteering.begin			= false;

		FireSteering.mode			= DJ_POSITION;
		FireUpDown.mode				= DJ_POSITION;
		FireArm.mode				= Zdrive_Position;

		FireSteering.ValueSet.angle = 0;
		FireUpDown.ValueSet.angle	= 0;
		FireArm.ValueSet.angle		= 0;

		FireArm.begin				= true;
		FireUpDown.begin			= true;
		FireSteering.begin			= true;
		OSTimeDly(5000);
	}

	CanBack(rx_message);
}

// 未知其他气缸之状态，而单独开关一个气缸
uint8_t CylinderCalculate(int Paw_, int command)
{
	switch (command) {
		case Close:
			CylinderControl = (1 << (Paw_ - 1)) | CylinderControl;	  // 此位 置1，其他不变
			return CylinderControl;

		case Open:
			CylinderControl = !(1 << (Paw_ - 1)) & CylinderControl;	   // 此位 置0，其他不变
			return CylinderControl;
	}
	return 0;
}

// 检测大疆电机位置模式是否到位
void DJ_Check_place(DJMOTORpointer DJ)
{
	if (DJ->mode == DJ_POSITION) {
		while (1) {
			// 检测是否到位
			if (ABS(DJ->ValueNow.angle - DJ->ValueSet.angle) < Reach_THRESHOLD) {	 // 累计小于门槛后，视为到位
				if (Mech.ReachCnt++ > 999999) {
					Mech.DuZhuanCnt = 0;
					Mech.ReachCnt	= 0;
					DJ->begin		= false;	// 电机关闭
					break;
				}
			} else
				Mech.ReachCnt = 0;	  // 防止 积累偶然成功

			//			// 检测是否堵转
			//			if (ABS(DJ->ValueNow.angle - DJ->ValuePre.angle) < DuZhuan_THRESHOLD) {
			//				if (Mech.DuZhuanCnt++ > 60000) {
			//					Mech.DuZhuanCnt = 0;
			//					Mech.ReachCnt	= 0;
			//					AnswerError(IdTransmiter(DJ->id), DuZhuan);
			//					while (1) {
			//						Beep.BeepOnNum++;
			//						OSTimeDly(500);	   // 报错了，蜂鸣器一直响，不用停
			//					}
			//				}
			//			} else
			//				Mech.DuZhuanCnt = 0;	// 防止 偶然积累
		}
	}
}

void CanBack(CanRxMsg rx_message)	 // 反馈原报文
{
	CanTxMsg message;

	message.IDE	  = CAN_Id_Extended;
	message.ExtId = 0x05020100 + (rx_message.ExtId & 0x0000000f);
	message.DLC	  = 2;
	message.RTR	  = CAN_RTR_Data;

	if (message.ExtId == 0x05020107) {	  // 此ID需要回馈 OK
		message.Data[0] = 'O';
		message.Data[1] = 'K';
	} else {	// 其余ID反馈原报文
		message.Data[0] = rx_message.Data[0];
		message.Data[1] = rx_message.Data[1];
	}

	CAN_Transmit(CAN1, &message);
}

void Can1Funtion(CanRxMsg rx_message)
{
	RX_message = rx_message;	// 传送消息

	// 根据不同的协议ID执行不同的操作
	if (rx_message.ExtId == 0x01020501 && rx_message.Data[0] == 'M')	// 使能/失能
		ChooseFlag = 1;

	if (rx_message.ExtId == 0x01020502 && rx_message.Data[0] == 'Z')	// 寻零
		ChooseFlag = 2;

	if (rx_message.ExtId == 0x01020503)	   // 设置发射俯仰角/回转角/速度
		ChooseFlag = 3;

	if (rx_message.ExtId == 0x01020504 && rx_message.Data[0] == 'F')	// 定点发射
		ChooseFlag = 4;

	if (rx_message.ExtId == 0x01020505 && rx_message.Data[0] == 'B')	// 左右取球夹爪动作
		ChooseFlag = 5;

	if (rx_message.ExtId == 0x01020506 && rx_message.Data[0] == 'S')	// 取秧/放秧动作
	{
		ChooseFlag = 6;
		Cnt6++;
	}

	if (rx_message.ExtId == 0x01020507 && rx_message.Data[0] == 'I' && rx_message.Data[1] == 'P')	 // 回到初始位置
		ChooseFlag = 7;

	if (rx_message.ExtId == 0x010205FF && rx_message.Data[0] == 'R' && rx_message.Data[1] == 'S')	 // 板子复位
	{
		__set_FAULTMASK(1);	   // 关闭所有的中断，确保执行复位时不被中断打断
		NVIC_SystemReset();	   // 系统软件复位，配置好的外设寄存器也一起复位
	}
}
