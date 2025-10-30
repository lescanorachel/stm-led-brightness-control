#include "i2c.h"

#define GPIOBEN 			(1U<<1)
#define I2C1EN 				(1U<<21)

#define I2C1_CR2_FREQ		(1U<<4) // 16Mhz
#define I2C1_CCR			80		// 100kHz - Standard mode
#define I2C1_TRISE			17		// 16 + 1
#define CR1_PE				(1U<<0)

#define CR1_START			(1U<<8)
#define CR1_STOP			(1U<<9)
#define CR1_ACK				(1U<<10)
#define CR1_SWRST			(1U<<15)

#define SR1_SB				(1U<<0)
#define SR1_ADDR			(1U<<1)
#define SR1_BTF				(1U<<2)
#define SR1_RXNE			(1U<<6)
#define SR1_TXE				(1U<<7)
#define SR2_BUSY			(1U<<1)

void i2c1_init(void)
{
//	Configure GPIO
//	Enable clock access to GPIOB
	RCC->AHB1ENR |= GPIOBEN;

//	Set PB8 (I2C1_SCL) and PB9 (I2C1_SDA) to AF mode
	GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);

	GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);

//	Set PB8 and PB9 to AF04 (I2C)
	GPIOB->AFR[1] &= ~(1U<<0);
	GPIOB->AFR[1] &= ~(1U<<1);
	GPIOB->AFR[1] |= (1U<<2);
	GPIOB->AFR[1] &= ~(1U<<3);

	GPIOB->AFR[1] &= ~(1U<<4);
	GPIOB->AFR[1] &= ~(1U<<5);
	GPIOB->AFR[1] |= (1U<<6);
	GPIOB->AFR[1] &= ~(1U<<7);

//	Set PB8 and PB9 as open-drain
	GPIOB->OTYPER |= (1U<<8);
	GPIOB->OTYPER |= (1U<<9);

//	Enable pull up for PB8 and PB9 pin
	GPIOB->PUPDR |= (1U<<16);
	GPIOB->PUPDR &= ~(1U<<17);

	GPIOB->PUPDR |= (1U<<18);
	GPIOB->PUPDR &= ~(1U<<19);

//	Configure I2C1
//	Enable clock access to I2C1
	RCC->APB1ENR |= I2C1EN;

//	Enter I2C1 to reset mode
	I2C1->CR1 |= (1U<<15);
//	Exit I2C1 from reset mode
	I2C1->CR1 &= ~(1U<<15);

//	I2C Peripheral Clock Frequency
	I2C1->CR2 = I2C1_CR2_FREQ;

//	Set I2C1 to standard mode, 100kHz
	I2C1->CCR = I2C1_CCR;

//	Set maximum rise time
	I2C1->TRISE = I2C1_TRISE;

//	Enable the I2C1 peripheral
	I2C1->CR1 |= CR1_PE;

}

void i2c1_burst_write(char slave_addr, int n, char * data)
{
	volatile int tmp;

//	Wait until bus is not busy
	while(I2C1->SR2 & SR2_BUSY){}

//	Generate start condition
	I2C1->CR1 |= CR1_START;

//	Wait until SB bit is set
	while (!(I2C1->SR1 & SR1_SB)){}

//	Transmit slave address + LSB reset (Enter transmitter mode)
	I2C1->DR = slave_addr << 1;

//	Wait until ADDR bit is set
	while (!(I2C1->SR1 & SR1_ADDR)){}

//	Clear ADDR flag by reading SR2 too
	tmp = I2C1->SR2;

	for (int i = 0; i < n; i++)
	{
		//	Wait until TX is empty
		while (!(I2C1->SR1 & SR1_TXE)){}

		//	Send the data byte
		I2C1->DR = data[i];
	}

//	Wait until transfer is finished
	while (!(I2C1->SR1 & SR1_BTF)){}

	//	Generate stop condition
	I2C1->CR1 |= CR1_STOP;
}
