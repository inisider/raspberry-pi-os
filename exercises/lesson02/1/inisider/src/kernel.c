#include "printf.h"
#include "utils.h"
#include "mini_uart.h"

void kernel_main(void)
{
	int el = get_el();

	if (el == 2) {
		uart_init();
	}

	init_printf(0, putc);

	printf("Exception level: %d \r\n", el);

	if (el == 1) {
		while (1) {
			uart_send(uart_recv());
		}
	}
}
