#include <detpic32.h>

#define MSVAL 20000

volatile int aux;
volatile unsigned char voltage = 0; //Global variable


void send2displays(unsigned char value);
void delay(unsigned int);
unsigned char toBcd(unsigned char value);

int main(void)
{
    unsigned int cnt = 0;
    //Configure all(digital I/O, analog input, A/D module)
    TRISBbits.TRISB14 = 1;
    TRISB = TRISB & 0xFC00;

    AD1PCFGbits.PCFG14 = 0;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.CLRASAM = 1;
    AD1CON3bits.SAMC = 16;
    AD1CON2bits.SMPI = 8-1; // 1 sample
    AD1CHSbits.CH0SA = 14; // input = 14;
    
    //Configure interrupt system
    IPC6bits.AD1IP = 4;
    IEC1bits.AD1IE = 1;
    IFS1bits.AD1IF = 0;
    EnableInterrupts();
    
    AD1CON1bits.ON = 1;
    while(1)
    {
        if(cnt == 25)
        {
            cnt = 0;
            //Start Conversion
            AD1CON1bits.ASAM = 1;
        }

        send2displays(voltage);
        cnt = (cnt + 1) % 256;
        delay(50);
    }
    return 0;
}

//void _int_(VECTOR) isr_adc(void) -> Vector = vector number in sheet
// AD1 - ADC1 Convert Done -> vector = 27
void _int_(27) isr_adc(void)
{
    int med=0; 
    int j;
    int sum=0;
    
    int *p = (int*) (&ADC1BUF0);
    for(j=0;j<8;j++){
        sum += p[j*4];
    }

    med=((sum/8)*33+511)/1023;
    voltage = toBcd(med);
    
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