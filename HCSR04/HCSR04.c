#include "HCSR04.h"

static HCSR04_t DevHandle;

void HCSR04_Init(void (*WriteTrig)(uint8_t), uint8_t (*ReadEcho)(void))
{
    DevHandle.state = 0;
    DevHandle.readFlag = 1;
    DevHandle.echoPin = 0;
    DevHandle.echoPin_r = 0;
    DevHandle.WriteTrig = WriteTrig;
    DevHandle.ReadEcho = ReadEcho;
}

void HCSR04_AddCallback(void (*callback)(void))
{
    DevHandle.callback = callback;
}

void HCSR04_TimerHandle()
{
    DevHandle.echoPin_r = DevHandle.echoPin;
    DevHandle.echoPin = DevHandle.ReadEcho();
    if (DevHandle.readFlag == 0)
    {
        switch (DevHandle.state)
        {
        case 0:
            DevHandle.WriteTrig(1);
            DevHandle.state = 1;
            break;
        case 1:
            DevHandle.WriteTrig(0);
            DevHandle.state = 2;
            DevHandle.timeCount = 0;
            break;
        case 2:
            if ((DevHandle.echoPin_r == 0) && (DevHandle.echoPin == 1) && (DevHandle.timeCount < 500))
            {
                DevHandle.state = 3;
                DevHandle.timeCount = 0;
            }
            else if (DevHandle.timeCount > 500)
            {
                DevHandle.readFlag = 2;
                DevHandle.state = 0;
                DevHandle.timeCount = 65535;
                if (DevHandle.callback)
                    DevHandle.callback();
            }
            else
            {
                DevHandle.timeCount++;
            }
            break;
        case 3:
            DevHandle.timeCount++;
            if ((DevHandle.echoPin_r == 1) & (DevHandle.echoPin == 0))
            {
                DevHandle.readFlag = 1;
                DevHandle.state = 0;
                if (DevHandle.callback)
                    DevHandle.callback();
            }
            break;
        default:
            break;
        }
    }
}

void HCSR04_StartCount()
{
    DevHandle.readFlag = 0;
}

uint16_t HCSR04_GetTimecount(void)
{
    return DevHandle.timeCount;
}

uint16_t HCSR04_ReadPolling()
{
    DevHandle.readFlag = 0;
    while (DevHandle.readFlag != 0)
        ;
    return DevHandle.timeCount;
}