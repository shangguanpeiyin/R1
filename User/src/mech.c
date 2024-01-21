#include "mech.h"

FirePlace Here[13];	   // 12�����㷢��� �Ĳ�����0Ϊdebug�·���
MechDef	  Mech;

uint8_t CylinderControl;

CanRxMsg		 RX_message;	// Can1Funtion() �� R1_Task() ���õı���
volatile uint8_t ChooseFlag;
int				 Cnt6;

uint8_t SeedPaw1Cylinder;	 // ��ȡ�����׵�λ�������ڼ�λ��
uint8_t SeedPaw2Cylinder;

void MechInit()
{
	Cnt6			= 0;

	CylinderControl = 0;
	ChooseFlag		= 0;

	Mech.SeedTimes	= 0;	// �Ѿ��ù�����Ĵ�������һ������Ҫ���棬�ڶ��β���Ҫ�������õ����Ρ�
	Mech.ReachCnt	= 0;	// ����Ƿ�λ�ļ�����
	Mech.DuZhuanCnt = 0;	// ����Ƿ��ת�ļ�����
	Mech.FirstPut	= 0;
	// ���ﵱȻ����0����������Ƕ��ٴ���
	// ���ֲ�������
	Mech.BallPawAngleUp			   = 496;	 // ��ĽǶȣ�Ҫ̧���٣�
	Mech.BallPawAngleDown		   = 113;	 // ץȡ��ĽǶȣ�ץ���ʱ��ĽǶȣ�����������
	Mech.BallPawAngleSecure		   = 316.8;
	Mech.SeedCatchAngle			   = -21;	 // ȡ��ĽǶ�
	Mech.SeedPutAngle			   = -81;	 // ����ĽǶ�
	Mech.SeedStorageAngle		   = 71;	 // �����������ĽǶ�
	Mech.SeedStorageGoAngle		   = 36;	 // ��Ż�����ȫ�Ƕ�
	Mech.BallPawtoZeroAngle		   = 0;

	Mech.FireAngleMax			   = 0;
	Mech.FireOriginal			   = 0;
	Mech.FireUpDownAngleDown	   = 0;	   // ���������

	BallPaw1.SteeringAngle		   = 447.79;	 // ���ȡ���ת��
	BallPaw2.SteeringAngle		   = -451.76;	 // �ұ�ȡ���ת��

	// ��ʼ��PVTģʽ�Ĳ���
	// ����
	FireArm.PVTMode.ReachTime[0]   = 0;
	FireArm.PVTMode.TargetSpeed[0] = 0;
	FireArm.PVTMode.TargetAngle[0] = 0;

	FireArm.PVTMode.ReachTime[1]   = 0;
	FireArm.PVTMode.TargetAngle[1] = 0;

	FireArm.PVTMode.ReachTime[2]   = 0;
	FireArm.PVTMode.TargetAngle[2] = 0;
	FireArm.PVTMode.TargetSpeed[2] = 0;	   // ������0�����ô�

	{	 // ��ʼ��12������㣬��������
		// ����
		Here[0].FireSteeringAngle  = 0;	   // 0Ϊdebug�·���
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
	// ��ʼ����������ף��˵�������׵Ŀ���λ�ǵ����ڼ�λ��
	// ������
	BallPaw1.Cylinder			   = 6;	   // ��
	BallPaw2.Cylinder			   = 5;
	SeedPaw1Cylinder			   = 8;
	SeedPaw2Cylinder			   = 7;

	// ��ʼ�����Ѱ���ٶȣ����ɶ���һ���ٶȣ���ΪѰ�㷽����ܲ�һ��
	SeedPaw.Limit.ZeroSpeed		   = 500;	   // ��
	SeedStorage.Limit.ZeroSpeed	   = -1500;	   // ��
	BallPaw1.Limit.ZeroSpeed	   = 500;
	BallPaw2.Limit.ZeroSpeed	   = 500;

	// PVT�������
	// ���꣬�����Ҫ���Լ����
	FireArm.PVTMode.ReachTime[0]   = 0;	   // ���ٽ׶�
	FireArm.PVTMode.TargetAngle[0] = 0;
	FireArm.PVTMode.TargetSpeed[0] = 0;
	FireArm.PVTMode.TargetSpeed[1] = 0;

	FireArm.PVTMode.ReachTime[1]   = 0;	   // ���ٽ׶�
	FireArm.PVTMode.TargetAngle[1] = 0;

	FireArm.PVTMode.ReachTime[2]   = 0;	   // ���ٽ׶�
	FireArm.PVTMode.TargetAngle[2] = 0;
	FireArm.PVTMode.TargetSpeed[2] = 0;	   // 0 ���ø�

	// ����λ��ģʽ����ʼ��ֱ�����ã������ٴ��޸���
	SeedPaw.mode				   = DJ_POSITION;
	SeedStorage.mode			   = DJ_POSITION;
	BallPaw1.mode				   = DJ_POSITION;
	BallPaw2.mode				   = DJ_POSITION;
	FireSteering.mode			   = DJ_POSITION;
	FireUpDown.mode				   = DJ_POSITION;
	FireArm.mode				   = Zdrive_Position;
}

// ʹ�ܡ�ʧ��
void XXable(CanRxMsg rx_message)
{
	if (rx_message.Data[1] & 1)	   // ȡ���һλ��seeding
	{
		SeedPaw.enable	   = true;
		SeedStorage.enable = true;
	} else {
		SeedPaw.enable	   = false;
		SeedStorage.enable = false;
	}

	if (rx_message.Data[1] >> 1 & 1)	// ȡ�����ڶ�λ��ball
	{
		BallPaw1.enable = true;
		BallPaw2.enable = true;
	} else {
		BallPaw1.enable = false;
		BallPaw2.enable = false;
	}

	if (rx_message.Data[1] >> 2 & 1)	// ȡ��������λ��fire
	{
		FireSteering.enable = true;
		FireUpDown.enable	= true;
		FireArm.enable		= true;
	} else {
		FireSteering.enable = false;
		FireUpDown.enable	= false;
		FireArm.enable		= false;
	}

	CanBack(rx_message);	// �����Ƿ�������
}

// Ѱ��
void FindZero(CanRxMsg rx_message)
{
	if (rx_message.Data[1] & 1)	   // ȡ���һλ��seeding
	{
		SeedPaw.enable	   = true;
		SeedStorage.enable = true;

		SeedPaw.begin	   = false;
		SeedStorage.begin  = false;

		SeedPaw.mode	   = DJ_ZERO;
		SeedStorage.mode   = DJ_ZERO;

		SeedPaw.begin	   = true;
		SeedStorage.begin  = true;	  // Ѱ��ɹ����Զ�false
		while (1) {
			if (SeedPaw.begin == false && SeedStorage.begin == false)
				break;
		}
		SeedPaw.mode		   = DJ_POSITION;
		SeedStorage.mode	   = DJ_POSITION;
		// SeedPaw.enable	   = false;
		// SeedStorage.enable = false;

		SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // ������
		SeedPaw.begin		   = true;

		DJ_Check_place(&SeedPaw);	 // ��λ�ж�

	}	 // 0��ʾ��Ѱ�㣬û��ָ�����

	if (rx_message.Data[1] >> 1 & 1)	// ȡ�����ڶ�λ��ball
	{
		BallPaw1.begin = false;
		BallPaw2.begin = false;

		BallPaw1.mode  = DJ_ZERO;
		BallPaw2.mode  = DJ_ZERO;

		BallPaw1.begin = true;
		BallPaw2.begin = true;	  // Ѱ��ɹ����Զ�false
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
		BallPaw2.begin			= true;	   // Ѱ��ɹ����Զ�false
		DJ_Check_place(&BallPaw1);
		DJ_Check_place(&BallPaw2);	  // Ѱ��֮��צ������ȥ���˷���fire��Ѱ�㡣ע�ⰲȫ��צ�ӣ���

		BallPaw1.begin				  = false;
		BallPaw2.begin				  = false;
		BallPaw1.StatusFlag.isSetZero = 1;
		BallPaw2.StatusFlag.isSetZero = 1;
	}

	if (rx_message.Data[1] >> 2 & 1)	// ȡ��������λ��fire
	{
		FireArm.begin					  = false;
		FireUpDown.begin				  = false;
		FireSteering.begin				  = false;

		FireSteering.StatusFlag.isSetZero = 1;	  // ���õ�ǰλ��Ϊ0
		FireUpDown.StatusFlag.isSetZero	  = 1;
		Zdrive_SetZero(FireArmId);	  // ���õ�ǰλ��Ϊ0

		// FireArm.enable		= false;
		// FireUpDown.enable	= false;
		// FireSteering.enable = false;

		FireArm.mode	  = Zdrive_Position;
		FireUpDown.mode	  = DJ_POSITION;
		FireSteering.mode = DJ_POSITION;
	}

	CanBack(rx_message);	// �����Ƿ�������
}

void SetParameters(CanRxMsg rx_message)
{
	Here[0].FireUpDownAngle	  = (rx_message.Data[0] << 8 | rx_message.Data[1]) / 10.0;
	Here[0].FireSteeringAngle = (rx_message.Data[2] << 8 | rx_message.Data[3]) / 10.0;
	Here[0].FireArmSpeed	  = (rx_message.Data[4] << 8 | rx_message.Data[5]);

	CanBack(rx_message);	// �����Ƿ�������
}

void FireInHere(CanRxMsg rx_message)	// �ջ�
{
	if (rx_message.Data[1] > 0) {
		// ��ת�븩��      1-12Ϊ12�����㷢���
		FireSteering.ValueSet.angle = Here[rx_message.Data[1]].FireSteeringAngle;	 // Here[1-12]
		FireUpDown.ValueSet.angle	= Here[rx_message.Data[1]].FireUpDownAngle;

		FireSteering.begin			= true;
		FireUpDown.begin			= true;

		// ����ת������λ
		DJ_Check_place(&FireUpDown);
		DJ_Check_place(&FireSteering);

		FireArm.ValueSet.angle = Mech.FireOriginal;
		FireArm.begin		   = true;
		OSTimeDly(5000);

		// Arm��PVTģʽ����
		FireArm.PVTMode.TargetSpeed[1] = Here[rx_message.Data[1]].FireArmSpeed;

		FireArm.PVTMode.PVTModeFlag	   = true;	  // ����PVTģʽ
		FireArm.begin				   = true;

	} else if (rx_message.Data[1] == 0) {	 // debug����ģʽ
		// ��ת�븩��      1-12Ϊ12�����㷢���
		FireSteering.ValueSet.angle = Here[0].FireSteeringAngle;	// Here[1-12]
		FireUpDown.ValueSet.angle	= Here[0].FireUpDownAngle;

		FireSteering.begin			= true;
		FireUpDown.begin			= true;

		// ����ת������λ
		DJ_Check_place(&FireUpDown);
		DJ_Check_place(&FireSteering);

		FireArm.ValueSet.angle = Mech.FireOriginal;
		FireArm.begin		   = true;
		OSTimeDly(5000);

		// Arm��PVTģʽ����
		FireArm.PVTMode.TargetSpeed[1] = Here[0].FireArmSpeed;

		FireArm.PVTMode.PVTModeFlag	   = true;	  // ����PVTģʽ
		FireArm.begin				   = true;
	}

	OSTimeDly(5000);
	// ���Arm��λ��Zdrive��
	if (FireArm.mode == Zdrive_Position) {
		while (1) {
			// ����Ƿ�λ
			if (ABS(FireArm.ValueReal.angle - FireArm.PVTMode.TargetAngle[2]) < Reach_THRESHOLD) {	  // �ۼ�С���ż�����Ϊ��λ
				if (Mech.ReachCnt++ > 100) {
					Mech.ReachCnt	= 0;
					Mech.DuZhuanCnt = 0;

					FireArm.begin	= false;
					break;
				}
			} else
				Mech.ReachCnt = 0;	  // ��ֹ ����żȻ�ɹ�

			// ����Ƿ��ת
			if (ABS(FireArm.ValueReal.angle - FireArm.ValuePre.angle) < DuZhuan_THRESHOLD) {
				if (Mech.DuZhuanCnt++ > 100) {
					Mech.DuZhuanCnt = 0;
					Mech.ReachCnt	= 0;
					AnswerError(IdTransmiter(FireArmId), DuZhuan);
					while (1) {
						Beep.BeepOnNum++;
						OSTimeDly(500);	   // �����ˣ�������һֱ�죬����ͣ
					}
				}
			} else
				Mech.DuZhuanCnt = 0;	// ��ֹ żȻ����
		}
	}

	// ����ۻ�λ
	FireArm.ValueSet.angle = 0;	   // �ص���㣬��ֹ��������������
	FireArm.begin		   = true;

	// ���Arm��λ��Zdrive��
	if (FireArm.mode == Zdrive_Position) {
		while (1) {
			// ����Ƿ�λ
			if (ABS(FireArm.ValueReal.angle - FireArm.ValueSet.angle) < Reach_THRESHOLD) {	  // �ۼ�С���ż�����Ϊ��λ
				if (Mech.ReachCnt++ > 100) {
					Mech.ReachCnt	= 0;
					Mech.DuZhuanCnt = 0;

					FireArm.begin	= false;
					break;
				}
			} else
				Mech.ReachCnt = 0;	  // ��ֹ ����żȻ�ɹ�

			// ����Ƿ��ת
			if (ABS(FireArm.ValueReal.angle - FireArm.ValuePre.angle) < DuZhuan_THRESHOLD) {
				if (Mech.DuZhuanCnt++ > 100) {
					Mech.DuZhuanCnt = 0;
					Mech.ReachCnt	= 0;
					AnswerError(IdTransmiter(FireArmId), DuZhuan);
					while (1) {
						Beep.BeepOnNum++;
						OSTimeDly(500);	   // �����ˣ�������һֱ�죬����ͣ
					}
				}
			} else
				Mech.DuZhuanCnt = 0;	// ��ֹ żȻ����
		}
	}

	CanBack(rx_message);	// �����Ƿ�������
}

void BallPaw(CanRxMsg rx_message)
{
	if (rx_message.Data[1])	   // 1��ȡ��
	{						   // PawCatch(&BallPaw2);

		BallPaw2.ValueSet.angle = Mech.BallPawAngleSecure;
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);

		FireUpDown.ValueSet.angle	= Mech.FireUpDownAngleDown;	   // ����������Խ���
		FireSteering.ValueSet.angle = BallPaw2.SteeringAngle;	   // ��ת����ת
		FireSteering.begin			= true;
		FireUpDown.begin			= true;
		DJ_Check_place(&FireSteering);	  // �жϵ�λ
		DJ_Check_place(&FireUpDown);	  // �жϵ�λ

		BallPaw2.ValueSet.angle = Mech.BallPawAngleDown;
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);	  // �жϵ�λ

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Close));	// ��
		OSTimeDly(5000);

		BallPaw2.ValueSet.angle = Mech.BallPawAngleUp;	  // ����
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Open));	   // �ſ�
		OSTimeDly(10000);

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Close));	// �н�
		OSTimeDly(5000);

		BallPaw2.ValueSet.angle = Mech.BallPawAngleSecure;	  // �ص���ȫλ��
		BallPaw2.begin			= true;
		DJ_Check_place(&BallPaw2);

		Valve_Ctrl(CylinderCalculate(BallPaw2.Cylinder, Open));	   // �ſ�
		OSTimeDly(10000);

	} else	  // 0��ȡ��
	{
		BallPaw1.ValueSet.angle = Mech.BallPawAngleSecure;
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);

		FireUpDown.ValueSet.angle	= Mech.FireUpDownAngleDown;	   // ����������Խ���
		FireSteering.ValueSet.angle = BallPaw1.SteeringAngle;	   // ��ת����ת
		FireSteering.begin			= true;
		FireUpDown.begin			= true;
		DJ_Check_place(&FireSteering);	  // �жϵ�λ
		DJ_Check_place(&FireUpDown);	  // �жϵ�λ

		BallPaw1.ValueSet.angle = Mech.BallPawAngleDown;
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);	  // �жϵ�λ

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Close));	// ��
		OSTimeDly(5000);

		BallPaw1.ValueSet.angle = Mech.BallPawAngleUp;	  // ����
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Open));	   // �ſ�
		OSTimeDly(10000);

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Close));	// ��
		OSTimeDly(5000);

		BallPaw1.ValueSet.angle = Mech.BallPawAngleSecure;	  // �ص���ȫλ��
		BallPaw1.begin			= true;
		DJ_Check_place(&BallPaw1);

		Valve_Ctrl(CylinderCalculate(BallPaw1.Cylinder, Open));	   // �ſ�
		OSTimeDly(10000);
	}

	CanBack(rx_message);	// �����Ƿ�������
}

// void PawCatch(DJMOTORpointer Paw)	 // ���򣬲�������
// {
// 	// Paw.begin					= false;
// 	// FireSteering.begin			= false;
// 	// FireSteering.mode			= DJ_POSITION;
// 	Paw->ValueSet.angle = Mech.BallPawAngleSecure;
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);

// 	FireUpDown.ValueSet.angle	= Mech.FireUpDownAngleDown;	   // ����������Խ���
// 	FireSteering.ValueSet.angle = Paw->SteeringAngle;		   // ��ת����ת
// 	FireSteering.begin			= true;
// 	FireUpDown.begin			= true;
// 	DJ_Check_place(&FireSteering);	  // �жϵ�λ
// 	DJ_Check_place(&FireUpDown);	  // �жϵ�λ

// 	Paw->ValueSet.angle = Mech.BallPawAngleDown;
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);	// �жϵ�λ

// 	Valve_Ctrl(CylinderCalculate(Paw->Cylinder, Close));	// ��
// 	OSTimeDly(5000);

// 	Paw->ValueSet.angle = Mech.BallPawAngleUp;	  // ����
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);

// 	Valve_Ctrl(CylinderCalculate(Paw->Cylinder, Open));	   // �ſ�
// 	OSTimeDly(5000);

// 	Paw->ValueSet.angle = Mech.BallPawAngleSecure;	  // �ص���ȫλ��
// 	Paw->begin			= true;
// 	DJ_Check_place(Paw);
// }

// void PawDown(DJMOTOR Paw)	 // צ�ӷ���ȥ
// {
// 	 Paw.ValueSet.angle = Mech.BallPawAngleDown;
// 	 Paw.begin		   = true;

// 	  DJ_Check_place(Paw);	// ��λ�ж�
// }

void Seeding(CanRxMsg rx_message)	 // ע�⣺Ҫ�γɱջ�
{
	if (rx_message.Data[1])	   // ������
	{
		if (Mech.SeedTimes == 0) {	  // ��һ���á�����֮�����֣������Ҫֱ�ӷţ�ʡʱ�䡣

			SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // ������
			SeedPaw.begin		   = true;

			DJ_Check_place(&SeedPaw);	 // ��λ�ж�

			Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Close));	   // ����ץ
			Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Close));

			SeedStorage.ValueSet.angle = Mech.SeedStorageAngle;	   // ������� ת������
			SeedStorage.begin		   = true;
			// OSTimeDly(20000);	 // һ����ʱ

			CanBack(rx_message);	// ��Ҫ���ʱ����ԭ����

			DJ_Check_place(&SeedStorage);	 // ��λ�ж�

			SeedPaw.ValueSet.angle = Mech.SeedPutAngle;	   //  ����ȥ������
			SeedPaw.begin		   = true;
			DJ_Check_place(&SeedPaw);	 // ��λ�ж�

			Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Open));	  // �ɿ�
			Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Open));
			OSTimeDly(5000);

			SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // ��������ȡ
			SeedPaw.begin		   = true;
			DJ_Check_place(&SeedPaw);	 // ��λ�ж�

			SeedStorage.ValueSet.angle = Mech.SeedStorageGoAngle;	 // ������� ת������
			SeedStorage.begin		   = true;
			DJ_Check_place(&SeedStorage);	 // ��λ�ж�

			Mech.SeedTimes = 1;	   // �Ѿ��ù� 1 ��
		} else if (Mech.SeedTimes == 1) {
			Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Close));	   // ����ץȡ
			Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Close));
			OSTimeDly(5000);

			SeedPaw.ValueSet.angle = Mech.SeedPutAngle;	   // ̧����
			SeedPaw.begin		   = true;
			DJ_Check_place(&SeedPaw);	 // ��λ�ж�

			CanBack(rx_message);	// ��Ҫ���ʱ����ԭ����

			Mech.SeedTimes = 2;
		}

	} else {						// ������
		if (Mech.SeedTimes == 2)	// �Ѿ��ù� 2 �Σ��ŵķ�ʽ
		{
			if (Mech.FirstPut == 1)	   // �ٷ���
			{
				Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Open));	  // ����ɿ�
				OSTimeDly(10000);

				CanBack(rx_message);	// ��Ҫ���ʱ����ԭ����

				SeedPaw.ValueSet.angle = Mech.SeedPutAngle;	   //  ת��ȥ����ȡ
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Close));	   // �н�
				Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Close));
				OSTimeDly(20000);

				SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Mech.FirstPut  = 0;
				Mech.SeedTimes = 1;	   // ��ʱ����ù� 1 �ε�״̬}

			} else if (Mech.FirstPut == 0) {					 // �ȷ�����
				SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;	 // ת��ȡ��Ƕ�
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Open));	  // �ұ��ɿ�
				OSTimeDly(10000);
				Mech.FirstPut = 1;	  // �ұ��Ѿ��Ź�

				CanBack(rx_message);	// ��Ҫ���ʱ����ԭ����

				SeedStorage.ValueSet.angle = Mech.SeedPutAngle;	   // ������������Ƕ�
				SeedStorage.begin		   = true;
				DJ_Check_place(&SeedStorage);
			}

		} else if (Mech.SeedTimes == 1) {	 // �ù� 1 �εķ��緽ʽ
			if (Mech.FirstPut == 0)			 // �ȷ�����
			{
				SeedPaw.ValueSet.angle = Mech.SeedCatchAngle;
				SeedPaw.begin		   = true;
				DJ_Check_place(&SeedPaw);

				Valve_Ctrl(CylinderCalculate(SeedPaw2Cylinder, Open));	  // �ұ��ɿ�
				OSTimeDly(10000);
				Mech.FirstPut = 1;	  // �ұ��Ѿ��Ź�

				CanBack(rx_message);	// ��Ҫ���ʱ����ԭ����
			} else {
				Valve_Ctrl(CylinderCalculate(SeedPaw1Cylinder, Open));	  // �ɿ�
				OSTimeDly(10000);

				CanBack(rx_message);	// ��Ҫ���ʱ����ԭ����

				Mech.FirstPut  = 0;
				Mech.SeedTimes = 0;	   // ��ʱ����ù� 0 �ε�״̬
			}
		}
	}
}
void YouBack(CanRxMsg rx_message)
{
	// Data[?]�������ǵü���޸�
	if (rx_message.Data[1] & 1)	   // ȡ���һλ��seeding
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

	if (rx_message.Data[1] >> 1 & 1)	// ȡ�����ڶ�λ��ball
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

	if (rx_message.Data[1] >> 2 & 1)	// ȡ��������λ��fire
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

// δ֪��������֮״̬������������һ������
uint8_t CylinderCalculate(int Paw_, int command)
{
	switch (command) {
		case Close:
			CylinderControl = (1 << (Paw_ - 1)) | CylinderControl;	  // ��λ ��1����������
			return CylinderControl;

		case Open:
			CylinderControl = !(1 << (Paw_ - 1)) & CylinderControl;	   // ��λ ��0����������
			return CylinderControl;
	}
	return 0;
}

// ���󽮵��λ��ģʽ�Ƿ�λ
void DJ_Check_place(DJMOTORpointer DJ)
{
	if (DJ->mode == DJ_POSITION) {
		while (1) {
			// ����Ƿ�λ
			if (ABS(DJ->ValueNow.angle - DJ->ValueSet.angle) < Reach_THRESHOLD) {	 // �ۼ�С���ż�����Ϊ��λ
				if (Mech.ReachCnt++ > 999999) {
					Mech.DuZhuanCnt = 0;
					Mech.ReachCnt	= 0;
					DJ->begin		= false;	// ����ر�
					break;
				}
			} else
				Mech.ReachCnt = 0;	  // ��ֹ ����żȻ�ɹ�

			//			// ����Ƿ��ת
			//			if (ABS(DJ->ValueNow.angle - DJ->ValuePre.angle) < DuZhuan_THRESHOLD) {
			//				if (Mech.DuZhuanCnt++ > 60000) {
			//					Mech.DuZhuanCnt = 0;
			//					Mech.ReachCnt	= 0;
			//					AnswerError(IdTransmiter(DJ->id), DuZhuan);
			//					while (1) {
			//						Beep.BeepOnNum++;
			//						OSTimeDly(500);	   // �����ˣ�������һֱ�죬����ͣ
			//					}
			//				}
			//			} else
			//				Mech.DuZhuanCnt = 0;	// ��ֹ żȻ����
		}
	}
}

void CanBack(CanRxMsg rx_message)	 // ����ԭ����
{
	CanTxMsg message;

	message.IDE	  = CAN_Id_Extended;
	message.ExtId = 0x05020100 + (rx_message.ExtId & 0x0000000f);
	message.DLC	  = 2;
	message.RTR	  = CAN_RTR_Data;

	if (message.ExtId == 0x05020107) {	  // ��ID��Ҫ���� OK
		message.Data[0] = 'O';
		message.Data[1] = 'K';
	} else {	// ����ID����ԭ����
		message.Data[0] = rx_message.Data[0];
		message.Data[1] = rx_message.Data[1];
	}

	CAN_Transmit(CAN1, &message);
}

void Can1Funtion(CanRxMsg rx_message)
{
	RX_message = rx_message;	// ������Ϣ

	// ���ݲ�ͬ��Э��IDִ�в�ͬ�Ĳ���
	if (rx_message.ExtId == 0x01020501 && rx_message.Data[0] == 'M')	// ʹ��/ʧ��
		ChooseFlag = 1;

	if (rx_message.ExtId == 0x01020502 && rx_message.Data[0] == 'Z')	// Ѱ��
		ChooseFlag = 2;

	if (rx_message.ExtId == 0x01020503)	   // ���÷��丩����/��ת��/�ٶ�
		ChooseFlag = 3;

	if (rx_message.ExtId == 0x01020504 && rx_message.Data[0] == 'F')	// ���㷢��
		ChooseFlag = 4;

	if (rx_message.ExtId == 0x01020505 && rx_message.Data[0] == 'B')	// ����ȡ���צ����
		ChooseFlag = 5;

	if (rx_message.ExtId == 0x01020506 && rx_message.Data[0] == 'S')	// ȡ��/������
	{
		ChooseFlag = 6;
		Cnt6++;
	}

	if (rx_message.ExtId == 0x01020507 && rx_message.Data[0] == 'I' && rx_message.Data[1] == 'P')	 // �ص���ʼλ��
		ChooseFlag = 7;

	if (rx_message.ExtId == 0x010205FF && rx_message.Data[0] == 'R' && rx_message.Data[1] == 'S')	 // ���Ӹ�λ
	{
		__set_FAULTMASK(1);	   // �ر����е��жϣ�ȷ��ִ�и�λʱ�����жϴ��
		NVIC_SystemReset();	   // ϵͳ�����λ�����úõ�����Ĵ���Ҳһ��λ
	}
}
