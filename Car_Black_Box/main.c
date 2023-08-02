/*
 * Name         : Areef Ahmad
 * Date         : 06/04/2023
 * Description  : Project - Car Black Box 
 */


#include "main.h"
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)


static void init_config(void) {
    /* Initialising all peripherals */
    init_clcd();
    init_digital_keypad();
    init_adc();
    init_i2c(100000);
    init_ds1307();
    init_AT24C04();
    init_timer2();
    init_uart(9600);
    
    /* Enabling global and peripheral interrupt enable bits */
    GIE = 1;
    PEIE = 1;
    
}

void main(void) {
    init_config();
    unsigned char control_flag = DASH_BOARD_FLAG, reset_flag;
    unsigned char speed = 0;
    unsigned char key, key2; 
    unsigned char wait1 = 0;
    char event[3] = "ON";
    char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};
    int gr = 0;
    unsigned char menu_pos;
    
    log_event(event, speed);
    //default system password will be stored in external EEPROM
    AT24C04_str_write(0x00, "1111");
    
    while (1) {
        
        __delay_us(500); //To remove bouncing effect 
        
        key = read_digital_keypad(STATE);
        key2 = read_digital_keypad(LEVEL);
        
        
        speed = read_adc()/10;
        if (speed > 99)
        {
            speed = 99;
        }
        
        if (key == SW1)
        {
            strcpy(event, "C ");
            log_event(event, speed);
        }
        else if (key == SW2 && gr < 6)
        {
            strcpy(event, gear[gr]);
            gr++;
            log_event(event, speed);
            
        }
        else if (key == SW3 && gr > 0)
        {
            gr--;
            strcpy(event, gear[gr]);
            log_event(event, speed);
        }
        else if ((key == SW4 || key == SW5) && (control_flag == DASH_BOARD_FLAG))
        {
            clear_screen();
            control_flag = LOGIN_FLAG;
            reset_flag = RESET_PASSWORD;
            clcd_print("Enter Password", LINE1(1));
            clcd_putch(' ', LINE2(2));
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            TMR2ON = 1;
        }
        else if (control_flag == LOGIN_MENU_FLAG && key2 == SW6)
        {
            switch(menu_pos)
            {
                case 0:
                    control_flag = VIEW_LOGS_FLAG;
                    break;
                case 1:
                    control_flag = CLEAR_LOGS_FLAG;
                    break;
                case 2:
                    control_flag = DOWNLOAD_FLAG;
                    break;
                case 3:
                    control_flag = CHANGE_PASSWORD;
                    break;
                case 4:
                    control_flag = RESET_TIME_FLAG;
                    break;
            }
            wait1 = 0;
        }
        else if (control_flag == LOGIN_MENU_FLAG && (key2 == SW5 && wait1++ == 10))
        {
            clear_screen();
            control_flag = DASH_BOARD_FLAG;
            wait1 = 0;
        }
        switch (control_flag)
        {
            case DASH_BOARD_FLAG: 
                display_dash_board(event, speed);
                break;
                
            case LOGIN_FLAG:
                switch (login(reset_flag, key))
                {
                    case RETURN_BACK:
                        control_flag = DASH_BOARD_FLAG;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    case LOGIN_SUCCESS:
                        control_flag = LOGIN_MENU_FLAG;
                        reset_flag = RESET_LOGIN_MENU;
                        continue;
                        //break;
                }
                break;
            case LOGIN_MENU_FLAG:
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                menu_pos = login_menu(reset_flag, key);
                break; 
            case VIEW_LOGS_FLAG:
                view_logs();
                clear_screen();
                control_flag = LOGIN_MENU_FLAG;
                break;
            case DOWNLOAD_FLAG:
                download_logs();
                clear_screen();
                control_flag = LOGIN_MENU_FLAG;
                break;
            case CLEAR_LOGS_FLAG:
                clear_screen();
                clear_logs();
                control_flag = LOGIN_MENU_FLAG;
                break;
            case CHANGE_PASSWORD:
                clear_screen();
                change_password();
                control_flag = DASH_BOARD_FLAG;
                break;
            case RESET_TIME_FLAG:
                clear_screen();
                reset_time();
                control_flag = DASH_BOARD_FLAG;
                break;
        }
        reset_flag = RESET_NOTHING;
    }
    return;
}