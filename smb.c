#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>
#include <stddef.h>

#include "smb.h"
#include "i2c_master_min.h"

char *__null__str  = "";
uint16_t __tmp__;

uint8_t I2C_ADDR = 0x40;

void inline __delay_loop__(uint16_t __count){
	__asm__ volatile (
	"1: sbiw %0,1" "\n\t"
	"brne 1b"
	: "=w" (__count)
	: "0" (__count)
	);
}

void __create_command(uint8_t command, const char *format, uint8_t parSize, const uint8_t const *par) {
	i2c_master_start(I2C_ADDR);
	i2c_master_write(command);
	i2c_master_write(parSize);
	while(parSize--) {
		i2c_master_write(*par++);
	}
	do {
		i2c_master_write(*format);
	}while(*format++);
	i2c_master_stop();
}


uint8_t smb_ready(void) {
	i2c_master_start(I2C_ADDR + I2C_READ);
	return i2c_master_readNak();
}

#define __NCYCLES__(us)		(us * (F_CPU / 1000000UL))

void smb_init(void) {
	uint8_t	f;

	SDADDR |= (1 << SDAPIN);
	SCLDDR |= (1 << SCLPIN);
	for(f = 0; f < 20 ; f++) {
		SCLPORT &= ~(1 << SCLPIN);
		SDAPORT &= ~(1 << SDAPIN);
		__delay_loop__(__NCYCLES__(10) >> 2);
		SCLPORT |= (1 << SCLPIN);
		SDAPORT |= (1 << SDAPIN);
		__delay_loop__(__NCYCLES__(10) >> 2);
	}
}

void smb_prints(char *string)				{while(smb_ready());__create_command(__PRINTS,(const char *)(string),0,__null__str);}
void smb_goto(uint8_t x, uint8_t y)			{while(smb_ready()); __tmp__ = ((x) << 8) | (y);__create_command(__DISP_GOTO,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
void smb_cls(void)							{while(smb_ready()); __create_command(__DISP_CLR,__null__str,0,__null__str);}
void smb_init_lcd(void)						{__create_command(__DISP_INIT,__null__str,0,__null__str);}
void smb_bar_init(void)						{__create_command(__INIT_BAR,__null__str,0,__null__str);}
void smb_shift_left(void)					{while(smb_ready()); __create_command(__SHIFT_LEFT,__null__str,0,__null__str);}
void smb_shift_right(void)					{while(smb_ready()); __create_command(__SHIFT_RIGHT,__null__str,0,__null__str);}
void smb_i2c_addr(uint8_t a)				{__tmp__ = a;__create_command(__SET_I2C_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
void smb_i2c_def_addr(uint8_t a)			{__tmp__ = a;__create_command(__SET_DEF_I2C_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
void smb_set_output(uint8_t o)				{while(smb_ready());__tmp__ = o;__create_command(__SET_OUTPUT,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
void smb_set_def_output(uint8_t o)			{__tmp__ = o;__create_command(__SET_DEFAULT_OUTPUT,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
void smb_set_lcd_i2c_addr(uint8_t a)		{__tmp__ = a;__create_command(__SET_LCD_I2C_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
void smb_set_lcd_i2c_def_addr(uint8_t a)	{__tmp__ = a;__create_command(__SET_LCD_I2C_DEF_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
void set_display(uint8_t d)					{__tmp__ = (d); __create_command(__SET_DISPLAY, __null__str,1,(const uint8_t const *)(&__tmp__));}
void set_flash(uint8_t f)					{__tmp__ = (f); __create_command(__SET_BLINK, __null__str,1,(const uint8_t const *)(&__tmp__));}
void set_cursor(uint8_t c)					{__tmp__ = (c); __create_command(__SET_CURSOR, __null__str,1,(const uint8_t const *)(&__tmp__));}
void smb_set_backlight(uint8_t b)			{while(smb_ready()); __tmp__ = (b); __create_command(__SET_BACKLIGHT,__null__str,1,(const uint8_t const *)(&__tmp__));}
void smb_set_display(uint8_t d)				{__tmp__ = (d); __create_command(__SET_DISPLAY, __null__str,1,(const uint8_t const *)(&__tmp__));}
void smb_set_flash(uint8_t f)				{__tmp__ = (f); __create_command(__SET_BLINK, __null__str,1,(const uint8_t const *)(&__tmp__));}
void smb_set_cursor(uint8_t c)				{__tmp__ = (c); __create_command(__SET_CURSOR, __null__str,1,(const uint8_t const *)(&__tmp__));}