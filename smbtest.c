
#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>

#include "i2c_master_min.h"
#include "smb.h"

#define SCL_CLOCK	100000UL

#define DEL			4	
#define DELAY(d)	_delay_us(d)


  int main(void) {
	uint16_t  counter = 0,counter1= 0xffff,counter2= 500,counter3=1; 
	volatile float a = 0;
	
	_delay_ms(200);
	
	smb_init();
	
	DDRC &= ~((1 << PC4) | (1 << PC5));
	PORTC |= (1 << PC4) | (1 << PC5); 
	asm("nop");
	   
	
	i2c_master_init();
	smb_set_output(NAK_LCD);
	DELAY(DEL);
	smb_init_lcd();
	DELAY(DEL);
	smb_cls();
	DELAY(DEL);
	smb_set_output(I2C_LCD);
	DELAY(DEL);
	smb_init_lcd();

	DELAY(DEL);
	smb_cls();
	DELAY(DEL);
	
	while(1) {
 		if(counter & 1) {
 			smb_set_output(NAK_LCD);
 
 			DELAY(DEL);
 			smb_goto(0,0);
 			DELAY(DEL);
 			smb_printf("1:counter:%05u", counter);
 			DELAY(DEL);
 			smb_goto(1,0);
 			DELAY(DEL);
 			smb_printf("2:counter:0x%04X",counter1);
 			DELAY(DEL);
 			smb_goto(2,0);
 			DELAY(DEL);
 			smb_printf("3:counter:0x%04X",counter2);
 			DELAY(DEL);
 			smb_goto(3,0);
 			DELAY(DEL);
 			smb_printf("4:counter:0x%04X",counter3);
 			DELAY(DEL);
 			//DELAY(500000);
 
 		}
 		else {
 			smb_set_output(I2C_LCD);
 			DELAY(DEL);
 			smb_goto(0,0);
 			DELAY(DEL);
 			smb_printf("float:%0.3f",a);
 			DELAY(DEL);
 			smb_goto(1,0);
 			DELAY(DEL);
 			smb_printf("exp:%.2e",a);
 			DELAY(DEL);
 		}
 		counter++;
 		counter1 --;
 		counter2 += 10;
 		counter3 *= 2;
 		counter3 += 3;
		a+=0.001;
	}
}


