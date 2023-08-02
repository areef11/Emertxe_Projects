/* 
 * File:   main.h
 * Author: areef
 *
 * Created on 31 March 2023, 08:32
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "car_black_box.h"
#include <string.h>
#include "EEPROM.h"
#include "timers.h"
#include "uart.h"

#define DASH_BOARD_FLAG     1
#define LOGIN_FLAG          2
#define LOGIN_SUCCESS       3
#define LOGIN_MENU_FLAG     4
#define RESET_LOGIN_MENU    5
#define RETURN_BACK         6
#define RESET_PASSWORD      7
#define RESET_NOTHING       8

#define VIEW_LOGS_FLAG      9
#define CLEAR_LOGS_FLAG     11
#define DOWNLOAD_FLAG       10
#define CHANGE_PASSWORD     12
#define RESET_TIME_FLAG     13

#define DISP_ON_AND_CURSOR_ON   0x0F


void view_logs(void);
void download_logs(void);
void clear_logs(void);
void change_password(void);
void reset_time(void);
void clear_screen(void);


#endif	/* MAIN_H */

