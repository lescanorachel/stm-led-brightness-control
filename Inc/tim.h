#ifndef TIM_H_
#define TIM_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define ARR_VAL		100
#define SR_UIF		(1U<<0)

void tim2_init(void);
void tim3_msec_init(uint16_t msec_delay);
void tim4_blocking_delay_init(void);
void generate_ms_delay(uint16_t msec_delay);

#endif
