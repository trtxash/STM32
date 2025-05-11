#ifndef GD32F450Z_LCD_H
#define GD32F450Z_LCD_H

#include "sys.h"
#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define LCD_WIDTH             480
#define LCD_HEIGHT            800
#define LCD_FB_BYTE_PER_PIXEL 1

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

    void lcd_disp_config(void);

#if defined(__cplusplus)
}
#endif

#endif /* GD32F450Z_LCD_H */
