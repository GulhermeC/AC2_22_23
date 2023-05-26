#include <detpic32.h>

void configUart(unsigned int baud, char parity, unsigned int stopbits);
void putc(char byte);
char getc(void);

typedef struct
{
    char mem[100];  // Storage Area
    int nchar;      // Number of characters to be transmitted
    int posrd;      // Position of the next character to be transmitted
} t_buf;

volatile t_buf txbuf;

int main(void){

    // port RC14 as output
    TRISCbits.TRISC14 = 0;
    configUart(115200, 'N', 1);
    IPC8bits.U2IP = 2; // prioridade da interrupção (1 a 6)

    U2STAbits.UTXISEL = 0; // Interrupts set when Output FIFO has space for next char
    U2STAbits.URXISEL = 0; // Interrupts set when Input FIFO has at least one char
    
    IEC1bits.U2TXIE = 0; // desativa pedidos de interrupção na transmissão
    IEC1bits.U2RXIE = 1; // ativa pedidos de interrupção na receção

    IFS1bits.U2RXIF = 0; // limpar pedido de interrupção do lado rx
    EnableInterrupts();
    while(1){};
    return 0;
}

void _int_(32) isr_UART2(){
    char cc;
    if(IFS1bits.U2TXIF == 1) {
        if(txbuf.nchar > 0){
            U2TXREG = txbuf.mem[txbuf.posrd];
            txbuf.posrd++;
            txbuf.nchar--;
        }else{
            IEC1bits.U2TXIE == 0;
        }
    }
    IFS1bits.U2TXIF = 0; // limpar pedido de interrupção do lado rx
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

void putstrInt(char *str)
{
    while(txbuf.nchar > 0); // Wait while the buffer is not empty
    while(*str != 0){
        txbuf.mem[txbuf.nchar]= *str;
        txbuf.nchar++;

    }
    txbuf.posrd = 0;
    IEC1bits.U2TXIE = 0; // desativa pedidos de interrupção na transmissão
}