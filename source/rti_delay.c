
#include "rti_delay.h"

uint32 rtiGetMyTick(uint32 compare)
{
    uint32 tick;
    uint32 counter = ((rtiREG1->COMPCTRL & (1U << (compare << 2U))) !=0U ) ? 1U : 0U;
    /* USER CODE BEGIN (27) */
    tick = rtiREG1->CNT[counter].FRCx;
    /* USER CODE END */
    return tick;
}
 
void rtiDelay_ms(uint32_t msec)
{
    uint32_t curTick = rtiGetMyTick(rtiCOMPARE0);
    uint32_t endTick = curTick + msec * 1000 * 10; /* *1000 means 1ms */
    while( curTick < endTick)
    {
        curTick = rtiGetMyTick(rtiCOMPARE0);
    }
}
 
void rtiDelay_us(uint32_t msec)
{
    uint32_t curTick = rtiGetMyTick(rtiCOMPARE0);
    uint32_t endTick = curTick + msec * 1 * 10; /* *1 means 1us */
    while( curTick < endTick)
    {
        curTick = rtiGetMyTick(rtiCOMPARE0);
    }
}