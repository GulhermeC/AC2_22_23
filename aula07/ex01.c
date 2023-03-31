#include <detpic32.h>

#define MSVAL 20000

volatile int aux;
void delay(unsigned int);

int main(void)
{
    while(1)
    {
        //Configure all(digital I/O, analog input, A/D module)
        TRISBbits.TRISB14 = 1;
        AD1PCFGbits.PCFG14 = 0;
        AD1CON1bits.SSRC = 7;
        AD1CON1bits.CLRASAM = 1;
        AD1CON3bits.SAMC = 16;
        AD1CON2bits.SMPI = 1-1; // 1 sample
        AD1CHSbits.CH0SA = 14; // input = 14;
        
        //Configure interrupt system
        IPC6bits.AD1IP = 4;
        IEC1bits.AD1IE = 1;
        IFS1bits.AD1IF = 0;
        EnableInterrupts();
        
        AD1CON1bits.ON = 1;
        // Start A/D conversion
        AD1CON1bits.ASAM = 1;
        while(1);
    }
    return 0;
}

//void _int_(VECTOR) isr_adc(void) -> Vector = vector number in sheet
// AD1 - ADC1 Convert Done -> vector = 27
void _int_(27) isr_adc(void)
{
    //delay(200)
    //Read conversion result (ADC1BUF0) and print it 
    printInt(ADC1BUF0,16);
    printStr("\n");

    aux=ADC1BUF0;
    //Start A/D conversion
    AD1CON1bits.ASAM = 1;
    //Reset AD1IF flag
    IFS1bits.AD1IF = 0;
}

void delay(unsigned int ms)
{
    unsigned int wait;
    
    resetCoreTimer();
    wait = MSVAL * ms;
    while(readCoreTimer() < wait);
}