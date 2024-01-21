#include "main.h"

// 定义信号量

// OS_EVENT* XXXSem;
// INT8U*	  ErrorXXXSem;

extern CanRxMsg			RX_message;	   // Can1Funtion() 和 R1_Task() 共用的变量
extern volatile uint8_t ChooseFlag;
extern int				Cnt6;
extern uint8_t			SeedPaw1Cylinder;	 // 左。取秧气缸的位（倒数第几位）
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

	MechInit();	   // 需要修改（在最下面）

	OSInit();
	OSTaskCreate(Start_Task, (void*) 0, (OS_STK*) &START_TASK_STK[TASK_STK_SIZE - 1], START_TASK_PRIO);
	OSStart();
}

static void Start_Task(void* pdata)
{
	OS_CPU_SR cpu_sr = 0;
	pdata			 = pdata;

	OS_CPU_SysTickInit();	 // 开启OS时钟
	OS_ENTER_CRITICAL();	 // 进入临界区

	// 创建任务
	OSTaskCreate(TestOrder_Task, (void*) 0, (OS_STK*) &TESTORDER_TASK_STK[TASK_STK_SIZE - 1], TESTORDER_TASK_PRIO);	   // 测试 17

	OSTaskCreate(R1_Task, (void*) 0, (OS_STK*) &R1_TASK_STK[TASK_STK_SIZE - 1], R1_TASK_PRIO);			// 测试 15
	OSTaskCreate(Beep_Task, (void*) 0, (OS_STK*) &BEEP_TASK_STK[TASK_STK_SIZE - 1], BEEP_TASK_PRIO);	// 42
	OSTaskCreate(Led_Task, (void*) 0, (OS_STK*) &LED_TASK_STK[TASK_STK_SIZE - 1], LED_TASK_PRIO);		// 43

	OSTaskSuspend(START_TASK_PRIO);	   // 挂起起始任务

	OS_EXIT_CRITICAL();	   // 退出临界区
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
int	 flag	 = 0;	 // 倒数第几位

// 15
static void R1_Task(void* pdata)
{
	pdata = pdata;
	// 这里含有全局被动技能：上一次操作执行完毕之前，不得执行下一次操作。会杀死指令
	while (1) {
		switch (ChooseFlag) {
			case 1:
				XXable(RX_message);		// 使能，失能
				if (ChooseFlag == 1)	// 其他操作直接跳过
					ChooseFlag = 0;
				break;
			case 2:
				FindZero(RX_message);	 // 寻零
				if (ChooseFlag == 2)
					ChooseFlag = 0;
				break;
			case 3:
				SetParameters(RX_message);	  // 设置发射俯仰角/回转角/速度
				if (ChooseFlag == 3)
					ChooseFlag = 0;
				break;
			case 4:
				FireInHere(RX_message);	   // 定点发射
				if (ChooseFlag == 4)
					ChooseFlag = 0;
				break;
			case 5:
				BallPaw(RX_message);	// 左右取球夹爪动作
				if (ChooseFlag == 5)
					ChooseFlag = 0;
				break;
			case 6:
				Seeding(RX_message);	// 取秧/放秧动作
				Cnt6--;
				if (ChooseFlag == 6)
					if (Cnt6 == 0)	  // 避免杀死命令
						ChooseFlag = 0;
				break;
			case 7:
				YouBack(RX_message);	// 回到初始位置
				if (ChooseFlag == 7)
					ChooseFlag = 0;
				break;

			default:
				break;
		}
		OSTimeDly(100);

		// 测试气缸代码
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

		// 方便修改data的代码
		if (KaiGuan) {
			if (flag) {
				RX_message.Data[1] = (1 << (flag - 1)) | RX_message.Data[1];
			}	 // 此位置1，其余不变
			else {
				RX_message.Data[1] = !(1 << (flag - 1)) & RX_message.Data[1];
			}	 // 此位置0，其余不变
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
