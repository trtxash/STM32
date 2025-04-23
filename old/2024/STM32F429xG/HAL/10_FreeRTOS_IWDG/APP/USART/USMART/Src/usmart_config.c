#include "usmart.h"
#include "usmart_str.h"
////////////////////////////用户配置区///////////////////////////////////////////////
// 这下面要包含所用到的函数所申明的头文件(用户自己添加)
#include "delay.h"
#include "sys.h"

// #include "ws2812.h"

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

		// (void *) WS_WriteAll_RGB,
		// "void WS_WriteAll_RGB(u8 n_R, u8 n_G, u8 n_B)",
		// (void *) WS_CloseAll,
		// "void WS_CloseAll(void)",
		// (void *) WS281x_SetPixelColor,
		// "void WS281x_SetPixelColor(uint16_t n, u32 GRBColor)",
		// (void *) WS281x_SetPixelRGB,
		// "void WS281x_SetPixelRGB(uint16_t n, u8 red, u8 green, u8 blue)",
		// (void *) rainbow,
		// "void rainbow(u8 wait)",
		// (void *) rainbowCycle,
		// "void rainbowCycle(u8 wait)",
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
