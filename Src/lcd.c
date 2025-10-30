#include "i2c.h"
#include "lcd.h"
#include "tim.h"

#define LCD_SLAVE_ADDR			(0x27)

void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_send_string(char * str);

void lcd_init(void)
{
//	Initialize I2C1
	i2c1_init();

//	Initialize TIM2 for delay generation
	tim4_blocking_delay_init();

	generate_ms_delay(45);
	lcd_send_cmd(0x30);
	generate_ms_delay(5);
	lcd_send_cmd(0x30);
	generate_ms_delay(10);
	lcd_send_cmd(0x30);
	generate_ms_delay(10);
	lcd_send_cmd(0x20); //	Configure to 4-bit mode (uses PCF8574)

	generate_ms_delay(10);
	lcd_send_cmd(0x28); // Function set - 4bits, 2 lines, 5x8
	generate_ms_delay(10);
	lcd_send_cmd(0x08); // Display off
	generate_ms_delay(10);
	lcd_send_cmd(0x01); // Display clear
	generate_ms_delay(10);
	lcd_send_cmd(0x06); // Entry mode set
	generate_ms_delay(10);
	lcd_send_cmd(0x0C); // Display on
	generate_ms_delay(10);

}

void lcd_send_cmd(char cmd)
{
	char cmd_u, cmd_l;
	cmd_u = (cmd & 0xF0); // Get the 4 upper bits
	cmd_l = ((cmd << 4) & 0xF0); // Get the 4 lower bits

	char instr_t[4]; // instruction array to transmit

//	Reset RS (0 - IR write as an internal operation); reset RW (0 - Write); set backlight (1)
//	Configure 1st nibble - D7 D6 D5 D4
	instr_t[0] = cmd_u | 0x0C; // with Enable set (1)
	instr_t[1] = cmd_u | 0x08; // with Enable set (0)
//	Configure 2nd nibble - D3 D2 D1 D0
	instr_t[2] = cmd_l | 0x0C; // with Enable set (1)
	instr_t[3] = cmd_l | 0x08; // with Enable set (0)

	i2c1_burst_write(LCD_SLAVE_ADDR, 4U, instr_t);

}

void lcd_send_data(char data)
{
	char data_u, data_l;
	data_u = (data & 0xF0); // Get the 4 upper bits
	data_l = ((data << 4) & 0xF0); // Get the 4 lower bits

	char data_t[4]; // data array to transmit

//	Reset RS (1 - IR write as an internal operation); reset RW (0 - Write); set backlight (1)
//	Configure 1st nibble - D7 D6 D5 D4
	data_t[0] = data_u | 0x0D; // with Enable set (1)
	data_t[1] = data_u | 0x09; // with Enable set (0)
//	Configure 2nd nibble - D3 D2 D1 D0
	data_t[2] = data_l | 0x0D; // with Enable set (1)
	data_t[3] = data_l | 0x09; // with Enable set (0)

	i2c1_burst_write(LCD_SLAVE_ADDR, 4U, data_t);
}

void lcd_clear_display(void)
{
	lcd_send_cmd(0x01); // Clear display
	generate_ms_delay(10);
}

void lcd_send_string(char * str)
{
//	lcd_clear_display(); // Clear display first -- to overwrite previously written longer strings
	while(*str != '\0')
	{
		lcd_send_data(*str);
		str++;
	}
}

void lcd_set_cursor(int row, int col)
{
	if(row == 0)
	{
		lcd_send_cmd(0x80 | col);
	}
	else if (row == 1)
	{
		lcd_send_cmd(0xC0 | col);
	}
	generate_ms_delay(10);
}



