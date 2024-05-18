#include "sys.h"

// 时钟系统配置函数
//  Fvco=Fs*(plln/pllm);
//  SYSCLK=Fvco/pllp=Fs*(plln/(pllm*pllp));
//  Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));
//  Fvco:VCO频率
//  SYSCLK:系统时钟频率
//  Fusb:USB,SDIO,RNG等的时钟频率
//  Fs:PLL输入时钟频率,可以是HSI,HSE等.
//  plln:主PLL倍频系数(PLL倍频)
//  pllm:主PLL和音频PLL分频系数(PLL之前的分频)
//  pllp:系统时钟的主PLL分频系数(PLL之后的分频)
//  pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
// 外部晶振为8M的时候（f407）,推荐值:plln=168,pllm=4,pllp=RCC_PLLP_DIV2,pllq=4,ahb=RCC_SYSCLK_DIV1,apb1=RCC_HCLK_DIV4,apb2=RCC_HCLK_DIV2
// 返回值:0,成功;1,失败
void Stm32_Clock_Init(u32 plln, u32 pllm, u32 pllp, u32 pllq)
{
    RCC_OscInitTypeDef RCC_OscInitStructure = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStructure = {0};

    __HAL_RCC_PWR_CLK_ENABLE(); // 使能PWR时钟

    // 下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    // 时使性能与功耗实现平衡。
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1); // 设置调压器输出电压级别1

    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE; // 时钟源为HSE
    RCC_OscInitStructure.HSEState = RCC_HSE_ON;                   // 打开HSE
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;               // 打开PLL
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE;       // PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM = pllm;                         // 主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    RCC_OscInitStructure.PLL.PLLN = plln;                         // 主PLL倍频系数(PLL倍频),取值范围:64~432.
    RCC_OscInitStructure.PLL.PLLP = pllp;                         // 系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    RCC_OscInitStructure.PLL.PLLQ = pllq;                         // USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
    if (HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK)
    {
        Error_Handler();
    }

    /** Activate the Over-Drive mode
     */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK)
    {
        Error_Handler();
    }

    // 选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStructure.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStructure.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStructure.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }

    // // STM32F405x/407x/415x/417x Z版本的器件支持预取功能
    // if (HAL_GetREVID() == 0x1001)
    // {
    //     __HAL_FLASH_PREFETCH_BUFFER_ENABLE(); // 使能flash预取
    // }
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
// 当编译提示出错的时候此函数用来报告错误的文件和所在行
//  file：指向源文件
//  line：指向在文件中的行数
void assert_failed(uint8_t *file, uint32_t line)
{
    while (1)
    {
    }
}
#endif

// THUMB指令不支持汇编内联
// 采用如下方法实现执行汇编指令WFI
void WFI_SET(void)
{
    __ASM volatile("WFI");
}
// 关闭所有中断(但是不包括fault和NMI中断)
void INTX_DISABLE(void)
{
    __ASM volatile("CPSID I");
    __ASM volatile("BX LR");
}
// 开启所有中断
void INTX_ENABLE(void)
{
    __ASM volatile("CPSIE I");
    __ASM volatile("BX LR");
}
// 设置栈顶地址
//  addr:栈顶地址
void MSR_MSP(u32 addr)
{
    __ASM volatile("MSR MSP, r0"); // set Main Stack value
    __ASM volatile("BX r14");
}

/*下面原先文件，但是会报错*/

// __ASM void WFI_SET(void)
// {
// 	WFI;
// }
// //关闭所有中断(但是不包括fault和NMI中断)
// __asm void INTX_DISABLE(void)
// {
// 	CPSID   I
// 	BX      LR
// }
// //开启所有中断
// __asm void INTX_ENABLE(void)
// {
// 	CPSIE   I
// 	BX      LR
// }
// //设置栈顶地址
// //addr:栈顶地址
// __asm void MSR_MSP(u32 addr)
// {
// 	MSR MSP, r0 			//set Main Stack value
// 	BX r14
// }
