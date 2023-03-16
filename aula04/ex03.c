#include <detpic32.h>

void delay(unsigned int);


int main(void)
{
    char c;
    LATDbits.LATD6 = 1;
    LATDbits.LATD5 = 0;
    // port RB8 - RB14 as output
    TRISB = TRISB & 0x807F;
    TRISD = TRISD & 0XFF9F;
    while(1)
    {
        c = getChar();
        LATB = LATB & 0x807F;

        if(c == 'a' || c == 'A')
        {
            LATBbits.LATB8 = 1;
        }
        if(c == 'b' || c == 'B')
        {
            LATBbits.LATB9 = 1;
        }
        if(c == 'c' || c == 'C')
        {
            LATBbits.LATB10 = 1;
        }
        if(c == 'd' || c == 'D')
        {
            LATBbits.LATB11 = 1;
        }
        if(c == 'e' || c == 'E')
        {
            LATBbits.LATB12 = 1;
        }
        if(c == 'f' || c == 'F')
        {
            LATBbits.LATB13 = 1;
        }
        if(c == 'g' || c == 'G')
        {
            LATBbits.LATB14 = 1;
        }
    }
    return 0;
}
