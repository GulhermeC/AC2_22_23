#include <detpic32.h>


int main(void)
{
    volatile int aux;
    // RB4 digital output disconnected
    TRISBbits.TRISB4 = 1;
    //set RB4 as analog input (AN4)
    AD1PCFGbits.PCFG4 = 0;
    // Conversion trigger selection bits: in this mode
    // an internal counter ends sampling and starts conversion
    AD1CON1bits.SSRC = 7;
    // Stop the conversion when the 1st A/D converter
    // interrup is generated. At the same time,
    // hardware clears the ASAM bit
    AD1CON1bits.CLRASAM = 1;
    // Sample time is 16 TAD (TAD = 100 ns)
    AD1CON3bits.SAMC = 16;
    // Interrupt is generated after N samples
    // recplace (N by the desired number of consecutive samples)
    // AD1CON2bits.SMPI = N-1;
    AD1CON2bits.SMPI = 1-1;
    // replace x by the desired input analog channel (0 to 15)
    // AD1CHSbits.CH0SA = x;
    AD1CHSbits.CH0SA = 4;
    // Enable A/D converter
    AD1CON1bits.ON = 1;
    // This must be the last command of the A/D
    // configure sequence

    TRISDbits.TRISD11 = 0;
    
    while(1)
    {
        //start conversion 
        AD1CON1bits.ASAM=1;
        LATDbits.LATD11 = 1;
        //wait for conversion to finish
        while (IFS1bits.AD1IF == 0);
        LATDbits.LATD11 = 0;
        printInt(ADC1BUF0, 16 | 3 << 16);
        printStr("\n");

        aux=ADC1BUF0;

        IFS1bits.AD1IF=0;
    }
    return 0;
}