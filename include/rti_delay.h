

#ifndef INCLUDE_RTI_DELAY_H_
#define INCLUDE_RTI_DELAY_H_
 
#include "rti.h"
 
uint32 rtiGetMyTick(uint32 compare);
void rtiDelay_ms(uint32_t msec);
void rtiDelay_us(uint32_t msec);
 
#endif /* INCLUDE_RTI_DELAY_H_ */