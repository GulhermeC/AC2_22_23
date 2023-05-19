#include <detpic32.h>

void delay(unsigned int invervals);
void putc(char byte);
char getc(void);
void configUart(unsigned int baud, char parity, unsigned int stopbits);

int main(void){

    configUart(115200,'N',1);
    while(1){
        char c = getc();
        putc(c);
    }
    return 0;
}

char getc(void){
    while(U2STAbits.URXDA == 0);
    return U2RXREG;
}

void putc(char byte){
    while(U2STAbits.UTXBF == 1);
    U2TXREG = byte;
}

void delay(unsigned int intervals){
    volatile unsigned int i;
    for(;intervals!=0;intervals--){
        for(i=4988;i!=0;i--){}
    }
}


void configUart(unsigned int baud, char parity, unsigned int stopbits)
{
    /// Configure UART2:
    // 1 - Configure BaudRate Generator
    // BRGH High Baug Rate Enable bit (1 = High Speed | 0 = Standard Speed)
    U2MODEbits.BRGH = 0;
    
    if(baud>=600 && baud <= 115200)
    {
        U2BRG=((20000000+8*baud)/(16*baud))-1;
    }else{
        U2BRG=10;
    }

    // 2 - Configure number of data bits, parity and number of stop bits
    //     (see U2MODE register)
    //PDSEL Parity and Data Selection (0) = 8bit data no parity
    if(parity=='E')
        U2MODEbits.PDSEL = 1;
    else if(parity=='O')
        U2MODEbits.PDSEL = 2;
    else
        U2MODEbits.PDSEL = 0;
    
    
    //STSEL Stop Selection bit (1 = 2 Stop bits) | (0 = 1 Stop bit)
    if(stopbits==2)
        U2MODEbits.STSEL = 1;
    else
        U2MODEbits.STSEL = 0;

    // 3 - Enable the transmitter and receiver modules
    U2STAbits.UTXEN = 1;
    U2STAbits.URXEN = 1;

    // 4 - Enable UART2 (see register)
    U2MODEbits.ON=1;

}