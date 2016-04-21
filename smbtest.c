/*
 * i2c_display_mster.c
 *
 * Created: 19/12/2015 20:16:37
 * Author : PJ
 */
#include <avr/io.h>
#include <util/delay.h>
#include <compat/twi.h>
#include <stddef.h>



#include "i2c_master_min.h"
//#include "i2cslave.h"

/* I2C clock in Hz */
#define SCL_CLOCK  100000UL



#define _DUMMY			0
#define I2C_LCD			1
#define NAK_LCD			2
#define UART			3
#define TERM			4
#define __LAST__		5



#define LCD_I2C	__wri


#define __PRINTF				0x01
#define	__PRINTC				0x02
#define __PRINTS				0x03
#define __PRINT_DEC				0x50
#define __PRINT_HEX				0x51
#define __SET_OUTPUT			0x04
#define __WRITE_EEPROM			0x05
#define __READ_EEPROM			0x06
#define __WRITE_SRAM			0x07
#define __READ_SRAM				0x08
#define __SET_DEFAULT_OUTPUT 	0x09
#define __DISP_INIT				0x20
#define __DISP_CLR				0x21
#define __DISP_GOTO				0x22
#define __SET_DISPLAY			0x23
#define __SET_BACKLIGHT			0x27
#define __SET_CURSOR			0x28
#define __SET_BLINK				0x29
#define __SHIFT_RIGHT			0x35
#define __SHIFT_LEFT			0x36
#define __SET_CHAR				0x37
#define __INIT_BAR				0x40
#define __DRAW_BAR				0x41
#define __SET_I2C_ADDR			0x60
#define __SET_DEF_I2C_ADDR		0x61
#define __SET_LCD_I2C_ADDR		0x62
#define __SET_LCD_I2C_DEF_ADDR	0x63
#define __SAVE_DISPLAY_SETTINGS	0x70
#define __LOAD_DISPLAY_SETTINGS 0x71


#define DL_BIT		2
#define CR_BIT		1
#define BL_BIT		0



#define __DISPLAY_CONTROL__					1

#define TR_ZEROES							(1 << 6)
#define TR_SPACES							(0)

static volatile char *__null__str  = "";

#define smb_prints(string)					{__create_command(__PRINTS,(const char *)(string),0,__null__str);}
#define smb_printf(format,par)				{while(smb_ready()); _delay_ms(1);__create_command(__PRINTF,(const char *)(format),sizeof(par),(const uint8_t const *)(&(par)));}
#define smb_goto(x,y)						{while(smb_ready());__tmp__ = ((x) << 8) | (y);__create_command(__DISP_GOTO,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
#define smb_cls()							{while(smb_ready());__create_command(__DISP_CLR,__null__str,0,__null__str);}
#define smb_init_lcd()							{__create_command(__DISP_INIT,__null__str,0,__null__str);}
#define smb_bar_init()						{__create_command(__INIT_BAR,__null__str,0,__null__str);}
#define smb_shift_left()					{__create_command(__SHIFT_LEFT,__null__str,0,__null__str);}
#define smb_shift_right()					{__create_command(__SHIFT_RIGHT,__null__str,0,__null__str);}
#define smb_i2c_addr(a)						{__tmp__ = a;__create_command(__SET_I2C_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
#define smb_i2c_def_addr(a)					{__tmp__ = a;__create_command(__SET_DEF_I2C_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
#define smb_set_output(o)					{__tmp__ = o;__create_command(__SET_OUTPUT,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
#define smb_set_def_output(o)				{__tmp__ = o;__create_command(__SET_DEFAULT_OUTPUT,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
#define smb_set_lcd_i2c_addr(a)				{__tmp__ = a;__create_command(__SET_LCD_I2C_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}
#define smb_set_lcd_i2c_def_addr(a)			{__tmp__ = a;__create_command(__SET_LCD_I2C_DEF_ADDR,__null__str,sizeof(int),(const uint8_t const *)&__tmp__);}

#define print_dec(num,nchars,zeroes)		{__tmp__ = (nchars) | (zeroes);__create_command(__PRINT_DEC,(char *)&__tmp__,sizeof((num)),(const uint8_t const *)(&num));}

#define set_display(d)						{__tmp__ = (d); __create_command(__SET_DISPLAY,  __null__str,1,(const uint8_t const *)(&__tmp__));}
#define set_flash(f)						{__tmp__ = (f); __create_command(__SET_BLINK,  __null__str,1,(const uint8_t const *)(&__tmp__));}
#define set_cursor(c)						{__tmp__ = (c); __create_command(__SET_CURSOR,  __null__str,1,(const uint8_t const *)(&__tmp__));}
#define set_backlight(b)					{__tmp__ = (b); __create_command(__SET_BACKLIGHT,__null__str,1,(const uint8_t const *)(&__tmp__));}
#define smb_set_display(d)					{__tmp__ = (d); __create_command(__SET_DISPLAY,  __null__str,1,(const uint8_t const *)(&__tmp__));}
#define smb_set_flash(f)					{__tmp__ = (f); __create_command(__SET_BLINK,  __null__str,1,(const uint8_t const *)(&__tmp__));}
#define smb_set_cursor(c)					{__tmp__ = (c); __create_command(__SET_CURSOR,  __null__str,1,(const uint8_t const *)(&__tmp__));}
#define smb_set_backlight(b)				{__tmp__ = (b); __create_command(__SET_BACKLIGHT,__null__str,1,(const uint8_t const *)(&__tmp__));}

volatile char *str = "Ala ma kota :)   ";

#define DL_BIT		2
#define CR_BIT		1
#define BL_BIT		0

uint8_t I2C_ADDR = 0x40;


void __create_command(uint8_t command, const char *format, uint8_t parSize, const uint8_t const *par) {


	i2c_master_start(I2C_ADDR);

	i2c_master_write(command);

	i2c_master_write(parSize);
//	__buff[0] = command;
//	__buff[1] = parSize;
	while(parSize--) {
		i2c_master_write(*par++);
	}
//		__buff[i++] = *par++;
	while(*format) {
		i2c_master_write(*format++);
//		__buff[i++] = *format++;
	}
	i2c_master_stop();
}

uint16_t 	__tmp__;


void initSPImaster(void) {
	DDRB |= (1 << PB3) | (1 << PB5)| (1 << PB2);
	DDRB &= ~((1 << PB4));

	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPR0);

	PORTB |= (1 << PB2);
}

uint8_t smb_ready(void) {
/*	if((PINC & (1 << PC4)) && (PINC & (1 << PC5))) {
		DDRC |= (1 << PC4) | (1 << PC5);
		PORTC |= (1 << PC4) | (1 << PC5);
		DDRC &= ~((1 << PC4) | (1 << PC5));
	}*/
	i2c_master_start(I2C_ADDR + I2C_READ);
	return i2c_master_readNak();
}

#define SDADDR		DDRC
#define SCLDDR		DDRC
#define SDAPORT		PORTC
#define SCLPORT		PORTC
#define SDAPIN		PC4
#define SCLPIN		PC5

void smb_init(void) {
	uint8_t	f;

	SDADDR |= (1 << SDAPIN);
	SCLDDR |= (1 << SCLPIN);
	for(f = 0; f < 10 ; f++) {
		SCLPORT &= ~(1 << SCLPIN);
		SDAPORT &= ~(1 << SDAPIN);
		_delay_us(10);
		SCLPORT |= (1 << SCLPIN);
		SDAPORT |= (1 << SDAPIN);
		_delay_us(10);
	}
}


int main(void) {
	float a = 0,b;
	uint8_t	f;

	DDRC |= (1 << PC5) | (1 << PC5);
	for(f = 0; f < 20 ; f++) {
		PORTC &= ~(1 << PC5);
		PORTC &= ~(1 << PC4);
		_delay_us(10);
		PORTC |= (1 << PC5);
		PORTC |= (1 << PC4);
		_delay_us(10);
	}


	smb_init();

	i2c_master_init();
	smb_init_lcd();
	smb_cls();
	while(1) {

		smb_goto(0,0);
		smb_printf("E:%2.2e",a);
		smb_goto(1,3);
		b = a * 2;
		smb_printf("F: %f ",b);
		b = a * 5;
		smb_printf("E: %E ",b);


		//_delay_ms(300);
		//_delay_ms(20);
		a+=.001;
		//_delay_ms(300);
	}
}


