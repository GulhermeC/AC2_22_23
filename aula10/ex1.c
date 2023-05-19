#include <detpic32.h>

int main(void){
    
    // Configure UART2:
    // 1 - Configure BaudRate Generator
    // BRGH High Baug Rate Enable bit (1 = High Speed | 0 = Standard Speed)
    U2MODEbits.BRGH = 0;
    U2BRG=10;
    
    
    // 2 - Configure number of data bits, parity and number of stop bits
    //     (see U2MODE register)
    //PDSEL Parity and Data Selection (0) = 8bit data no parity
    U2MODEbits.PDSEL = 0;
    //STSEL Stop Selection bit (1 = 2 Stop bits) | (0 = 1 Stop bit)
    U2MODEbits.STSEL = 0;

    // 3 - Enable the transmitter and receiver modules
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    // 4 - Enable UART2 (see register)
    U2MODEbits.ON=1;
}
