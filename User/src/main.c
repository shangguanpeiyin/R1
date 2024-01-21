#include "main.h"

// �����ź���

// OS_EVENT* XXXSem;
// INT8U*	  ErrorXXXSem;

extern CanRxMsg			RX_message;	   // Can1Funtion() �� R1_Task() ���õı���
extern volatile uint8_t ChooseFlag;
extern int				Cnt6;
extern uint8_t			SeedPaw1Cylinder;	 // ��ȡ�����׵�λ�������ڼ�λ��
extern uint8_t			SeedPaw2Cylinder;

int main(void)
{
	SystemInit();
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_3);
	CAN1_Init();
	CAN2_Init();
	Tim3_Init();
	Tim2_Init();

	CAN_InitSendQueue();
	param_Init();

	BEEP_Init();
	LED_Init();
	DJmotorInit();
	Zdrive_Init();

	Valve_Init();
	//	YTL_Configuration();

	MechInit();	   // ��Ҫ�޸ģ��������棩

	OSInit();
	OSTaskCreate(Start_Task, (void*) 0, (OS_STK*) &START_TASK_STK[TASK_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
}

static void Start_Task(void* pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata			 = pdata;

	OS_CPU_SysTickInit();	 // ����OSʱ��
	OS_ENTER_CRITICAL();	 // �����ٽ���

	// ��������
	OSTaskCreate(TestOrder_Task, (void*) 0, (OS_STK*) &TESTORDER_TASK_STK[TASK_STK_SIZE - 1], TESTORDER_TASK_PRIO);	   // ���� 17

	OSTaskCreate(R1_Task, (void*) 0, (OS_STK*) &R1_TASK_STK[TASK_STK_SIZE - 1], R1_TASK_PRIO);			// ���� 15
	OSTaskCreate(Beep_Task, (void*) 0, (OS_STK*) &BEEP_TASK_STK[TASK_STK_SIZE - 1], BEEP_TASK_PRIO);	// 42
	OSTaskCreate(Led_Task, (void*) 0, (OS_STK*) &LED_TASK_STK[TASK_STK_SIZE - 1], LED_TASK_PRIO);		// 43

	OSTaskSuspend(START_TASK_PRIO);	   // ������ʼ����

	OS_EXIT_CRITICAL();	   // �˳��ٽ���
	OSTimeDly(100);
}

// 17
static void TestOrder_Task(void* pdata)
{
	pdata = pdata;
	while (1) { OSTimeDly(100); }
}

bool WangWeiFlag = false;

bool diancifa;
int	 aaaaa	 = 0;
int	 bbbbb	 = 0;
int	 allll	 = 0xff;
bool KaiGuan = false;
int	 flag	 = 0;	 // �����ڼ�λ

// 15
static void R1_Task(void* pdata)
{
	pdata = pdata;
	// ���ﺬ��ȫ�ֱ������ܣ���һ�β���ִ�����֮ǰ������ִ����һ�β�������ɱ��ָ��
	while (1) {
		switch (ChooseFlag) {
			case 1:
				XXable(RX_message);		// ʹ�ܣ�ʧ��
				if (ChooseFlag == 1)	// ��������ֱ������
					ChooseFlag = 0;
				break;
			case 2:
				FindZero(RX_message);	 // Ѱ��
				if (ChooseFlag == 2)
					ChooseFlag = 0;
				break;
			case 3:
				SetParameters(RX_message);	  // ���÷��丩����/��ת��/�ٶ�
				if (ChooseFlag == 3)
					ChooseFlag = 0;
				break;
			case 4:
				FireInHere(RX_message);	   // ���㷢��
				if (ChooseFlag == 4)
					ChooseFlag = 0;
				break;
			case 5:
				BallPaw(RX_message);	// ����ȡ���צ����
				if (ChooseFlag == 5)
					ChooseFlag = 0;
				break;
			case 6:
				Seeding(RX_message);	// ȡ��/������
				Cnt6--;
				if (ChooseFlag == 6)
					if (Cnt6 == 0)	  // ����ɱ������
						ChooseFlag = 0;
				break;
			case 7:
				YouBack(RX_message);	// �ص���ʼλ��
				if (ChooseFlag == 7)
					ChooseFlag = 0;
				break;

			default:
				break;
		}
		OSTimeDly(100);

		// �������״���
		if (diancifa == 1) {
			Valve_Ctrl(CylinderCalculate(aaaaa, bbbbb));

			if (allll == 1)
				Valve_Ctrl(255);
			else if (allll == 0)
				Valve_Ctrl(0);
			else
				diancifa = 0;
			allll = 0xffff;
		}

		// �����޸�data�Ĵ���
		if (KaiGuan) {
			if (flag) {
				RX_message.Data[1] = (1 << (flag - 1)) | RX_message.Data[1];
			}	 // ��λ��1�����಻��
			else {
				RX_message.Data[1] = !(1 << (flag - 1)) & RX_message.Data[1];
			}	 // ��λ��0�����಻��
		} else
	}
}

// 42
static void Beep_Task(void* pdata)
{
	pdata = pdata;
	for (;;) {
		beeptask();
		OSTimeDly(1000);
	}
}

// 43
static void Led_Task(void* pdata)
{
	pdata = pdata;
	for (;;) {
		ledtask();
		OSTimeDly(5000);
	}
}
