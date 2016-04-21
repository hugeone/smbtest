/*************************************************************************
* Title:    I2C master library using hardware TWI interface
* Author:   Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
* File:     $Id: twimaster.c,v 1.4 2015/01/17 12:16:05 peter Exp $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device with hardware TWI 
* Usage:    API compatible with I2C Software Library i2cmaster.h
**************************************************************************/
#include <inttypes.h>
#include <avr/io.h>
#include <compat/twi.h>
#include "i2c_master_min.h"

void i2c_master_init(void)
{
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
  
  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */

}/* i2c_init */

void i2c_master_start(uint8_t address)
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

void i2c_master_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while(TWCR & (1<<TWSTO));

}


void i2c_master_write( uint8_t data )
{	
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

uint8_t i2c_master_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
    return TWDR;

}
