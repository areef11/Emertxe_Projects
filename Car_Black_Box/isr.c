#include <xc.h>
#include "main.h"

extern int min;
static int sec = 0;
int return_time;
unsigned char ch;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {
        if (++count == 1250)
        {
            count = 0;
            sec++;
            if (sec > 60)
            {
                if (min > 0)
                {
                    min--;
                }
                else if (min == 0 && return_time > 0)
                {
                    return_time--;
                }
                sec = 0;
            }
        }
        TMR2IF = 0;
    }
    
    if (RCIF == 1)
    {
        if (OERR == 1)
            OERR = 0;
        
        ch = RCREG;
        
        RCIF = 0;
    }
}