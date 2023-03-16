#include <detpic32.h>

#define MSVAL 20000

void delay(unsigned int);


int main(void)
{
    // port RC14 as output
    TRISCbits.TRISC14 = 0;
    while(1)
    {
        delay(500);
        LATCbits.LATC14 = !LATCbits.LATC14;
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