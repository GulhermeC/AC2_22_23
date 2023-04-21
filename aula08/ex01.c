#include <detpic32.h>

int main(void)
{
    T3CONbits.TCKPS = 7;    // 1:256 prescaler (i.e. fin = 78 125 KHz)
    PR3 = 39062;            // Fout = 20MHz / (256 * (39061,5 + 1)) = 2 Hz
    TMR3 = 0;               // Clear timer T2 count register
    T3CONbits.TON = 1;      // Enable timer T2 (must be the last command of the
                            // timer configuration sequence
    while(1)
    {
        while(IFS0bits.T3IF==0);
        IFS0bits.T3IF=0;
        putChar('.');
    }
}