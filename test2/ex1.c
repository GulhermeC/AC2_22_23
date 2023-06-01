/*
Exit 0C2
FREQ 150Hz
Duty-Cycle dependent of switches DS4 and DS1 -> ports RB3 and RB0
RB3: 0, RB0: 1 - 25% - start
RB3: 1, RB0: 0 - 70%
verify every 250 microseconds
TIMER T2 as reference -> config should permit generation of PWM signal
    at max resolution


*/

#include <detpic32.h>

void delay(int us){
    resetCoreTimer();
    while (readCoreTimer() < us * 20);
}

int main(void){
    TRISB = TRISB | 0x0009; // Switches 0x1001

    T2CONbits.TCKPS = 2;    // 1:4 prescaler
    PR2 = 33332;            // 150 Hz
    TMR2 = 0;               // Reset timer T2 count register
    T2CONbits.TON = 1;      // Enable timer (last command)

    IPC2bits.T2IP = 2;      // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1;      // Enable timer T2 interrupts
    IFS0bits.T2IF = 0;      // Reset timer T2 interrupt flag
    EnableInterrupts();

    OC2CONbits.OCM = 6;
    OC2CONbits.OCTSEL = 0;
    OC2RS = 8333;
    OC2CONbits.ON = 1;

    while(1) {
        if ((PORTB & 0X000F) == 1){
            OC2RS = 8333;
        } else if ((PORTB & 0X000F) == 8) {
                OC2RS = 23332;
        }
        delay(250);
    }
    return 0;
}

// Interrupt Service Routine (interrupt handler)
void _int_(8) isr_adc(void) {
    IFS0bits.T2IF = 0; // Reset Timer T2 interrupt flag
}