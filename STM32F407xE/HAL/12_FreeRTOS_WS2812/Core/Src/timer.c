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

u32 sec;
u32 fps;
u32 fps_num;

TIM_HandleTypeDef TIM1_Handler;            // 定时器1句柄
TIM_OC_InitTypeDef TIM1_CHxHandler;        // 定时器1通道句柄，4路
TIM_HandleTypeDef TIM2_Handler;            // 定时器2句柄
TIM_OC_InitTypeDef TIM2_CHxHandler;        // 定时器2通道句柄，4路
TIM_HandleTypeDef TIM3_Handler;            // 定时器3句柄
TIM_OC_InitTypeDef TIM3_CHxHandler;        // 定时器3通道句柄，4路
TIM_HandleTypeDef TIM4_Handler;            // 定时器4句柄
TIM_MasterConfigTypeDef TIM4_MasterConfig; // 定时器4主配置
TIM_OC_InitTypeDef TIM4_CHxHandler;        // 定时器4通道句柄，4路
DMA_HandleTypeDef hdma_tim4_ch1;           // 定时器4，ch1
TIM_HandleTypeDef TIM5_Handler;            // 定时器5句柄，用来发生PWM波
TIM_OC_InitTypeDef TIM5_CHxHandler;        // 定时器5通道句柄，4路
TIM_HandleTypeDef TIM8_Handler;            // 定时器8句柄，用来发生PWM波
TIM_OC_InitTypeDef TIM8_CHxHandler;        // 定时器8通道句柄，4路
DMA_HandleTypeDef hdma_tim8_ch1;           // 定时器CH1，PWM+DMA
TIM_HandleTypeDef TIM12_Handler;           // 定时器12句柄，用来发生PWM波
TIM_OC_InitTypeDef TIM12_CHxHandler;       // 定时器12通道句柄，1路
TIM_HandleTypeDef TIM13_Handler;           // 定时器13句柄
TIM_HandleTypeDef TIM14_Handler;           // 定时器14句柄

// 通用定时器2中断初始化
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM1_Init(u16 arr, u16 psc)
{
    TIM1_Handler.Instance = TIM1;                             // 定时器1
    TIM1_Handler.Init.Prescaler = psc;                        // 分频系数
    TIM1_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数器
    TIM1_Handler.Init.Period = arr;                           // 自动装载值
    TIM1_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟分频因子
    HAL_TIM_Base_Init(&TIM1_Handler);

    HAL_TIM_Base_Start_IT(&TIM1_Handler); // 使能定时器11和定时器11更新中断：TIM_IT_UPDATE
}

// 通用定时器2中断初始化
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM2_Init(u16 arr, u16 psc)
{
    TIM2_Handler.Instance = TIM2;                             // 定时器2
    TIM2_Handler.Init.Prescaler = psc;                        // 分频系数
    TIM2_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数器
    TIM2_Handler.Init.Period = arr;                           // 自动装载值
    TIM2_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟分频因子
    HAL_TIM_Base_Init(&TIM2_Handler);

    HAL_TIM_Base_Start_IT(&TIM2_Handler); // 使能定时器11和定时器11更新中断：TIM_IT_UPDATE
}

// 通用定时器3中断初始化
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM3_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM3_Handler.Instance = TIM3;                                        // 通用定时器3
    TIM3_Handler.Init.Prescaler = psc;                                   // 分频系数
    TIM3_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 向上计数器
    TIM3_Handler.Init.Period = arr;                                      // 自动装载值
    TIM3_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            // 时钟分频因子
    TIM3_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM3_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_Base_Start_IT(&TIM3_Handler) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    {
        Error_Handler();
    }
}

// 通用定时器4中断初始化
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器4!(定时器4挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM4_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM4_Handler.Instance = TIM4;                                        // 定时器11
    TIM4_Handler.Init.Prescaler = psc;                                   // 分频系数
    TIM4_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 向上计数器
    TIM4_Handler.Init.Period = arr;                                      // 自动装载值
    TIM4_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            // 时钟分频因子
    TIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM4_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_Base_Start_IT(&TIM4_Handler) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    {
        Error_Handler();
    }
}

// 通用定时器8中断初始化
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器4!(定时器4挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM8_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM8_Handler.Instance = TIM8;                                        // 定时器11
    TIM8_Handler.Init.Prescaler = psc;                                   // 分频系数
    TIM8_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 向上计数器
    TIM8_Handler.Init.Period = arr;                                      // 自动装载值
    TIM8_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            // 时钟分频因子
    TIM8_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM8_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_Base_Start_IT(&TIM8_Handler) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    {
        Error_Handler();
    }
}

// 通用定时器4中断初始化
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器4!(定时器4挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM12_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM12_Handler.Instance = TIM12;                                       // 定时器11
    TIM12_Handler.Init.Prescaler = psc;                                   // 分频系数
    TIM12_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 向上计数器
    TIM12_Handler.Init.Period = arr;                                      // 自动装载值
    TIM12_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            // 时钟分频因子
    TIM12_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM12_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_Base_Start_IT(&TIM12_Handler) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    {
        Error_Handler();
    }
}

// 通用定时器13中断初始化
// !!!用作了USMART定时!!!
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器13!(定时器13挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM13_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM13_Handler.Instance = TIM13;                            // 定时器13
    TIM13_Handler.Init.Prescaler = psc;                        // 分频系数
    TIM13_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数器
    TIM13_Handler.Init.Period = arr;                           // 装载值
    TIM13_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 时钟分频因子
    // TIM13_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM13_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_Base_Start_IT(&TIM13_Handler) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    {
        Error_Handler();
    }
}

// 通用定时器14中断初始化
// !!!用作了时间定时!!!
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
// 这里使用的是定时器14!(定时器14挂在APB1上，时钟为HCLK/2)(F407)
//  F401 Timer clock is  APB1 clock.
void TIM14_Init(u16 arr, u16 psc)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    TIM14_Handler.Instance = TIM14;                                       // 定时器11
    TIM14_Handler.Init.Prescaler = psc;                                   // 分频系数
    TIM14_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 向上计数器
    TIM14_Handler.Init.Period = arr;                                      // 自动装载值
    TIM14_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;            // 时钟分频因子
    TIM14_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; // ARPE,1有更新事件才自动重装他的上限值,如果用OS尽量要打开
    if (HAL_TIM_Base_Init(&TIM14_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_Base_Start_IT(&TIM14_Handler) != HAL_OK) // 也可以用这个，使能定时器3和定时器3更新中断：TIM_IT_UPDATE
    {
        Error_Handler();
    }
}

// TIM5 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
// ways: PWM输出方式，例如可以选择输出PWM1和PWM2，那么就需要设置为0x03
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM4_PWM_Init(u16 arr, u16 psc, u8 ways)
{
    TIM4_Handler.Instance = TIM4;                             // 定时器5
    TIM4_Handler.Init.Prescaler = psc;                        // 定时器分频
    TIM4_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数模式
    TIM4_Handler.Init.Period = arr;                           // 自动重装载值
    TIM4_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    TIM4_Handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&TIM4_Handler) != HAL_OK)
    {
        Error_Handler();
    }
    TIM4_MasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    TIM4_MasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&TIM4_Handler, &TIM4_MasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    // HAL_TIM_PWM_Init(&TIM4_Handler); // 初始化PWM

    TIM4_CHxHandler.OCMode = TIM_OCMODE_PWM1;         // 模式选择PWM1
    TIM4_CHxHandler.Pulse = 0;                        // 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM4_CHxHandler.OCPolarity = TIM_OCPOLARITY_HIGH; // 输出比较极性为高
    // TIM4_CHxHandler.OCIdleState = TIM_OCIDLESTATE_RESET;
    // TIM4_CHxHandler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    TIM4_CHxHandler.OCFastMode = TIM_OCFAST_DISABLE;
    if (ways & 0B0001)
    {
        if (HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_1) != HAL_OK)
        {
            Error_Handler();
        }; // 配置TIM5通道1
           // HAL_TIM_PWM_Start(&TIM4_Handler, TIM_CHANNEL_1);                           // 启动TIM5通道1的PWM输出
    }
    if (ways & 0B0010)
    {
        if (HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_2) != HAL_OK)
        {
            Error_Handler();
        }; // 配置TIM5通道2
    }
    if (ways & 0B0100)
    {
        if (HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_3) != HAL_OK)
        {
            Error_Handler();
        }; // 配置TIM5通道3
    }
    if (ways & 0B1000)
    {
        if (HAL_TIM_PWM_ConfigChannel(&TIM4_Handler, &TIM4_CHxHandler, TIM_CHANNEL_4) != HAL_OK)
        {
            Error_Handler();
        }; // 配置TIM5通道4
    }
}

// TIM5 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
// ways: PWM输出方式，例如可以选择输出PWM1和PWM2，那么就需要设置为0x03
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM5_PWM_Init(u16 arr, u16 psc, u8 ways)
{
    TIM5_Handler.Instance = TIM5;                             // 定时器5
    TIM5_Handler.Init.Prescaler = psc;                        // 定时器分频
    TIM5_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数模式
    TIM5_Handler.Init.Period = arr;                           // 自动重装载值
    TIM5_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    HAL_TIM_PWM_Init(&TIM5_Handler);                          // 初始化PWM

    TIM5_CHxHandler.OCMode = TIM_OCMODE_PWM1;        // 模式选择PWM1
    TIM5_CHxHandler.Pulse = arr / 1;                 // 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM5_CHxHandler.OCPolarity = TIM_OCPOLARITY_LOW; // 输出比较极性为低

    if (ways & 0B0001)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_1); // 配置TIM5通道1
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_1);                           // 启动TIM5通道1的PWM输出
    }
    if (ways & 0B0010)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_2); // 配置TIM5通道2
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_2);                           // 启动TIM5通道2的PWM输出
    }
    if (ways & 0B0100)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_3); // 配置TIM5通道3
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_3);                           // 启动TIM5通道3的PWM输出
    }
    if (ways & 0B1000)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM5_Handler, &TIM5_CHxHandler, TIM_CHANNEL_4); // 配置TIM5通道4
        HAL_TIM_PWM_Start(&TIM5_Handler, TIM_CHANNEL_4);                           // 启动TIM5通道4的PWM输出
    }
}

// TIM8 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
// ways: PWM输出方式，例如可以选择输出PWM1和PWM2，那么就需要设置为0x03
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM8_PWM_Init(u16 arr, u16 psc, u8 ways)
{
    TIM8_Handler.Instance = TIM8;                             // 定时器5
    TIM8_Handler.Init.Prescaler = psc;                        // 定时器分频
    TIM8_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数模式
    TIM8_Handler.Init.Period = arr;                           // 自动重装载值
    TIM8_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    HAL_TIM_PWM_Init(&TIM8_Handler);                          // 初始化PWM

    TIM8_CHxHandler.OCMode = TIM_OCMODE_PWM1;        // 模式选择PWM1
    TIM8_CHxHandler.Pulse = arr / 1;                 // 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM8_CHxHandler.OCPolarity = TIM_OCPOLARITY_LOW; // 输出比较极性为低

    if (ways & 0B0001)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &TIM8_CHxHandler, TIM_CHANNEL_1); // 配置TIM5通道1
        HAL_TIM_PWM_Start(&TIM8_Handler, TIM_CHANNEL_1);                           // 启动TIM5通道1的PWM输出
    }
    if (ways & 0B0010)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &TIM8_CHxHandler, TIM_CHANNEL_2); // 配置TIM5通道2
        HAL_TIM_PWM_Start(&TIM8_Handler, TIM_CHANNEL_2);                           // 启动TIM5通道2的PWM输出
    }
    if (ways & 0B0100)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &TIM8_CHxHandler, TIM_CHANNEL_3); // 配置TIM5通道3
        HAL_TIM_PWM_Start(&TIM8_Handler, TIM_CHANNEL_3);                           // 启动TIM5通道3的PWM输出
    }
    if (ways & 0B1000)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM8_Handler, &TIM8_CHxHandler, TIM_CHANNEL_4); // 配置TIM5通道4
        HAL_TIM_PWM_Start(&TIM8_Handler, TIM_CHANNEL_4);                           // 启动TIM5通道4的PWM输出
    }
}

// TIM12 PWM部分初始化
// arr：自动重装值。
// psc：时钟预分频数
// ways: PWM输出方式，例如可以选择输出PWM1和PWM2，那么就需要设置为0x03,tim12 pwm只有2个通道
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM12_PWM_Init(u16 arr, u16 psc, u8 ways)
{
    TIM12_Handler.Instance = TIM12;                            // 定时器5
    TIM12_Handler.Init.Prescaler = psc;                        // 定时器分频
    TIM12_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;       // 向上计数模式
    TIM12_Handler.Init.Period = arr;                           // 自动重装载值
    TIM12_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; // 数字分频因子,数字分频因子为1时，TIMxCLK = PCLKx,其他值时，TIMxCLK = PCLKx/2^(数字分频因子)
    HAL_TIM_PWM_Init(&TIM12_Handler);                          // 初始化PWM

    TIM12_CHxHandler.OCMode = TIM_OCMODE_PWM1;        // 模式选择PWM1
    TIM12_CHxHandler.Pulse = arr / 1;                 // 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM12_CHxHandler.OCPolarity = TIM_OCPOLARITY_LOW; // 输出比较极性为低

    if (ways & 0B0001)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM12_Handler, &TIM12_CHxHandler, TIM_CHANNEL_1); // 配置TIM5通道1
        HAL_TIM_PWM_Start(&TIM12_Handler, TIM_CHANNEL_1);                            // 启动TIM5通道1的PWM输出
    }
    if (ways & 0B0010)
    {
        HAL_TIM_PWM_ConfigChannel(&TIM12_Handler, &TIM12_CHxHandler, TIM_CHANNEL_2); // 配置TIM5通道2
        HAL_TIM_PWM_Start(&TIM12_Handler, TIM_CHANNEL_2);                            // 启动TIM5通道2的PWM输出
    }
}

// 定时器底册驱动，开启时钟，设置中断优先级
// 此函数会被HAL_TIM_Base_Init()函数调用
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
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);
    }
    else if (htim->Instance == TIM4)
    {
        __HAL_RCC_TIM4_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM4_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM4_IRQn);
    }
    else if (htim->Instance == TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM5_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM5_IRQn);
    }
    else if (htim->Instance == TIM8)
    {
        __HAL_RCC_TIM8_CLK_ENABLE();
    }
    else if (htim->Instance == TIM12)
    {
        __HAL_RCC_TIM12_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM8_BRK_TIM12_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM8_BRK_TIM12_IRQn);
    }
    else if (htim->Instance == TIM13)
    {
        __HAL_RCC_TIM13_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM8_UP_TIM13_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM8_UP_TIM13_IRQn);
    }
    else if (htim->Instance == TIM14)
    {
        __HAL_RCC_TIM14_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
    }
}

// 定时器底层驱动，时钟使能，引脚配置
// 此函数会被HAL_TIM_PWM_Init()调用
//  htim:定时器句柄
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
        __HAL_RCC_GPIOD_CLK_ENABLE();

        GPIO_Initure.Pin = GPIO_PIN_12;
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;  // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;      // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH; // 高速
        GPIO_Initure.Alternate = GPIO_AF2_TIM4;
        HAL_GPIO_Init(GPIOD, &GPIO_Initure);

        hdma_tim4_ch1.Instance = DMA1_Stream0;
        hdma_tim4_ch1.Init.Channel = DMA_CHANNEL_2;
        hdma_tim4_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tim4_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tim4_ch1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tim4_ch1.Init.PeriphDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_tim4_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
        hdma_tim4_ch1.Init.Mode = DMA_CIRCULAR;
        hdma_tim4_ch1.Init.Priority = DMA_PRIORITY_HIGH;
        hdma_tim4_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_tim4_ch1) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC1], hdma_tim4_ch1);
    }
    else if (htim->Instance == TIM5)
    {
        __HAL_RCC_TIM5_CLK_ENABLE();  // 使能定时器5
        __HAL_RCC_GPIOA_CLK_ENABLE(); // 开启GPIOA时钟

        GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3; // PA0~PA3
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;                                  // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;                                      // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH;                                 // 高速
        GPIO_Initure.Alternate = GPIO_AF2_TIM5;                               // PA0,2复用为AF2_TIM5
        HAL_GPIO_Init(GPIOA, &GPIO_Initure);
    }
    else if (htim->Instance == TIM8)
    {
        __HAL_RCC_TIM8_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();

        GPIO_Initure.Pin = GPIO_PIN_6;
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;  // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;      // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH; // 高速
        GPIO_Initure.Alternate = GPIO_AF3_TIM8;
        HAL_GPIO_Init(GPIOC, &GPIO_Initure);

        hdma_tim8_ch1.Instance = DMA2_Stream2;
        hdma_tim8_ch1.Init.Channel = DMA_CHANNEL_7;
        hdma_tim8_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tim8_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tim8_ch1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tim8_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim8_ch1.Init.MemDataAlignment = DMA_PDATAALIGN_HALFWORD;
        hdma_tim8_ch1.Init.Mode = DMA_NORMAL;
        hdma_tim8_ch1.Init.Priority = DMA_PRIORITY_LOW;
        hdma_tim8_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hdma_tim8_ch1) != HAL_OK)
        {
            Error_Handler();
        }

        __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_CC1], hdma_tim8_ch1);
    }
    else if (htim->Instance == TIM12)
    {
        __HAL_RCC_TIM12_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        GPIO_Initure.Pin = GPIO_PIN_14;
        GPIO_Initure.Mode = GPIO_MODE_AF_PP;  // 复用推挽输出
        GPIO_Initure.Pull = GPIO_PULLUP;      // 上拉
        GPIO_Initure.Speed = GPIO_SPEED_HIGH; // 高速
        GPIO_Initure.Alternate = GPIO_AF9_TIM12;
        HAL_GPIO_Init(GPIOB, &GPIO_Initure);
    }
}

// 设置TIM通道n的占空比
//  compare:比较值,越大占空比越小
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

// 设置TIM通道n的占空比
//  compare:比较值,越大占空比越小
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

// 设置TIM通道n的占空比
//  compare:比较值,越大占空比越小
void TIM_SetTIM8Compare_n(u32 compare, u8 n)
{
    switch (n)
    {
    case 1:
        TIM8->CCR1 = compare;
        break;
    case 2:
        TIM8->CCR2 = compare;
        break;
    case 3:
        TIM8->CCR3 = compare;
        break;
    case 4:
        TIM8->CCR4 = compare;
        break;
    default:
        break;
    }
}

// 设置TIM通道n的占空比
//  compare:比较值,越大占空比越小
void TIM_SetTIM12Compare_n(u32 compare, u8 n)
{
    switch (n)
    {
    case 1:
        TIM12->CCR1 = compare;
        break;
    case 2:
        TIM12->CCR2 = compare;
        break;
    default:
        break;
    }
}

// 设置TIM通道n的占空比
//  DutyCycle:占空比,越大占空比越大，0~100
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

// 设置TIM通道n的占空比
//  DutyCycle:占空比,越大占空比越大，0~100
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

// 设置TIM通道n的占空比
//  DutyCycle:占空比,越大占空比越大，0~100
void TIM_SetTIM8_DutyCycle_n(u8 DutyCycle, u8 n)
{
    u32 compare;
    compare = (100 - DutyCycle) * TIM8_Handler.Init.Period / 100;
    switch (n)
    {
    case 1:
        TIM8->CCR1 = compare;
        break;
    case 2:
        TIM8->CCR2 = compare;
        break;
    case 3:
        TIM8->CCR3 = compare;
        break;
    case 4:
        TIM8->CCR4 = compare;
        break;
    default:
        break;
    }
}

// 设置TIM通道n的占空比
//  DutyCycle:占空比,越大占空比越大，0~100
void TIM_SetTIM12_DutyCycle_n(u8 DutyCycle, u8 n)
{
    u32 compare;
    compare = (100 - DutyCycle) * TIM12_Handler.Init.Period / 100;
    switch (n)
    {
    case 1:
        TIM12->CCR1 = compare;
        break;
    case 2:
        TIM12->CCR2 = compare;
        break;
    default:
        break;
    }
}

// 定时器1中断服务函数
void TIM1_UP_TIM10_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}
// 定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
    // if (__HAL_TIM_GET_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE))
    // {
    //     __HAL_TIM_CLEAR_FLAG(&TIM3_Handler, TIM_FLAG_UPDATE); // 清除更新标志
    // }
}
// 定时器4中断服务函数
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM4_Handler);
}
// 定时器5中断服务函数
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM5_Handler);
}
// 定时器8BRK,12中断服务函数
void TIM8_BRK_TIM12_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM12_Handler);
}
// 定时器8up,13中断服务函数
void TIM8_UP_TIM13_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM13_Handler);
}
// 定时器8com,14中断服务函数
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM14_Handler);
}

// 回调函数，定时器中断服务函数调用
// 中断回调函数会自动清除中断标志
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&TIM2_Handler))
    {
    }
    else if (htim == (&TIM3_Handler))
    {
        OLED_Refresh();
        fps_num++;
    }
    else if (htim == (&TIM4_Handler))
    {
    }
    else if (htim == (&TIM5_Handler))
    {
    }
    else if (htim == (&TIM8_Handler))
    {
    }
    else if (htim == (&TIM12_Handler))
    {
    }
    else if (htim == (&TIM13_Handler))
    {
        usmart_dev.scan();                                              // 执行usmart扫描
        __HAL_TIM_SET_COUNTER(&USMARTTimer_HandleTypeDef, 0);           // 清空定时器的CNT
        __HAL_TIM_SET_AUTORELOAD(&USMARTTimer_HandleTypeDef, 1000 - 1); // 恢复原来的设置
    }
    else if (htim == (&TIM14_Handler))
    {
        static u16 x1ms = 0;
        u8 temp[32] = {0};

        x1ms++;

        if (x1ms % 10 == 0) // 10ms
        {
        }
        if (x1ms % 100 == 0) // 100ms
        {
            HAL_RTC_GetTime(&RTC_Handler, &RTC_TimeStruct, RTC_FORMAT_BIN);
            sprintf(temp, "Time:%02d:%02d:%02d", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
            OLED_ShowString(0, 16, temp, 16, 1, ~BACKGROUND);
            HAL_RTC_GetDate(&RTC_Handler, &RTC_DateStruct, RTC_FORMAT_BIN);
            sprintf(temp, "Date:20%02d-%02d-%02d", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Date);
            OLED_ShowString(0, 32, temp, 16, 1, ~BACKGROUND);
            sprintf(temp, "Week:%d", RTC_DateStruct.WeekDay);
            OLED_ShowString(0, 48, temp, 16, 1, ~BACKGROUND);
        }
        if (x1ms % 1000 == 0) // 1000ms
        {
            sec++;
            // fps = fps_num;
            // fps_num = 0;

            // sprintf(temp, "%dfps", fps);
            // OLED_ShowString(0, 48, temp, 16, 1, RED);
            // OLED_ShowString(0, 64, temp, 16, 1, GREEN);
            // OLED_ShowString(0, 80, temp, 16, 1, BLUE);
            sprintf(temp, "%dS", sec);
            OLED_ShowString(0, 0, temp, 16, 1, ~BACKGROUND);
        }
        if (x1ms % 10000 == 0) // 10000ms
        {
        }

        if (x1ms >= 60000)
        {
            x1ms = 0; // 归零
        }
    }
}
