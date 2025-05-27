#ifndef __MOTORENCODER_H 
#define __MOTORENCODER_H 
#include "sys.h" 

void MotorEncoder_Init(void); 
int Read_Encoder(void);
void Encoder_TIM2(u16 arr, u16 psc);
#endif
