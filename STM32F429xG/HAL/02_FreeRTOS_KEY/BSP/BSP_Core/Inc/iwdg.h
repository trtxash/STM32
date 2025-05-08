#ifndef __IWDG_H__
#define __IWDG_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

    extern IWDG_HandleTypeDef hiwdg;

    void IWDG_Init(uint8_t prer, uint16_t rlr);
    void IWDG_Feed(void);

#ifdef __cplusplus
}
#endif

#endif /* __IWDG_H__ */
