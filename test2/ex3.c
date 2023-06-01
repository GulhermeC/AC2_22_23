/*
1. UART2 -> 9600 bps, odd parity, 8 data bits, 2 stop bits
2. Receber 'U' -> incrementar contador modulo 16
    Resultado deve ser mostrado nos 4 LED ligados RE4 a RE1 
    Start -> valor maximo
3. Receber 'R' -> repor valor minimo
    Resultado deve ser mostrado nos 4 LED
    Transmitir a string RESET

Rececao de caracter deve ser feito por interruptcao
Transmitir deve ser feito por polling
*/

#include <detpic32.h>

void putc(char byte2send) {
    while (U2STAbits.UTXBF == 1);
    U2TXREG = byte2send;
}

void putStr(char* str) {
    int i;
    for (i = 0; str[i] != '\0'; i++){
        putc(str[i]);
    }
}

volatile unsigned char counter = 15;

int main(void) {
    TRISE = TRISE & 0xFFE1;
    LATE = (LATE & 0xFFE1) | counter << 1;

    U2BRG = 129;                // 9600 baudrate
    U2MODEbits.BRGH = 0;
    U2MODEbits.PDSEL = 2;       // odd
    U2MODEbits.STSEL = 1;       // 2 stop bits
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;

    IPC8bits.U2IP = 2;          // Interrupt priority (must be in range [1..6])
    IEC1bits.U2TXIE = 0;        // Disable U2 transmiter interrupts
    IEC1bits.U2RXIE = 1;        // Enable U2 receiver interrupts
    IFS1bits.U2RXIF = 0;        // Reset U2 interrupt flag

    EnableInterrupts();
    
    while(1);
    return 0;
}

// Interrupt Service Routine (interrupt handler)
void _int_(32) isr_receive_UART2(void) {
    if (IFS1bits.U2RXIF == 1) {
        char byte;
        byte = U2RXREG;
        if (byte == 'U') {
            counter++;
            if (counter == 16){
                counter = 0;
            }
        }
        if (byte == 'R') {
            counter = 0;
            putStr("RESET");
        }
        LATE = (LATE & 0xFFE1) | counter << 1;
        IFS1bits.U2RXIF = 0;        // Reset Flag
    }
}