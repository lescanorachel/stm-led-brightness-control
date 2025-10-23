# STM32 LED Brightness Control
This project demonstrates the use of ADC and TIM peripherals to control LED brightness level by varying the duty cycle of the generated PWM signal using a potentiometer. UART is also configured to simply display the brightness level every second.

## Peripherals Used
### ADC
ADC is configured to accept and process the values provided by the potentiometer to use as basis for the control of LED's brightness.

### TIM
TIM2 and TIM3 are configured for different purposes. TIM2 peripheral is used to generate the PWM output to drive the LED. On the other hand, TIM3 is used to generate the timer responsible for triggering an interrupt every second.  

### UART
UART is the communication protocol used to transmit the current brightness level to the computer terminal for displaying purposes which is triggered every second.

## How to Use
Simply adjust the knob of the potentiometer to control the LED's brightness.


