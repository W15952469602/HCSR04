#ifndef __HCSR04__
#define __HCSR04__

#include "stdint.h"

//Timer interval >= 1 millisecond

typedef struct {
	uint16_t timeCount = 0;
	uint8_t state : 4;
	uint8_t readFlag : 2;
	uint8_t echoPin : 1;
	uint8_t echoPin_r : 1;
	void (*WriteTrig)(uint8_t);
	uint8_t (*ReadEcho)(void);
	void (*callback)(void);
} HCSR04_t;

void HCSR04_Init(void (*WriteTrig)(uint8_t), uint8_t (*ReadEcho)(void));
void HCSR04_AddCallback(void (*callback)(void));
void HCSR04_TimerHandle(void);
void HCSR04_StartCount(void);
uint16_t HCSR04_GetTimecount(void);
uint16_t HCSR04_ReadPolling(void);

#endif