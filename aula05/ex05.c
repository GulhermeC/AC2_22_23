#include <detpic32.h>

#define MSVAL 20000

void send2displays(unsigned char value);
void delay(unsigned int);
unsigned char toBcd(unsigned char value);


int main(void)
{
    //inputs
    TRISB = TRISB & 0x80FF;
    //outputs
    TRISD = TRISD & 0XFF9F;
    int counter = 0;
    int bcd;
    while(1)
    {
        int i = 0;
        do
        {
            // wait .5 seconds = delay(500)
            delay(10);
            // send to display
            bcd = toBcd(counter);
            send2displays(bcd);       
        }while(++i < 20);
        counter = ++counter%256;
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

void send2displays(unsigned char value)
{
    static unsigned char displayFlag = 0;
    static const char display7codes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    char digit_low = value & 0x0F;
    char digit_high = value >> 4;

    if (displayFlag == 0){
        LATDbits.LATD6 = 1;
        LATDbits.LATD5 = 0;
        LATB= (LATB & 0x80FF)|(display7codes[digit_high] << 8);
        displayFlag = 1;
    }
    else{
        LATDbits.LATD6 = 0;
        LATDbits.LATD5 = 1;
        LATB= (LATB & 0x80FF)|(display7codes[digit_low] << 8);
        displayFlag = 0;
    }
}

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}