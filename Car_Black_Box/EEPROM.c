/*
 * File:   EEPROM.c
 * Author: areef
 *
 * Created on 04 April 2023, 10:05
 */


#include "i2c.h"
#include "EEPROM.h"


void init_AT24C04(void)
{
    unsigned char dummy;
    
    dummy = read_AT24C04(SEC_ADDR);
    dummy = dummy & 0x7F;
    write_AT24C04(SEC_ADDR, dummy);
}

unsigned char read_AT24C04(unsigned char addr)
{
    unsigned char data;
    
    i2c_start();
    i2c_write(EE_SLAVE_WRITE);
    i2c_write(addr);
    i2c_rep_start();
    i2c_write(EE_SLAVE_READ);
    data = i2c_read(0);
    i2c_stop();
    
    return data;
}

void write_AT24C04(unsigned char addr, unsigned char data)
{
    i2c_start();
    int ack = i2c_write(EE_SLAVE_WRITE);
    i2c_write(addr);
    i2c_write(data);
    i2c_stop();
}

void AT24C04_str_write(unsigned char addr, char *data)
{
    while(*data != 0)
    {
        write_AT24C04(addr, *data);
        data++;
        addr++;
    }
}