#ifndef _INFRAREDTOBE_H
#define _INFRAREDTOBE_H
#include "sys.h"
#include "usart.h"

extern RxPack rxpack;

void Infraredtobe_Init(void);
u8 Read_Infraredtobe_bits(void);
int Read_Infraredtobe_sums(void);

#endif