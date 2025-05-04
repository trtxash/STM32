#include "wwdg.h"

WWDG_HandleTypeDef hwwdg;

/*
	WWDG init function
	wdgtb	最小超时us	最大超时ms
	0		91.02		5.83
	1		182.04		11.65
	2		364.06		23.3
	3		728.18		46.6
	tr :T[6:0],计数器值wr :W[6:0],窗口值 fprer:分频系数（WDGTB）,仅最低2位有效
	Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=45Mhz
*/
void WWDG_Init(uint8_t tr, uint8_t wr, uint32_t fprer)
{
	hwwdg.Instance = WWDG;
	hwwdg.Init.Prescaler = fprer;
	hwwdg.Init.Window = wr;
	hwwdg.Init.Counter = tr;
	hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
	if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
	{
		Error_Handler();
	}
}