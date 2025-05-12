/* Includes ------------------------------------------------------------------*/
#include "ltdc.h"
#include "main.h"
#include "sdram.h"
#include "tim.h"

/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

/* HAL_FMC+HAL_SDRAM */

void HAL_FMC_MspInit(void)
{
    /* USER CODE BEGIN FMC_MspInit 0 */

    /* USER CODE END FMC_MspInit 0 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /** FMC GPIO Configuration
    PF0   ------> FMC_A0
    PF1   ------> FMC_A1
    PF2   ------> FMC_A2
    PF3   ------> FMC_A3
    PF4   ------> FMC_A4
    PF5   ------> FMC_A5
    PC0   ------> FMC_SDNWE
    PC2   ------> FMC_SDNE0
    PC3   ------> FMC_SDCKE0
    PF11   ------> FMC_SDNRAS
    PF12   ------> FMC_A6
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PF15   ------> FMC_A9
    PG0   ------> FMC_A10
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG2   ------> FMC_A12
    PG4   ------> FMC_BA0
    PG5   ------> FMC_BA1
    PG8   ------> FMC_SDCLK
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PG15   ------> FMC_SDNCAS
    */
    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* GPIO_InitStruct */
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;

    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USER CODE BEGIN FMC_MspInit 1 */

    /* USER CODE END FMC_MspInit 1 */
}

void HAL_FMC_MspDeInit(void)
{
    /* USER CODE BEGIN FMC_MspDeInit 0 */

    /* USER CODE END FMC_MspDeInit 0 */

    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_DISABLE();

    /** FMC GPIO Configuration
    PF0   ------> FMC_A0
    PF1   ------> FMC_A1
    PF2   ------> FMC_A2
    PF3   ------> FMC_A3
    PF4   ------> FMC_A4
    PF5   ------> FMC_A5
    PC0   ------> FMC_SDNWE
    PC2   ------> FMC_SDNE0
    PC3   ------> FMC_SDCKE0
    PF11   ------> FMC_SDNRAS
    PF12   ------> FMC_A6
    PF13   ------> FMC_A7
    PF14   ------> FMC_A8
    PF15   ------> FMC_A9
    PG0   ------> FMC_A10
    PG1   ------> FMC_A11
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PG2   ------> FMC_A12
    PG4   ------> FMC_BA0
    PG5   ------> FMC_BA1
    PG8   ------> FMC_SDCLK
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PG15   ------> FMC_SDNCAS
    */

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1);

    /* USER CODE BEGIN FMC_MspDeInit 1 */

    /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *sdramHandle)
{
    /* USER CODE BEGIN SDRAM_MspInit 0 */

    /* USER CODE END SDRAM_MspInit 0 */
    HAL_FMC_MspInit();
    /* USER CODE BEGIN SDRAM_MspInit 1 */

    /* USER CODE END SDRAM_MspInit 1 */
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *sdramHandle)
{
    /* USER CODE BEGIN SDRAM_MspDeInit 0 */

    /* USER CODE END SDRAM_MspDeInit 0 */
    HAL_FMC_MspDeInit();
    /* USER CODE BEGIN SDRAM_MspDeInit 1 */

    /* USER CODE END SDRAM_MspDeInit 1 */
}

// 25MHz clock for LTDC
void HAL_LTDC_MspInit(LTDC_HandleTypeDef *ltdcHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    if (ltdcHandle->Instance == LTDC)
    {
        /* USER CODE BEGIN LTDC_MspInit 0 */

        /* USER CODE END LTDC_MspInit 0 */

        /** Initializes the peripherals clock
         */
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
        PeriphClkInitStruct.PLLSAI.PLLSAIN = 50;
        PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
        PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2; // 24/12*50/2/2=25MHz
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
            Error_Handler();
        }

        /* LTDC clock enable */
        __HAL_RCC_LTDC_CLK_ENABLE();

        __HAL_RCC_GPIOF_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOG_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();
        /**LTDC GPIO Configuration
        PF10     ------> LTDC_DE
        PA3     ------> LTDC_B5
        PA4     ------> LTDC_VSYNC
        PA6     ------> LTDC_G2
        PB0     ------> LTDC_R3
        PB10     ------> LTDC_G4
        PB11     ------> LTDC_G5
        PG6     ------> LTDC_R7
        PG7     ------> LTDC_CLK
        PC6     ------> LTDC_HSYNC
        PC7     ------> LTDC_G6
        PA8     ------> LTDC_R6
        PA11     ------> LTDC_R4
        PA12     ------> LTDC_R5
        PD3     ------> LTDC_G7
        PG10     ------> LTDC_G3
        PG11     ------> LTDC_B3
        PG12     ------> LTDC_B4
        PB8     ------> LTDC_B6
        PB9     ------> LTDC_B7
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_12;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF9_LTDC;
        HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

        /* USER CODE BEGIN LTDC_MspInit 1 */

        /* USER CODE END LTDC_MspInit 1 */
    }
}

void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *ltdcHandle)
{

    if (ltdcHandle->Instance == LTDC)
    {
        /* USER CODE BEGIN LTDC_MspDeInit 0 */

        /* USER CODE END LTDC_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_LTDC_CLK_DISABLE();

        /**LTDC GPIO Configuration
        PF10     ------> LTDC_DE
        PA3     ------> LTDC_B5
        PA4     ------> LTDC_VSYNC
        PA6     ------> LTDC_G2
        PB0     ------> LTDC_R3
        PB10     ------> LTDC_G4
        PB11     ------> LTDC_G5
        PG6     ------> LTDC_R7
        PG7     ------> LTDC_CLK
        PC6     ------> LTDC_HSYNC
        PC7     ------> LTDC_G6
        PA8     ------> LTDC_R6
        PA11     ------> LTDC_R4
        PA12     ------> LTDC_R5
        PD3     ------> LTDC_G7
        PG10     ------> LTDC_G3
        PG11     ------> LTDC_B3
        PG12     ------> LTDC_B4
        PB8     ------> LTDC_B6
        PB9     ------> LTDC_B7
        */
        HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);

        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_9);

        HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);

        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6 | GPIO_PIN_7);

        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_3);

        /* USER CODE BEGIN LTDC_MspDeInit 1 */

        /* USER CODE END LTDC_MspDeInit 1 */
    }
}

void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *dma2dHandle)
{

    if (dma2dHandle->Instance == DMA2D)
    {
        /* USER CODE BEGIN DMA2D_MspInit 0 */

        /* USER CODE END DMA2D_MspInit 0 */
        /* DMA2D clock enable */
        __HAL_RCC_DMA2D_CLK_ENABLE();

        HAL_NVIC_SetPriority(DMA2D_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(DMA2D_IRQn);
        /* USER CODE BEGIN DMA2D_MspInit 1 */

        /* USER CODE END DMA2D_MspInit 1 */
    }
}

void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *dma2dHandle)
{

    if (dma2dHandle->Instance == DMA2D)
    {
        /* USER CODE BEGIN DMA2D_MspDeInit 0 */

        /* USER CODE END DMA2D_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DMA2D_CLK_DISABLE();
        /* USER CODE BEGIN DMA2D_MspDeInit 1 */

        /* USER CODE END DMA2D_MspDeInit 1 */
    }
}

/**
 * @brief TIM_Base MSP Initialization
 * This function configures the hardware resources used in this example
 * @param tim_baseHandle: TIM_Base handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{
    if (tim_baseHandle->Instance == TIM14)
    {
        __HAL_RCC_TIM14_CLK_ENABLE();

        /* TIM6 interrupt Init */
        HAL_NVIC_SetPriority(TIM8_TRG_COM_TIM14_IRQn, 4, 0);
        HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
        /* USER CODE BEGIN TIM6_MspInit 1 */

        /* USER CODE END TIM6_MspInit 1 */
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *tim_baseHandle)
{

    if (tim_baseHandle->Instance == TIM14)
    {
        __HAL_RCC_TIM14_CLK_DISABLE();
    }
}

// void HAL_WWDG_MspInit(WWDG_HandleTypeDef *wwdgHandle)
// {

//   if (wwdgHandle->Instance == WWDG)
//   {
//     __HAL_RCC_WWDG_CLK_ENABLE();

//     HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
//     HAL_NVIC_EnableIRQ(WWDG_IRQn);
//   }
// }
