#include "motorencoder.h"

extern int Encoder; //外部变量 extern说明改变量已在其它文件定义
/**************************************************************************
函数功能：编码器初始化函数
入口参数：无
返回  值：无
**************************************************************************/
void MotorEncoder_Init(void)
{
	TIM_HandleTypeDef TIM4_Handler;		 //定时器句柄
	TIM_OC_InitTypeDef TIM4_CH1Handler;	 //定时器4通道1句柄
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体

	__HAL_RCC_TIM4_CLK_ENABLE();  //使能TIM4时钟
	__HAL_RCC_GPIOB_CLK_ENABLE(); //使能CPIOB时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // TIM4_CH1、TIM4_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);				   //根据GPIO_InitStructure的参数初始化GPIO

	TIM4_Handler.TIM_Period = 0xffff;				   //设定计数器自动重装值
	TIM4_Handler.TIM_Prescaler = 0;					   // 预分频器
	TIM4_Handler.TIM_ClockDivision = TIM_CKD_DIV1;	   //选择时钟分频：不分频
	TIM4_Handler.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM4_Handler);			   //根据TIM_TimeBaseInitStruct的参数初始化定时器TIM4

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //使用编码器模式3：CH1、CH2同时计数，为四分频
	TIM_ICStructInit(&TIM4_CH1Handler);																	  //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM4_CH1Handler.TIM_ICFilter = 10;																	  //设置滤波器长度
	TIM_ICInit(TIM4, &TIM4_CH1Handler);																	  //根TIM4_CH1Handler参数初始化定时器TIM4编码器模式

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);	   //清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //更新中断使能
	TIM_SetCounter(TIM4, 0);				   //初始化清空编码器数值

	TIM_Cmd(TIM4, ENABLE); //使能定时器4
}

/**************************************************************************
函数功能：读取TIM4编码器数值
入口参数：无
返回  值：无
**************************************************************************/
int Read_Encoder(void)
{
	int Encoder_TIM;
	Encoder_TIM = TIM4->CNT; //读取计数
	if (Encoder_TIM > 0xefff)
		Encoder_TIM = Encoder_TIM - 0xffff; //转化计数值为有方向的值，大于0正转，小于0反转。
											// TIM4->CNT范围为0-0xffff，初值为0。
	TIM4->CNT = 0;							//读取完后计数清零
	return Encoder_TIM;						//返回值
}

/**************************************************************************
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM4->SR & 0X0001) //溢出中断
	{
	}
	TIM4->SR &= ~(1 << 0); //清除中断标志位
}

/**************************************************************************
函数功能：通用定时器2初始化函数，
入口参数：自动重装载值 预分频系数 默认定时时钟为72MHZ时，两者共同决定定时中断时间
返回  值：无
**************************************************************************/
void Encoder_TIM2(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体
	NVIC_InitTypeDef NVIC_InitStrue;			   //定义一个中断优先级初始化的结构体

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能通用定时器2时钟

	TIM_TimeBaseInitStrue.TIM_Period = arr;						//计数模式为向上计数时，定时器从0开始计数，计数超过到arr时触发定时中断服务函数
	TIM_TimeBaseInitStrue.TIM_Prescaler = psc;					//预分频系数，决定每一个计数的时长
	TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up; //计数模式：向上计数
	TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;		//一般不使用，默认TIM_CKD_DIV1
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStrue);				//根据TIM_TimeBaseInitStrue的参数初始化定时器TIM2

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //使能TIM2中断，中断模式为更新中断：TIM_IT_Update

	NVIC_InitStrue.NVIC_IRQChannel = TIM2_IRQn;			  //属于TIM2中断
	NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;			  //中断使能
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 1; //抢占优先级为1级，值越小优先级越高，0级优先级最高
	NVIC_InitStrue.NVIC_IRQChannelSubPriority = 1;		  //响应优先级为1级，值越小优先级越高，0级优先级最高
	NVIC_Init(&NVIC_InitStrue);							  //根据NVIC_InitStrue的参数初始化VIC寄存器，设置TIM2中断

	TIM_Cmd(TIM2, ENABLE); //使能定时器TIM2
}

/**************************************************************************
函数功能：TIM4中断服务函数 定时读取编码器数值并清零
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler()
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == 1) //当发生中断时状态寄存器(TIMx_SR)的bit0会被硬件置1
	{
		Encoder = Read_Encoder();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //状态寄存器(TIMx_SR)的bit0置0
	}
}
