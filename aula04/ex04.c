#include <detpic32.h>

#define MSVAL 20000

void delay(unsigned int);


int main(void)
{
    // enable display low (RD5) and disable display high (RD6)
    LATDbits.LATD6 = 0;
    LATDbits.LATD5 = 1;
    // port RB8 - RB14 as outputs
    TRISB = TRISB & 0x807F;
    // port RD5 - RD6 as outputs
    TRISD = TRISD & 0XFF9F; 
    while(1)
    {
        int count = 1;
        int i;
        for(i = 0; i < 7; i++)
        {
            // send to display
            LATB = LATB & 0x807F | (count << 8);
            // wait .5 seconds
            delay(500);
            count  = count << 1;
        }
        // toggle display selection
        LATDbits.LATD6 = !LATDbits.LATD6;
        LATDbits.LATD5 = !LATDbits.LATD5;
    }
    return 0;
}

void delay(unsigned int ms)
{
    unsigned int wait;
    
    resetCoreTimer();
    wait = MSVAL * ms;
    while(readCoreTimer() < wait);
}