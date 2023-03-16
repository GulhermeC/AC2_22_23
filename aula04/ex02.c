#include <detpic32.h>

#define MSVAL 20000

void delay(unsigned int);

int main(void)
{
    TRISE = TRISE & 0xFF87; // re6/5/4/3 = 0 as outpúts;
    int count = 0;

    while(1)
    {
        LATE = (LATE & 0x87) | (count << 3); //0x87 = 1000 0111 => re6/5/4/3 = 0   
        delay(250);
        count += 1;

        if(count >= 16)
        {
            count = 0;
        }
    }
}

void delay(unsigned int ms)
{
    unsigned int wait;

    resetCoreTimer();
    wait = MSVAL * ms;
    while(readCoreTimer() < wait);
}