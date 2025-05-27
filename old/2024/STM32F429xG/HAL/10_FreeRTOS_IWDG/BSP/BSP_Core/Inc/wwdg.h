#ifndef __WWDG_H__
#define __WWDG_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define WWDG_CNT 0X7F // 保存WWDG计数器的设置值，默认为最大

  extern WWDG_HandleTypeDef hwwdg;

  void WWDG_Init(uint8_t tr, uint8_t wr, uint32_t fprer);

#ifdef __cplusplus
}
#endif

#endif /* __WWDG_H__ */
