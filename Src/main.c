#include <stdio.h>
#include "adc.h"
#include "lcd.h"
#include "tim.h"
#include "uart.h"

int potentio_value;
double brightness;
char lcd_str[16];

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

//	Initialize LCD 16x2
	lcd_init();

	while(1)
	{
		potentio_value = adc_read();

		// 0-4095 possible discrete levels (12-bit ADC)
		brightness = (double) potentio_value/4095;

		// Update duty cycle of PWM based on potentiometer
		TIM2->CCR1 = ARR_VAL * (brightness);

	}
}

void generate_brightness_bars(void)
{
//	Compute the number of  bars to fill in the LCD
	int bars = (int) ((brightness*100) / 6.25);
	for (int i = 0; i < 16; i++)
	{
		if(i < bars)
		{
			//	Write fill
			lcd_str[i] = (char) 0xFF;
		}
		else
		{
			//	Write blank
			lcd_str[i] = (char) 0xFE;
		}
	}

}

void TIM3_IRQHandler(void)
{
	if(TIM3->SR & SR_UIF)
	{
		// print brightness value (UART)
		printf("Brightness: %d%%\r\n", (int)(brightness*100));

		// print to LCD (I2C)
		lcd_set_cursor(0, 0);
		sprintf(lcd_str, "Brightness: %d%% ", (int)(brightness*100));
		lcd_send_string(lcd_str);

		lcd_set_cursor(1, 0);
		generate_brightness_bars();
		lcd_send_string(lcd_str);

		// clear UIF flag
		TIM3->SR &= ~SR_UIF;

	}
}
