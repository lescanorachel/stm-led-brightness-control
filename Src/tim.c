#include "tim.h"

#define GPIOAEN			(1U<<0)
#define TIM2EN			(1U<<0)
#define TIM3EN			(1U<<1)
#define TIM4EN			(1U<<2)
#define CR1_CEN			(1U<<0)

#define CCMR1_OC1M		(1U<<3)
#define CR1_ARPE		(1U<<7)
#define EGR_UG			(1U<<0)
#define CCER_CC1P		(1U<<1)
#define CCER_CC1EEN		(1U<<0)
#define CR1_URS			(1U<<2)
#define DIER_UIE		(1U<<0)

void tim2_init(void)
{
//	Configure TIM2_CH1 GPIO Pin
//	Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

//	Set PA0 mode to alternate function mode
	GPIOA->MODER &= ~(1U<<0);
	GPIOA->MODER |= (1U<<1);

//	Choose AF type for PA0 - AF01 (TIM2_CH1)
	GPIOA->AFR[0] |= (1U<<0);
	GPIOA->AFR[0] &= ~(1U<<1);
	GPIOA->AFR[0] &= ~(1U<<2);
	GPIOA->AFR[0] &= ~(1U<<3);

//	Enable clock access to TIM2
	RCC->APB1ENR |= TIM2EN;

//	Set prescaler value
	TIM2->PSC = 16 - 1; // 16MHz to 1MHz
//	Set auto-reload value
	TIM2->ARR = ARR_VAL - 1; // 1MHz to 10kHz (assuming ARR_VAL is 100)

//	Configure channel 1
//	Set initial CCR1 to 50% of ARR
	TIM2->CCR1 = ARR_VAL / 2;

//	Set channel 1 to PWM mode 1
	TIM2->CCMR1 &= ~(1U<<4);
	TIM2->CCMR1 |= (1U<<5);
	TIM2->CCMR1 |= (1U<<6);

//	Enable preload register
	TIM2->CCMR1 |= CCMR1_OC1M;
//	Enable auto-reload preload register
	TIM2->CR1 |= CR1_ARPE;

//	Set URS - do not generate interrupt when UG bit is set in EGR
	TIM2->CR1 |= CR1_URS;

//	Initialize counter and update registers
	TIM2->EGR |= EGR_UG;

//	Ensure channel 1 is active high
	TIM2->CCER &= ~CCER_CC1P;

//	Enable OC1 to PA0
	TIM2->CCER |= CCER_CC1EEN;

//	Enable timer
	TIM2->CR1 |= CR1_CEN;

}


void tim3_msec_init(uint16_t msec_delay)
{
//	Enable clock access for TIM3
	RCC->APB1ENR |= TIM3EN;

//	Set prescaler value
	TIM3->PSC = 16000 - 1; // 16MHz to 1kHz
//	Set auto-reload value
	TIM3->ARR = msec_delay - 1; // 1kHz to 1Hz (1 sec)

//	Enable interrupt flag
	TIM3->DIER |= DIER_UIE;

//	Set URS - do not generate interrupt when UG bit is set in EGR
	TIM3->CR1 |= CR1_URS;

//	Initialize counter and update registers
	TIM3->EGR |= EGR_UG;

//	Enable timer
	TIM3->CR1 |= CR1_CEN;

//	Enable NVIC interrupt for TIM3
	NVIC_EnableIRQ(TIM3_IRQn);
}

void tim4_blocking_delay_init(void)
{
//	Enable clock access to TIM2
	RCC->APB1ENR |= TIM4EN;

//	Set prescaler value
	TIM4->PSC = 16000 - 1; // 16MHz to 1kHz
//	Set initial auto-reload value
	TIM4->ARR = 1 - 1; // 1kHz to 1kHz (1ms)

//	Set URS - do not generate interrupt when UG bit is set in EGR
	TIM4->CR1 |= CR1_URS;

//	Initialize counter and update registers
	TIM4->EGR |= EGR_UG;

//	Make sure UIF is reset
	TIM4->SR &= ~(SR_UIF);

//	Disable timer on init
	TIM4->CR1 &= ~CR1_CEN;

}

void generate_ms_delay(uint16_t msec_delay)
{
//	Set auto-reload value
	TIM4->ARR = msec_delay - 1;

//	Initialize counter and update registers
	TIM4->EGR |= EGR_UG;

//	Make sure UIF is reset
	TIM4->SR &= ~(SR_UIF);

//	Enable timer
	TIM4->CR1 |= CR1_CEN;

	while(!(TIM4->SR & SR_UIF)){}

//	Clear UIF and disable timer
	TIM4->SR &= ~(SR_UIF);
	TIM4->CR1 &= ~CR1_CEN;

}
