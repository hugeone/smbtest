#ifndef __I2C_MIN
#define __I2C_MIN 


#define SCL_CLOCK  100000UL
#define I2C_READ    1
#define I2C_WRITE   0


extern void i2c_master_init(void);
extern void i2c_master_stop(void);
extern void i2c_master_start(uint8_t addr);
extern void i2c_master_write(uint8_t data);
extern uint8_t i2c_master_readNak(void);

#endif