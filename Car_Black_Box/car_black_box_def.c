/*
 * File:   car_black_box_def.c
 */

#include "main.h"

//variable declaration 
unsigned char clock_reg[3];
char time[7];
char log[11];
static char pos = -1;
static int access = 0; //for printing logs
int min;
extern int return_time;
char *menu[] = {"View Log", "Clear Log", "Download Log", "Change Password", "Reset Time"};


void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    
    //HH
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    //MM
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    //SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    
    time[6] = '\0';
}

void display_time()
{
    get_time();
    
    clcd_putch(time[0], LINE2(2));
    clcd_putch(time[1], LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(time[2], LINE2(5));
    clcd_putch(time[3], LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(time[4], LINE2(8));
    clcd_putch(time[5], LINE2(9));
}

void display_dash_board(char event[], unsigned char speed)
{
    clcd_print("  TIME     E  SP", LINE1(0));
    //display time
    display_time();
    //to display event
    clcd_print(event, LINE2(11));
    //to display speed
    clcd_putch(speed/10 + '0', LINE2(14));
    clcd_putch(speed%10 + '0', LINE2(15));
    
}

void log_car_event()
{
    char addr;
    addr = 5;
    pos++;
    if (pos == 10)
    {
        pos = 0;
    }
    addr = pos * 10 + addr;
    //log write on external EEPROM
    AT24C04_str_write(addr, log);
    
    if (access < 9)
    {
        access++;
    }
    
}

void log_event(char event[], unsigned char speed)
{
    strncpy(log, time, 6);
    strncpy(&log[6], event, 2);
    //speed 0 - 99
    log[8] = speed/10;
    log[9] = speed%10;
    log[10] = '\0';
    log_car_event();
}

unsigned char login(unsigned char reset_flag, unsigned char key)
{
    static char npasswd[4], i;
    static char spasswd[4];
    static unsigned char attempt_rem;
    if (reset_flag == RESET_PASSWORD)
    {
        i = 0;
        attempt_rem = 3;
        npasswd[0] = '\0';
        npasswd[1] = '\0';
        npasswd[2] = '\0';
        npasswd[3] = '\0';
        key = ALL_RELEASED;
        return_time = 5;
        min = 0;
    }
    
    if (return_time == 0)
    {
        return RETURN_BACK;
    }
    
    __delay_us(500);
    if (key == SW4 && i < 4)
    {
        npasswd[i] = '1';
        i++;
        clcd_putch('*', LINE2(i+2));
        return_time = 5;
        key = ALL_RELEASED;
    }
    else if (key == SW5 && i < 4)
    {
        npasswd[i] = '0';
        i++;
        clcd_putch('*', LINE2(i+2));
        return_time = 5;
        key = ALL_RELEASED;
    }
    
    if (i == 4)
    {
        for(int j = 0; j < 4; j++)
        {
            spasswd[j] = read_AT24C04(j);
        }
        if (strncmp(npasswd, spasswd, 4) == 0)
        {
            clear_screen();
            clcd_print("Use SW6/RB5 ", LINE1(0));
            clcd_print("to select option", LINE2(0));
            __delay_ms(3000);
            return LOGIN_SUCCESS;
        }
        else
        {
            attempt_rem--;
            if (attempt_rem == 0)
            {
                clear_screen();
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print("YOU ARE BLOCKED", LINE1(0));
                clcd_print("Wait For 15 Min", LINE2(0));
                min = 15;
                while (min)
                {
                    clcd_putch(min/10 + '0', LINE2(9));
                    clcd_putch(min%10 + '0', LINE2(10));
                }
                attempt_rem = 3;
            }
            else
            {
                clear_screen();
                clcd_print("Wrong Password", LINE1(0));
                clcd_putch(attempt_rem + '0', LINE2(0));
                clcd_print("Attempts Rem", LINE2(2));
                __delay_ms(2000);
                
            }
            clear_screen();
            clcd_print("Enter Password", LINE1(1));
            clcd_putch(' ', LINE2(4));
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            i = 0;
            return_time = 5;
        }
    }
    return 0;
}


void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

unsigned char login_menu(unsigned char reset_flag, unsigned char key)
{
    //char *menu[] = {"View Log", "Clear Log", "Download Log", "Reset Time", "Change Password"};
    //                    0            1              2             3                 4
    static unsigned char menu_pos;
    
    if (reset_flag == RESET_LOGIN_MENU)
    {
        menu_pos = 0;
        clear_screen();
    }
    
    if (key == SW5 && menu_pos < 4) 
    {
        menu_pos++;
        clear_screen();
    }
    else if (key == SW4 && menu_pos > 0)
    {
        menu_pos--;
        clear_screen();
    }
    
    if(menu_pos < 4)
    {
        clcd_putch('*', LINE1(0)); 
        clcd_print(menu[menu_pos], LINE1(2));
        clcd_print(menu[menu_pos+1], LINE2(2));
    }
    else if (menu_pos == 4)
    {
        clcd_putch('*', LINE2(0)); 
        clcd_print(menu[menu_pos-1], LINE1(2));
        clcd_print(menu[menu_pos], LINE2(2));
    }
    return menu_pos;
}

void view_logs()
{        
    /* view_logs function displays the stored logs sequentially */
    if (access == 0) {
        clear_screen();
        clcd_print("No Logs", LINE1(4));
        clcd_print("Available", LINE2(4));
        __delay_ms(1500);
        return;
    }
    else
    {
        unsigned char key, read_addr = 5, k = 1, read_pos = 1, wait2 = 0;
        char log_data[11], disp_log[17];
        
        for(int j = 0; j < access; j++)
        {
            key = read_digital_keypad(STATE);
        
            read_addr = read_pos * 10 + read_addr;
            
            for(int i = 0; i < 11; i++)
            {
                log_data[i] = read_AT24C04(read_addr+i);
            }
            
            
            disp_log[0] = k + '0';
            disp_log[1] = ' ';
            disp_log[2] = log_data[0];
            disp_log[3] = log_data[1];
            disp_log[4] = ':';
            disp_log[5] = log_data[2];
            disp_log[6] = log_data[3];
            disp_log[7] = ':';
            disp_log[8] = log_data[4];
            disp_log[9] = log_data[5];
            disp_log[10] = ' ';
            disp_log[11] = log_data[6];
            disp_log[12] = log_data[7];
            disp_log[13] = ' ';
            disp_log[14] = log_data[8] + '0';
            disp_log[15] = log_data[9] + '0';
            disp_log[16] = '\0';
        
            clear_screen();
            clcd_print("# TIME     E  SP", LINE1(0));
            clcd_print(disp_log, LINE2(0));
            __delay_ms(2000);
            k++;
        }
    }
}

void download_logs()
{
    /* download_logs function sends the logs to cutecom (serially connected device) */
    if (access == 0)
    {
        clear_screen();
        clcd_print("No Logs", LINE1(4));
        clcd_print("Available", LINE2(4));
        __delay_ms(1500);
        return;
    }
    
    clear_screen();
    clcd_print("Downloading Logs", LINE1(0));
     
    unsigned char key, read_addr = 5, k = 1, read_pos = 1, wait2 = 0;
    char log_data[11], disp_log[17];
        
    for(int j = 0; j < access; j++)
    {
        key = read_digital_keypad(STATE);
        
        read_addr = read_pos * 10 + read_addr;
           
        for(int i = 0; i < 11; i++)
        {
            log_data[i] = read_AT24C04(read_addr+i);
        }
            
            
        disp_log[0] = k + '0';
        disp_log[1] = ' ';
        disp_log[2] = log_data[0];
        disp_log[3] = log_data[1];
        disp_log[4] = ':';
        disp_log[5] = log_data[2];
        disp_log[6] = log_data[3];
        disp_log[7] = ':';
        disp_log[8] = log_data[4];
        disp_log[9] = log_data[5];
        disp_log[10] = ' ';
        disp_log[11] = log_data[6];
        disp_log[12] = log_data[7];
        disp_log[13] = ' ';
        disp_log[14] = log_data[8] + '0';
        disp_log[15] = log_data[9] + '0';
        disp_log[16] = '\0';
        
        uputs(disp_log);
        uputs("\n");
        __delay_ms(500);
        k++;
    }
    
    return;
        
}

void clear_logs()
{
    if (access == 0)
    {
        clear_screen();
        clcd_print("No Logs Availab-", LINE1(0));
        clcd_print("e To Clear.", LINE2(0));
        __delay_ms(2000);
        return;
    }
    pos = -1;
    access = 0;
    clear_screen();
    clcd_print("Logs Cleared", LINE1(0));
    __delay_ms(2000);
    clcd_print("                ", LINE2(0));
    clear_screen();
    return;
}

void reset_time(void) {
    char set_time[7];
    unsigned char pos = 5;
    unsigned char blink_flag = 0;
    unsigned char key;
    unsigned char digit, wait = 0, exit_bit = 0;
    
    set_time[0] = time[0];
    set_time[1] = time[1];
    set_time[2] = time[2];
    set_time[3] = time[3];
    set_time[4] = time[4];
    set_time[5] = time[5];
    
    clcd_print(set_time, LINE2(4));
    
    while (exit_bit != 1) 
    {
        key = read_digital_keypad(STATE);
        
        if (key == SW5 && wait == 10)
        {
            exit_bit = 1;
        }
        else if (key == SW5)
        {
            pos--;
            if(pos < 4)
            {
                pos = 10;
            }
            wait++;
        }
        else if (key == SW4)
        {
            set_time[pos]++;
            if (set_time[pos] > 9)
            {
                set_time[pos] = 0;
            }
        }
        clcd_print(set_time, LINE2(4));
    }
    
    time[0] = set_time[0];
    time[1] = set_time[1];
    time[2] = set_time[2];
    time[3] = set_time[3];
    time[4] = set_time[4];
    time[5] = set_time[5];
    clear_screen();
    
    return;
}

void change_password()
{
    __delay_us(1000);
    char password[5];
    char c_password[5];
    unsigned char pos = 0;
    unsigned char key;
    key = ALL_RELEASED;
    
    clcd_print("Enter Password", LINE1(1));
    clcd_putch(' ', LINE2(4));
    clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
    __delay_us(100);
    
    while(pos < 4)
    {
        __delay_us(500);
        key = read_digital_keypad(STATE);
        if (key == SW4)
        {
            password[pos] = '1';
            pos++;
            clcd_putch('*', LINE2(pos+2));
            key = ALL_RELEASED;
        }
        else if (key == SW5)
        {
            password[pos] = '0';
            pos++;
            clcd_putch('*', LINE2(pos+2));
            key = ALL_RELEASED;
        }
    }
    
    /* Confirm the new password */
    clear_screen();
    clcd_print("Confirm New", LINE1(1));
    clcd_print("Password:", LINE2(2));
    __delay_ms(1000);
    clear_screen();
    clcd_print("Enter Password", LINE1(1));
    clcd_putch(' ', LINE2(2));
    pos = 0;
    
    while(pos < 4)
    {
        key = read_digital_keypad(STATE);
        if (key == SW4)
        {
            c_password[pos] = '1';
            pos++;
            clcd_putch('*', LINE2(pos+2));
            key = ALL_RELEASED;
        }
        else if (key == SW5)
        {
            c_password[pos] = '0';
            pos++;
            clcd_putch('*', LINE2(pos+2));
            key = ALL_RELEASED;
        }
    }
    
    if(pos == 4)
    {
        clear_screen();
        if(strncmp(password, c_password, 4) == 0)
        {
            clcd_print("Password Changed", LINE1(0));
            AT24C04_str_write(0x00, password);
            __delay_ms(2000);
            clear_screen();
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            return;
        }
        else
        {
            clear_screen();
            clcd_print("Passwords Do", LINE1(0));
            clcd_print("Not Match", LINE2(0));
            __delay_ms(1000);
            clear_screen();
            return;
        }
    }
}
