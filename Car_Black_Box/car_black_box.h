/* 
 * File:   car_black_box.h
 * Author: areef
 *
 * Created on 31 March 2023, 08:36
 */

#ifndef CAR_BLACK_BOX_H
#define	CAR_BLACK_BOX_H

void display_dash_board(char event[], unsigned char speed);
void log_event(char event[], unsigned char speed);
void clear_screen(void);
void clcd_write(unsigned char byte, unsigned char mode);
unsigned char login(unsigned char reset_flag, unsigned char key);
unsigned char login_menu(unsigned char reset_flag, unsigned char key);

void view_logs();
void download_logs();
void clear_logs();
void reset_time();
void change_password();

#endif	/* CAR_BLACK_BOX_H */

