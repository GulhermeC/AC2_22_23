#include <detpic32.h>

void configureAll(void);

volatile int voltage = 0;

unsigned char toBcd(unsigned char value);
void send2displays(unsigned char value);


int main(void) {

    configureAll();
    
    while(1)
}

void configureAll(void) {

    //inputs
    TRISB = TRISB & 0x80FF;
    //outputs
    TRISD = TRISD & 0XFF9F;
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
    AD1CON2bits.SMPI = 8-1;
    // replace x by the desired input analog channel (0 to 15)
    // AD1CHSbits.CH0SA = x;
    AD1CHSbits.CH0SA = 4;
    // Enable A/D converter
    AD1CON1bits.ON = 1;
    // This must be the last command of the A/D
    // configure sequence

    T1CONbits.TCKPS = 5;    // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR1 = 62499;            // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR1 = 0;               // Reset timer T1 count register
    T1CONbits.TON = 1;      // Enable timer T1 (must be the last command of the
                            // timer configuration sequence)
    
    OC1CONbits.OCM = 6;     // PWM mode on OCx; fault pin disabled
    OC1CONbits.OCTSEL =0;   // Use timer T1 as the time base for PWM generation
    
                            //((PR2 + 1) * duty cycle)/100 
    OC1RS = 12500;        // Ton constant
    OC1CONbits.ON = 1;    // Enable OC1 module

    T3CONbits.TCKPS = 5;    // 1:32 prescaler (i.e Fout_presc = 625 KHz)
    PR3 = 62499;            // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
    TMR3 = 0;               // Reset timer T1 count register
    T3CONbits.TON = 1;      // Enable timer T1 (must be the last command of the
                            // timer configuration sequence)
    
    OC2CONbits.OCM = 6;   // PWM mode on OCx; fault pin disabled
    OC2CONbits.OCTSEL =0; // Use timer T3 as the time base for PWM generation
    
                            //((PR2 + 1) * duty cycle)/100 
    OC2RS = 12500;        // Ton constant
    OC2CONbits.ON = 1;    // Enable OC1 module

    IFS0bits.T3IF = 0;      //Reset timer T3 interrupt flag
    IPC3bits.T3IP = 2;      //Interrupt priority (must be in range[1..6])
    IEC0bits.T3IE = 1;      //Enable timer T3 interrupts

    IFS0bits.T1IF = 0;      //Reset timer T1 interrupt flag
    IPC1bits.T1IP = 3;      //Interrupt priority (must be in range[1..6])
    IEC0bits.T1IE = 1;      //Enable timer T1 interrupts

    EnableInterrupts();
}

void _int_(4) isr_T1(void)
{
    putChar('1');
    AD1CON1bits.ASAM=1;
    IFS0bits.T1IF = 0;  
}

void _int_(12) isr_T3(void)
{
    putChar('2');
    send2displays(voltage);
    IFS0bits.T3IF = 0;  
}

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

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
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