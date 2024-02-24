#ifndef TIMER_H
#define TIMER_H

#include "stm32l476xx.h"

class TimerHelpers
{
    public:
    TimerHelpers() = delete;
    static void SetupTIM4(void);
    static void delay_ms(uint32_t ms);
};


#endif