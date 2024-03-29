#include "delay.h"

/** 
  * @brief  延时、毫秒
  */
	
void Delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

/** 
  * @brief  延时、微秒
  */
void Delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
}

/** 
  * @brief  延时、指令周期数
  */

void Delay(u16 t)
{
	while(t--);
}

 

 
u32 my_us = 21;     //计21个数，用时1us
u32 my_ms = 21000;  //计21000个数，用时1ms
 
void Delay_Init(void)
{
	//systick定时器频率：168/8 = 21MHZ    
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	//配置systick时钟源及中断	SystemCoreClock = 168000 000  1ms产生中断
	//SysTick_Config(SystemCoreClock/1000);  //初始化systick,时钟为HCLK,并开启中断
 
}
 
 
//参数u32 nus值范围：0~798915
void delay_us(u32 nus)
{
	u32 temp;
	
	//设置重装值
	SysTick->LOAD = my_us*nus - 1;
	//设置VAL为0
	SysTick->VAL = 0x00;	
	//开定时器
	SysTick->CTRL |= (0x01<<0);
	
	do
	{
		temp = SysTick->CTRL;  //temp 第十六为1,则说时计数到0
	}while(  temp & (0x01<<0) && !(temp & (0x01<<16)) );
			//判断定时器是否开启   判断计数是否到0
	
	//关定时器
	SysTick->CTRL &= ~(0x01<<0);	
 
}
 
//参数u32 nms值范围：0~798
void delay_ms(u32 nms)
{
 
	u32 temp;
	
	//设置重装值
	SysTick->LOAD = my_ms*nms - 1;
	//设置VAL为0
	SysTick->VAL = 0x00;
	
	//开定时器
	SysTick->CTRL |= (0x01<<0);
	
	
	do
	{
		temp = SysTick->CTRL;  //temp 第十六为1,则说时计数到0
	}while(  temp & (0x01<<0) && !(temp & (0x01<<16)) );
			//判断定时器是否开启   判断计数是否到0
	
	//关定时器
	SysTick->CTRL &= ~(0x01<<0);
 
}
 
 
void delay_s(int n)
{
	int i;
	for(i=0; i<n; i++)
	{
		delay_ms(500);
		delay_ms(500);
	}

}

