#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
// 这下面要包含所用到的函数所申明的头文件(用户自己添加)
#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "led.h"
#include "rtc.h"

// 函数名列表初始化(用户自己添加)
// 用户直接在这里输入要执行的函数名及其查找串
struct _m_usmart_nametab usmart_nametab[] =
	{
#if USMART_USE_WRFUNS == 1 // 如果使能了读写操作
		(void *)read_addr,
		"u32 read_addr(u32 addr)",
		(void *)write_addr,
		"void write_addr(u32 addr,u32 val)",
#endif
		(void *)delay_ms,
		"void delay_ms(u16 nms)",
		(void *)delay_xms,
		"void delay_xms(u32 nms)",
		(void *)delay_us,
		"void delay_us(u32 nus)",
		(void *)OLED_Clear,
		"void OLED_Clear(void)",
		(void *)OLED_DrawPoint,
		"void OLED_DrawPoint(u16 x, u8 y, u8 t, u16 color)",
		(void *)OLED_DrawLine,
		"void OLED_DrawLine(u16 x1, u8 y1, u16 x2, u8 y2, u8 mode, u16 color)",
		(void *)OLED_DrawCircle,
		"void OLED_DrawCircle(u16 x, u8 y, u8 r, u16 color)",
		(void *)OLED_ShowChar,
		"void OLED_ShowChar(u16 x, u8 y, u8 chr, u8 size1, u8 mode, u16 color)",
		(void *)OLED_ShowString,
		"void OLED_ShowString(u16 x, u8 y, u8 *chr, u8 size1, u8 mode, u16 color)",
		(void *)OLED_ShowNum,
		"void OLED_ShowNum(u16 x, u8 y, u32 num, u8 len, u8 size1, u8 mode, u16 color)",
		(void *)OLED_ShowChinese,
		"void OLED_ShowChinese(u16 x, u8 y, u8 num, u8 size1, u8 mode, u16 color)",

		(void *)LED0_Reverse,
		"void LED0_Reverse(void)",
		(void *)LED1_Reverse,
		"void LED1_Reverse(void)",

		(void *)RTC_Set_Time,
		"HAL_StatusTypeDef RTC_Set_Time(u8 hour, u8 min, u8 sec, u8 ampm)",
		(void *)RTC_Set_Date,
		"HAL_StatusTypeDef RTC_Set_Date(u8 year, u8 month, u8 date, u8 week)",
		(void *)RTC_Set_AlarmA,
		"void RTC_Set_AlarmA(u8 week, u8 hour, u8 min, u8 sec)",
		(void *)RTC_Set_WakeUp,
		"void RTC_Set_WakeUp(u32 wksel, u16 cnt)",
};
///////////////////////////////////END///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
// 函数控制管理器初始化
// 得到各个受控函数的名字
// 得到函数总数量
struct _m_usmart_dev usmart_dev =
	{
		usmart_nametab,
		usmart_init,
		usmart_cmd_rec,
		usmart_exe,
		usmart_scan,
		sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), // 函数数量
		0,														   // 参数数量
		0,														   // 函数ID
		1,														   // 参数显示类型,0,10进制;1,16进制
		0,														   // 参数类型.bitx:,0,数字;1,字符串
		0,														   // 每个参数的长度暂存表,需要MAX_PARM个0初始化
		0,														   // 函数的参数,需要PARM_LEN个0初始化
};
