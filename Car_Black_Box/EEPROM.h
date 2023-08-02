/* 
 * File:   EEPROM.h
 * Author: areef
 *
 * Created on 04 April 2023, 10:05
 */

#ifndef AT24C04_H
#define	AT24C04_H

#define EE_SLAVE_WRITE             0b10100000 // 0xD0
#define EE_SLAVE_READ              0b10100001 // 0xD1

#define SEC_ADDR                0x00


void init_AT24C04(void);
unsigned char read_AT24C04(unsigned char addr);
void write_AT24C04(unsigned char addr, unsigned char data);
void AT24C04_str_write(unsigned char addr, char *data);

#endif

