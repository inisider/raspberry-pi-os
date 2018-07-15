#include "utils.h"
#include "peripherals/uart.h"
#include "peripherals/gpio.h"

void uart_send ( char c )
{
	while(1) {
		if(!(get32(UART_FR) & (1 << 5))) 
			break;
	}
	put32(UART_DR, c);
}

char uart_recv ( void )
{
	while(1) {
		if(!(get32(UART_FR) & (1 << 4)))
			break;
	}
	return(get32(UART_DR) & 0xFF);
}

void uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		uart_send((char)str[i]);
	}
}

void uart_init ( void )
{
	unsigned int selector;

	selector = get32(GPFSEL1);
	selector &= ~(7<<12);                   // clean gpio14
	selector |= 4<<12;                      // set alt5 for gpio14
	selector &= ~(7<<15);                   // clean gpio15
	selector |= 4<<15;                      // set alt5 for gpio15
	put32(GPFSEL1,selector);

	put32(GPPUD,0);
	delay(150);
	put32(GPPUDCLK0,(1<<14)|(1<<15));
	delay(150);
	put32(GPPUDCLK0,0);

	put32(UART_CR, 0);	// Disable uart

	// baud divisor = UARTCLK / (16 * baud_rate)
	// baud divisor = 48 * 10^6 / (16 * 115200) = 26.0416666667
	// integer part = 26
	// fractional part = (int) ((0.0416666667 * 64) + 0.5) = 3
	put32(UART_IBRD, 26);                 // baud rate divisor, integer part
	put32(UART_FBRD, 3);                  // baud rate divisor, fractional part

	// FEN is seted => transmit and receive FIFO buffers are enabled
	// WLEN is b11 => word length is 8 bits.
	put32(UART_LCRH, (1 << 4) | (3 << 5));

	put32(UART_IMSC, 0); // Disable Interrupts
	put32(UART_CR, 1 | (1 << 8) | (1 << 9)); // Enable UART + Tx + Rx
}
