#ifndef LCD1602_H__
#define LCD1602_H__

#include "stm32f4xx.h"
void lcd_init(void);
void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_send_string(char * str);
void lcd_set_cursor(int row, int col);

#endif
