#include <detpic32.h>

int main(void)
{
    TRISDbits.TRISD0 = 0;   // RD0 set as output
    TRISDbits.TRISD2 = 0;   // RD2 set as output
    LATDbits.LATD0 = 0;
    LATDbits.LATD2 = 0;

    T3CONbits.TCKPS = 7;    // 1:256 prescaler (i.e. fin = 78 125 KHz)
    T1CONbits.TCKPS = 5;
    PR3 = 585936,5;          // Fout = 20MHz / (256 * (39061,5 + 1)) = 2 Hz
    PR1 = 31249;
    TMR3 = 0;               // Clear timer T3 count register
    TMR1 = 0;
    T3CONbits.TON = 1;      // Enable timer T3 (must be the last command of the
    T1CONbits.TON = 1;
                            // timer configuration sequence
    
    IFS0bits.T3IF = 0;      //Reset timer T3 interrupt flag
    IPC3bits.T3IP = 2;      //Interrupt priority (must be in range[1..6])
    IEC0bits.T3IE = 1;      //Enable timer T3 interrupts

    IFS0bits.T1IF = 0;      //Reset timer T1 interrupt flag
    IPC1bits.T1IP = 3;      //Interrupt priority (must be in range[1..6])
    IEC0bits.T1IE = 1;      //Enable timer T1 interrupts
    
    EnableInterrupts();
    while(1);
}

void _int_(12)isr_T3(void)
{
    putChar('3');
    LATDbits.LATD2 = !LATDbits.LATD2;
    IFS0bits.T3IF = 0;
}

void _int_(4)isr_T1(void)
{
    putChar('1');
    LATDbits.LATD0 = !LATDbits.LATD0;
    IFS0bits.T1IF = 0;   
}