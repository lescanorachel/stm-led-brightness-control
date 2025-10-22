#include <stdio.h>
#include "adc.h"
#include "tim.h"
#include "uart.h"

int potentio_value;
double brightness;

int main(void)
{
//	Initialize debug UART
	uart_init();

//	Initialize ADC
	pa1_adc_init();

//	Initialize TIM2 timer
	tim2_init();

//	Start conversion
	start_conversion();

//	Initialize timer for generation of interrupt every 1sec (or 1000msec)
	tim3_msec_init(1000);

	while(1)
	{
		potentio_value = adc_read();

		// 0-4095 possible discrete levels (12-bit ADC)
		brightness = (double) potentio_value/4095;

		// Update duty cycle of PWM based on potentiometer
		TIM2->CCR1 = ARR_VAL * (brightness);

	}
}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR & SR_UIF)
	{
		// print brightness value
		printf("Brightness: %d%%\r\n", (int)(brightness*100));

		// clear UIF flag
		TIM3->SR &= ~SR_UIF;

	}
}
