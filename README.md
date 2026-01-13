# ADC-Controlled PWM LED Brightness System on STM32 with UART and LCD Output
This project demonstrates the use of ADC and TIM peripherals to control LED brightness level by varying the duty cycle of the generated PWM signal using a potentiometer. UART and I2C are also configured to simply display the brightness level every second to a terminal and to an LCD 16x2 display, respectively.

## Peripherals Used
### ADC
ADC is configured to accept and process the values provided by the potentiometer to use as basis for the control of LED's brightness.

### I2C
I2C is configured and used to communicate with an LCD 16x2 display. LCD is used to display the brightness level (both as text on the first line and represented as bars on the second line) simultaneous with the display using UART.

### UART
UART is the communication protocol used to transmit the current brightness level to the computer terminal for displaying purposes which is triggered every second.

### TIM
TIM2, TIM3, and TIM4 are configured for different purposes. TIM2 peripheral is used to generate the PWM output to drive the LED. On the other hand, TIM3 is used to generate the timer responsible for triggering an interrupt every second to control display interval while TIM4 is used to generate blocking delays in milliseconds.

## How to Use
Simply adjust the knob of the potentiometer to control the LED's brightness. Brightness level display on the computer terminal and LCD are shown and updated every second.


