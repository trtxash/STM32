/**
 * @file	timer.c
 * @brief 	定时器设置
 *          适用STM32F401CCU6，F4一般都行，但具体定时器需要配置
 *          顺序为，句柄，定时器初始化，定时器PWM初始化，定时器底层驱动，定时器PWM底层驱动，定时器PWM占空比设置，中断服务，中断回调函数
 *          中断服务函数名可在 “startup_stm32f40_41xxx.s”文件中找到
 * @author 	TRTX-gamer
 * @version 1.00
 * @date 	2022年7月16号15点32分
 */
#include "timer.h"

TIM_HandleTypeDef TIM1_Handler;     //定时器1句柄
TIM_OC_InitTypeDef TIM1_CHxHandler; //定时器1通道句柄，4路
TIM_HandleTypeDef TIM2_Handler;     //定时器2句柄
TIM_OC_InitTypeDef TIM2_CHxHandler; //定时器2通道句柄，4路
TIM_HandleTypeDef TIM3_Handler;     //定时器3句柄
TIM_OC_InitTypeDef TIM3_CHxHandler; //定时器3通道句柄，4路
TIM_HandleTypeDef TIM4_Handler;     //定时器4句柄，用来发生PWM波
TIM_OC_InitTypeDef TIM4_CHxHandler; //定时器5通道句柄，4路
TIM_HandleTypeDef TIM5_Handler;     //定时器5句柄，用来发生PWM波
TIM_OC_InitTypeDef TIM5_CHxHandler; //定时器5通道句柄，4路

RxPack rxpack;

//通用定时器2中断初始化
// arr：自动重装值。
// psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
// F401 Timer clock is  APB1 clock.
void TIM1_Init(u16 arr, u16 psc)
{
    TIM1_Handler.Instance = TIM1;                             //定时器1
    TIM1_Handler.Init.Prescaler = psc;                        //分频系数
    TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数器
    TIM1_Handler.Init.Period = arr;                           //自动装载值
    TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
    HAL_TIM_Base_Init(&TIM1_Handler);

    HAL_TIM_Base_Start_IT(&TIM1_Handler); //使能定时器11和定时器11更新中断：TIM_IT_UPDATE
}

//通用定时器2中断初始化
// arr：自动重装值。
// psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
// F401 Timer clock is  APB1 clock.
void TIM2_Init(u16 arr, u16 psc)
{
    TIM2_Handler.Instance = TIM2;                             //定时器2
    TIM2_Handler.Init.Prescaler = psc;                        //分频系数
    TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数器
    TIM2_Handler.Init.Period = arr;                           //自动装载值
    TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);

    HAL_TIM_Base_Start_IT(&TIM2_Handler); //使能定时器11和定时器11更新中断：TIM_IT_UPDATE
}

//通用定时器3中断初始化
// arr：自动重装值。
// psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
// F401 Timer clock is  APB1 clock.
void TIM3_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM3_Handler.Instance = TIM3;                                        //通用定时器3
    TIM3_Handler.Init.Prescaler = psc;                                   //分频系数
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                  //向上计数器
    TIM3_Handler.Init.Period = arr;                                      //自动装载值
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            //时钟分频因子
    TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM3_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&TIM3_Handler, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&TIM3_Handler, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_TIM_ENABLE_IT(&TIM3_Handler, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE(&TIM3_Handler);
}

//通用定时器4中断初始化
// arr：自动重装值。
// psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
// F401 Timer clock is  APB1 clock.
void TIM4_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM4_Handler.Instance = TIM4;                                        //定时器11
    TIM4_Handler.Init.Prescaler = psc;                                   //分频系数
    TIM4_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                  //向上计数器
    TIM4_Handler.Init.Period = arr;                                      //自动装载值
    TIM4_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            //时钟分频因子
    TIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM4_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&TIM4_Handler, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&TIM4_Handler, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    __HAL_TIM_ENABLE_IT(&TIM4_Handler, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE(&TIM4_Handler);
}

// TIM5 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
// ways: PWM输出方式，例如可以选择输出PWM1和PWM2，那么就需要设置为0x03
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM4_PWM_Init(u16 arr, u16 psc, u8 ways)
{
    TIM4_Handler.Instance = TIM4;                             //定时器5
    TIM4_Handler.Init.Prescaler = psc;                        //定时器分频
    TIM4_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数模式
    TIM4_Handler.Init.Period = arr;                           //自动重装载值
    TIM4_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    HAL_TIM_PWM_Init(&TIM4_Handler);                          //初始化PWM

    TIM4_CHxHandler.OCMode = TIM_OCMODE_PWM1;        //模式选择PWM1
    TIM4_CHxHandler.Pulse = arr / 1;                 //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CHxHandler.OCPolarity = TIM_OCPOLARITY_LOW; //输出比较极性为低

    if (ways & 0B0001)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_1); //配置TIM5通道1
        HAL_TIM_PWM_Start(&TIM4_Handler, TIM_CHANNEL_1);                           //启动TIM5通道1的PWM输出
    }
    if (ways & 0B0010)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_2); //配置TIM5通道2
        HAL_TIM_PWM_Start(&TIM4_Handler, TIM_CHANNEL_2);                           //启动TIM5通道2的PWM输出
    }
    if (ways & 0B0100)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_3); //配置TIM5通道3
        HAL_TIM_PWM_Start(&TIM4_Handler, TIM_CHANNEL_3);                           //启动TIM5通道3的PWM输出
    }
    if (ways & 0B1000)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_4); //配置TIM5通道4
        HAL_TIM_PWM_Start(&TIM4_Handler, TIM_CHANNEL_4);                           //启动TIM5通道4的PWM输出
    }
}

// TIM5 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
// ways: PWM输出方式，例如可以选择输出PWM1和PWM2，那么就需要设置为0x03
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM5_PWM_Init(u16 arr, u16 psc, u8 ways)
{
    TIM5_Handler.Instance = TIM5;                             //定时器5
    TIM5_Handler.Init.Prescaler = psc;                        //定时器分频
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       //向上计数模式
    TIM5_Handler.Init.Period = arr;                           //自动重装载值
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    HAL_TIM_PWM_Init(&TIM5_Handler);                          //初始化PWM

    TIM5_CHxHandler.OCMode = TIM_OCMODE_PWM1;        //模式选择PWM1
    TIM5_CHxHandler.Pulse = arr / 1;                 //设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM5_CHxHandler.OCPolarity = TIM_OCPOLARITY_LOW; //输出比较极性为低

    if (ways & 0B0001)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_1); //配置TIM5通道1
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_1);                           //启动TIM5通道1的PWM输出
    }
    if (ways & 0B0010)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_2); //配置TIM5通道2
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_2);                           //启动TIM5通道2的PWM输出
    }
    if (ways & 0B0100)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_3); //配置TIM5通道3
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_3);                           //启动TIM5通道3的PWM输出
    }
    if (ways & 0B1000)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_4); //配置TIM5通道4
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_4);                           //启动TIM5通道4的PWM输出
    }
}

//定时器底册驱动，开启时钟，设置中断优先级
//此函数会被HAL_TIM_Base_Init()函数调用
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    }
    else if (htim->Instance == TIM2)
    {
        __HAL_RCC_TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
    else if (htim->Instance == TIM3)
    {
        __HAL_RCC_TIM3_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM3_IRQn, 4, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
    else if (htim->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM4_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
    }
    else if (htim->Instance == TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM5_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
// htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    if (htim->Instance == TIM1)
    {
    }
    else if (htim->Instance == TIM2)
    {
    }
    else if (htim->Instance == TIM3)
    {
    }
    else if (htim->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_Initure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;  //复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;      //上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH; //高速
        GPIO_Initure.Alternate = GPIO_AF2_TIM4;
        HAL_GPIO_Init(GPIOB, &GPIO_Initure);
    }
    else if (htim->Instance == TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();  //使能定时器5
        __HAL_RCC_GPIOA_CLK_ENABLE(); //开启GPIOA时钟

        GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3; // PA0~PA3
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;                                  //复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;                                      //上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH;                                 //高速
        GPIO_Initure.Alternate = GPIO_AF2_TIM5;                               // PA0,2复用为AF2_TIM5
        HAL_GPIO_Init(GPIOA, &GPIO_Initure);
    }
}

//设置TIM通道n的占空比
// compare:比较值,越大占空比越小
void TIM_SetTIM4Compare_n(u32 compare, u8 n)
{
    switch (n)
    {
    case 1:
        TIM4->CCR1 = compare;
        break;
    case 2:
        TIM4->CCR2 = compare;
        break;
    case 3:
        TIM4->CCR3 = compare;
        break;
    case 4:
        TIM4->CCR4 = compare;
        break;
    default:
        break;
    }
}

//设置TIM通道n的占空比
// compare:比较值,越大占空比越小
void TIM_SetTIM5Compare_n(u32 compare, u8 n)
{
    switch (n)
    {
    case 1:
        TIM5->CCR1 = compare;
        break;
    case 2:
        TIM5->CCR2 = compare;
        break;
    case 3:
        TIM5->CCR3 = compare;
        break;
    case 4:
        TIM5->CCR4 = compare;
        break;
    default:
        break;
    }
}

//设置TIM通道n的占空比
// DutyCycle:占空比,越大占空比越大，0~100
void TIM_SetTIM4_DutyCycle_n(u8 DutyCycle, u8 n)
{
    u32 compare;
    compare = (100 - DutyCycle) * TIM4_Handler.Init.Period / 100;
    switch (n)
    {
    case 1:
        TIM4->CCR1 = compare;
        break;
    case 2:
        TIM4->CCR2 = compare;
        break;
    case 3:
        TIM4->CCR3 = compare;
        break;
    case 4:
        TIM4->CCR4 = compare;
        break;
    default:
        break;
    }
}

//设置TIM通道n的占空比
// DutyCycle:占空比,越大占空比越大，0~100
void TIM_SetTIM5_DutyCycle_n(u8 DutyCycle, u8 n)
{
    u32 compare;
    compare = (100 - DutyCycle) * TIM5_Handler.Init.Period / 100;
    switch (n)
    {
    case 1:
        TIM5->CCR1 = compare;
        break;
    case 2:
        TIM5->CCR2 = compare;
        break;
    case 3:
        TIM5->CCR3 = compare;
        break;
    case 4:
        TIM5->CCR4 = compare;
        break;
    default:
        break;
    }
}

//定时器1中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
    // u8 temp2[16] = {0};

    // TIME_N5ms++;
    // tem = SMBus_ReadTemp();

    // if (readValuePack(&rxpack))
    // {
    //     /* 编码器读取 */
    //     Encoder_1 = rxpack.shorts[0];
    //     Encoder_2 = rxpack.shorts[1];
    //     Encoder_3 = rxpack.shorts[2];
    //     Encoder_4 = rxpack.shorts[3];

    //     /* 红外数据处理 */
    //     Now_pos_num = Read_Infraredtobe_sums();

    //     /* 小车状态 */
    //     Encoder_target_1 = Encoder_target_2 = Encoder_target_3 = Encoder_target_4 = Encoder_target; // 编码器赋值
    //     if (car_state == 1)                                                                         // 直走
    //     {
    //         if (Now_pos_num > 0 && Now_pos_num < 9)
    //         {
    //             if (Now_pos_num == 1) // 右转
    //             {
    //                 Encoder_target_2 += 25;
    //                 Encoder_target_4 += 25;
    //                 Encoder_target_1 -= 25;
    //                 Encoder_target_3 -= 25;
    //             }
    //             else if (Now_pos_num >= 3) // 大右转
    //             {
    //                 Encoder_target_2 += 75;
    //                 Encoder_target_4 += 75;
    //                 Encoder_target_1 -= 75;
    //                 Encoder_target_3 -= 75;
    //             }
    //         }
    //         else if (Now_pos_num < 0)
    //         {
    //             if (Now_pos_num == -1) // 左转
    //             {
    //                 Encoder_target_2 -= 25;
    //                 Encoder_target_4 -= 25;
    //                 Encoder_target_1 += 25;
    //                 Encoder_target_3 += 25;
    //             }
    //             else if (Now_pos_num <= -3) // 大左转
    //             {
    //                 Encoder_target_2 -= 75;
    //                 Encoder_target_4 -= 75;
    //                 Encoder_target_1 += 75;
    //                 Encoder_target_3 += 75;
    //             }
    //         }
    //     }
    //     else if (car_state == -1) // 停止
    //     {
    //         Encoder_target_1 = Encoder_target_2 = Encoder_target_3 = Encoder_target_4 = Encoder_target = 0;
    //         pwmval_1 = pwmval_2 = pwmval_3 = pwmval_4 = 0;
    //     }
    //     else if (car_state == 2) // 右转
    //     {
    //         Encoder_target_2 = 250;
    //         Encoder_target_4 = 250;
    //         Encoder_target_1 = -250;
    //         Encoder_target_3 = -250;
    //     }
    //     else if (car_state == -2) // 左转
    //     {
    //     }

    //     /* PID计算与反馈 */
    //     pwmval_1 = Velocity_FeedbackControl_1(Encoder_target_1, Encoder_1); //速度反馈控制
    //     /* TIM4，ch1右上反转，ch2右上正转，ch3左上反转，ch4左上正转 */
    //     if (pwmval_1 >= 0)
    //     {
    //         TIM_SetTIM4_DutyCycle_n(pwmval_1, 2); //修改比较值，修改占空比
    //         TIM_SetTIM4_DutyCycle_n(0, 1);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM4_DutyCycle_n(-pwmval_1, 1); //修改比较值，修改占空比
    //         TIM_SetTIM4_DutyCycle_n(0, 2);         //修改比较值，修改占空比
    //     }
    //     pwmval_2 = Velocity_FeedbackControl_2(Encoder_target_2, Encoder_2); //速度反馈控制
    //     /* TIM4，ch1右上反转，ch2右上正转，ch3左上反转，ch4左上正转 */
    //     if (pwmval_2 >= 0)
    //     {
    //         TIM_SetTIM4_DutyCycle_n(pwmval_2, 4); //修改比较值，修改占空比
    //         TIM_SetTIM4_DutyCycle_n(0, 3);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM4_DutyCycle_n(-pwmval_2, 3); //修改比较值，修改占空比
    //         TIM_SetTIM4_DutyCycle_n(0, 4);         //修改比较值，修改占空比
    //     }
    //     pwmval_3 = Velocity_FeedbackControl_3(Encoder_target_3, Encoder_3); //速度反馈控制
    //     /* TIM5，ch1左下正转，ch2左下反转，ch3右下反转，ch4右下正转 */
    //     if (pwmval_3 >= 0)
    //     {
    //         TIM_SetTIM5_DutyCycle_n(pwmval_3, 4); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 3);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM5_DutyCycle_n(-pwmval_3, 3); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 4);         //修改比较值，修改占空比
    //     }
    //     pwmval_4 = Velocity_FeedbackControl_4(Encoder_target_4, Encoder_4); //速度反馈控制
    //     /* TIM5，ch1左下正转，ch2左下反转，ch3右下反转，ch4右下正转 */
    //     if (pwmval_4 >= 0)
    //     {
    //         TIM_SetTIM5_DutyCycle_n(pwmval_4, 1); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 2);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM5_DutyCycle_n(-pwmval_4, 2); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 1);         //修改比较值，修改占空比
    //     }
    // }

    // /* 红外 */
    // for (u8 i = 0; i < 8; i++)
    // {
    //     OLED_ShowNum(i * 6, 54, rxpack.bools[i], 1, 8, 1);
    // }
    // if (Now_pos_num >= 0)
    // {
    //     OLED_ShowChar(50, 54, '+', 8, 1);
    //     OLED_ShowNum(56, 54, Now_pos_num, 1, 8, 1);
    // }
    // else
    // {
    //     OLED_ShowChar(50, 54, '-', 8, 1);
    //     OLED_ShowNum(56, 54, -Now_pos_num, 1, 8, 1);
    // }

    // /* 编码器显示 */
    // if (Encoder_1 >= 0)
    // {
    //     OLED_ShowChar(0, 16, '+', 8, 1);
    //     OLED_ShowNum(6, 16, Encoder_1, 4, 8, 1); // 显示红外传感器的数值
    // }
    // else
    // {
    //     OLED_ShowChar(0, 16, '-', 8, 1);
    //     OLED_ShowNum(6, 16, -Encoder_1, 4, 8, 1); // 显示红外传感器的数值
    // }
    // if (Encoder_2 >= 0)
    // {
    //     OLED_ShowChar(0, 24, '+', 8, 1);
    //     OLED_ShowNum(6, 24, Encoder_2, 4, 8, 1); // 显示红外传感器的数值
    // }
    // else
    // {
    //     OLED_ShowChar(0, 24, '-', 8, 1);
    //     OLED_ShowNum(6, 24, -Encoder_2, 4, 8, 1); // 显示红外传感器的数值
    // }
    // if (Encoder_3 >= 0)
    // {
    //     OLED_ShowChar(0, 32, '+', 8, 1);
    //     OLED_ShowNum(6, 32, Encoder_3, 4, 8, 1); // 显示红外传感器的数值
    // }
    // else
    // {
    //     OLED_ShowChar(0, 32, '-', 8, 1);
    //     OLED_ShowNum(6, 32, -Encoder_3, 4, 8, 1); // 显示红外传感器的数值
    // }
    // if (Encoder_4 >= 0)
    // {
    //     OLED_ShowChar(0, 40, '+', 8, 1);
    //     OLED_ShowNum(6, 40, Encoder_4, 4, 8, 1); // 显示红外传感器的数值
    // }
    // else
    // {
    //     OLED_ShowChar(0, 40, '-', 8, 1);
    //     OLED_ShowNum(6, 40, -Encoder_4, 4, 8, 1); // 显示红外传感器的数值
    // }

    // /* 温度和电量 */
    // sprintf(temp2, "TEM:%3.1f;POW:%d", tem, rxpack.shorts[4]);
    // OLED_ShowString(0, 0, temp2, 16, 1);

    // /* 接受错误 */
    // OLED_ShowNum(0, 48, err, 5, 8, 1);

    // OLED_Refresh();

    // __HAL_TIM_CLEAR_FLAG(&TIM1_Handler, TIM_FLAG_UPDATE); //清除更新标志
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
    // u8 temp_x;
    // u8 temp[16]; // 储存要显示的字符串，最多16个字符

    // if (bluetooth) // 是否蓝牙遥控
    // {
    //     /* 速度处理 */
    //     TargetSpeed_1 = TargetSpeed_2 = TargetSpeed;
    //     TargetSpeed_1 += Angle_Target / 2;
    //     TargetSpeed_2 -= Angle_Target / 2;

    //     Encoder_1 = -Calculate_Velocity(Read_Encoder(3));                //读取编码器的值计算速度
    //     pwmval_1 = Velocity_FeedbackControl_1(TargetSpeed_1, Encoder_1); //速度反馈控制
    //     if (pwmval_1 >= 0)
    //     {
    //         TIM_SetTIM5_DutyCycle_n(pwmval_1, 2); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 1);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM5_DutyCycle_n(-pwmval_1, 1); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 2);         //修改比较值，修改占空比
    //     }

    //     Encoder_2 = Calculate_Velocity(Read_Encoder(4));                 //读取编码器的值计算速度
    //     pwmval_2 = Velocity_FeedbackControl_2(TargetSpeed_2, Encoder_2); //速度反馈控制
    //     if (pwmval_2 >= 0)
    //     {
    //         TIM_SetTIM5_DutyCycle_n(pwmval_2, 4); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 3);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM5_DutyCycle_n(-pwmval_2, 3); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 4);         //修改比较值，修改占空比
    //     }
    // }
    // else // 默认循迹
    // {
    //     /* 速度处理 */
    //     TargetSpeed_1 = TargetSpeed_2 = TargetSpeed;
    //     Now_pos = Read_Infraredtobe_bits();     //读取红外传感器数据
    //     Now_pos_num = Read_Infraredtobe_sums(); // 读取红外传感器数据的数值
    //     if (Now_pos_num > 0)
    //     {
    //         if (Now_pos == 1)
    //         {
    //             TargetSpeed_1 += 0.2;
    //             TargetSpeed_2 -= 0.2;
    //         }
    //         else if (Now_pos == 2)
    //         {
    //             TargetSpeed_1 += 0.4;
    //             TargetSpeed_2 -= 0.4;
    //         }
    //         else
    //         {
    //             TargetSpeed_1 += 1.0;
    //             TargetSpeed_2 -= 1.0;
    //         }
    //     }
    //     else if (Now_pos_num < 0)
    //     {
    //         if (Now_pos == -1)
    //         {
    //             TargetSpeed_1 -= 0.2;
    //             TargetSpeed_2 += 0.2;
    //         }
    //         else if (Now_pos == 2)
    //         {
    //             TargetSpeed_1 -= 0.4;
    //             TargetSpeed_2 += 0.4;
    //         }
    //         else
    //         {
    //             TargetSpeed_1 -= 1.0;
    //             TargetSpeed_2 += 1.0;
    //         }
    //     }

    //     // TargetSpeed_1 += (double)Now_pos_num / 3.7;
    //     // TargetSpeed_2 -= (double)Now_pos_num / 3.7;
    //     /* 左右电机分别速度控制 */
    //     Encoder_1 = -Calculate_Velocity(Read_Encoder(3));         //读取编码器的值计算速度
    //     pwmval_1 = X_FeedbackControl_1(TargetSpeed_1, Encoder_1); //速度反馈控制
    //     if (pwmval_1 >= 0)
    //     {
    //         TIM_SetTIM5_DutyCycle_n(pwmval_1, 2); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 1);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM5_DutyCycle_n(-pwmval_1, 1); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 2);         //修改比较值，修改占空比
    //     }

    //     Encoder_2 = Calculate_Velocity(Read_Encoder(4));          //读取编码器的值计算速度
    //     pwmval_2 = X_FeedbackControl_2(TargetSpeed_2, Encoder_2); //速度反馈控制
    //     if (pwmval_2 >= 0)
    //     {
    //         TIM_SetTIM5_DutyCycle_n(pwmval_2, 4); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 3);        //修改比较值，修改占空比
    //     }
    //     else
    //     {
    //         TIM_SetTIM5_DutyCycle_n(-pwmval_2, 3); //修改比较值，修改占空比
    //         TIM_SetTIM5_DutyCycle_n(0, 4);         //修改比较值，修改占空比
    //     }
    // }

    // if (!move) // 不移动关电机
    // {
    //     TargetSpeed_1 = TargetSpeed_2 = 0;
    //     pwmval_1 = pwmval_2 = 0;
    //     TIM_SetTIM5_DutyCycle_n(0, 1);
    //     TIM_SetTIM5_DutyCycle_n(0, 2);
    //     TIM_SetTIM5_DutyCycle_n(0, 3);
    //     TIM_SetTIM5_DutyCycle_n(0, 4);
    // }

    // /* OLED显示部分 */
    // sprintf(temp, "%4.2f", Angle_Target / 1.5); //将速度转换为字符串
    // OLED_ShowString(24, 0, temp, 16, 1);
    // sprintf(temp, "%4.2f", TargetSpeed); //将速度转换为字符串
    // OLED_ShowString(88, 0, temp, 16, 1);

    // sprintf(temp, "%4.2f", Encoder_1); //将速度转换为字符串
    // OLED_ShowString(24, 16, temp, 16, 1);
    // sprintf(temp, "%4.2f", Encoder_2); //将速度转换为字符串
    // OLED_ShowString(88, 16, temp, 16, 1);

    // OLED_ShowNum(0, 48, (u16)Now_pos, 4, 16, 1); // 显示红外传感器的数值
    // if (Now_pos_num >= 0)
    // {
    //     OLED_ShowChar(64, 48, '+', 16, 1);
    //     OLED_ShowNum(72, 48, Now_pos_num, 1, 16, 1); // 显示红外传感器的数值
    // }
    // else
    // {
    //     OLED_ShowChar(64, 48, '-', 16, 1);
    //     OLED_ShowNum(72, 48, -Now_pos_num, 1, 16, 1); // 显示红外传感器的数值
    // }
    // OLED_Refresh();

    // /* 标志清除部分 */
    // __HAL_TIM_CLEAR_FLAG(&TIM2_Handler, TIM_FLAG_UPDATE); //清除更新标志
}
//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE))
    {
        u32 status_value = taskENTER_CRITICAL_FROM_ISR(); //进入临界区
        printf("TIM3输出......\r\n");
        taskEXIT_CRITICAL_FROM_ISR(status_value);             //退出临界区
        __HAL_TIM_CLEAR_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE); //清除更新标志
    }
}
//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&TIM4_Handler, TIM_FLAG_UPDATE))
    {
        u32 status_value = taskENTER_CRITICAL_FROM_ISR(); //进入临界区
        printf("TIM4输出......\r\n");
        taskEXIT_CRITICAL_FROM_ISR(status_value);             //退出临界区
        __HAL_TIM_CLEAR_FLAG(&TIM4_Handler, TIM_FLAG_UPDATE); //清除更新标志
    }
}
//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);
}

//回调函数，定时器中断服务函数调用
//中断回调函数会自动清除中断标志
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&TIM2_Handler))
    {
    }
    else if (htim == (&TIM3_Handler))
    {
    }
    else if (htim == (&TIM4_Handler))
    {
    }
    else if (htim == (&TIM5_Handler))
    {
    }
}
