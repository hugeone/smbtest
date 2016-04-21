#ifndef __SMB__
#define __SMB__

#define SDADDR					DDRC
#define SCLDDR					DDRC
#define SDAPORT					PORTC
#define SCLPORT					PORTC
#define SDAPIN					PC4
#define SCLPIN					PC5



	
#define _DUMMY					0
#define I2C_LCD					1
#define NAK_LCD					2
#define UART					3
#define TERM					4
#define __LAST__				5

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


#define DL_BIT					2
#define CR_BIT					1
#define BL_BIT					0

extern uint8_t I2C_ADDR;
extern char *__null__str;
extern uint16_t __tmp__;

#define __DISPLAY_CONTROL__		1

#define TR_ZEROES				(1 << 6)
#define TR_SPACES				(0)

uint8_t smb_ready(void);
void __create_command(uint8_t command, const char *format, uint8_t parSize, const uint8_t const *par);
void smb_init(void);

#define smb_printf(format,par)				{while(smb_ready()); __create_command(__PRINTF,(const char *)(format),sizeof(par),(const uint8_t const *)(&(par)));}
#define print_dec(num,nchars,zeroes)		{__tmp__ = (nchars) | (zeroes);__create_command(__PRINT_DEC,(char *)&__tmp__,sizeof((num)),(const uint8_t const *)(&num));}

void smb_prints(char *string);	
void smb_goto(uint8_t x, uint8_t y);	
void smb_cls(void);						
void smb_init_lcd(void);				
void smb_bar_init(void);				
void smb_shift_left(void);				
void smb_shift_right(void);				
void smb_i2c_addr(uint8_t a);			
void smb_i2c_def_addr(uint8_t a);			
void smb_set_output(uint8_t o);				
void smb_set_def_output(uint8_t o);			
void smb_set_lcd_i2c_addr(uint8_t a);		
void smb_set_lcd_i2c_def_addr(uint8_t a);	
void set_display(uint8_t d);				
void set_flash(uint8_t f);					
void set_cursor(uint8_t c);					
void smb_set_backlight(uint8_t b);			
void smb_set_display(uint8_t d);			
void smb_set_flash(uint8_t f);				
void smb_set_cursor(uint8_t c);				

#endif