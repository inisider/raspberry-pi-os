#include "uart.h"
#include "utils.h"

#define AMOUNT_OF_CPU	(4)
static unsigned int curr_proc = 0;

void kernel_main(void)
{
	unsigned int proc_id;

	proc_id = get_proc_id();

	if (!proc_id)
		uart_init();

	while (curr_proc != proc_id) {}
	
	uart_send_string("Hello, processor ");
	uart_send(proc_id + '0');
	uart_send_string("\r\n");

	curr_proc++;

	if (!proc_id) {
		while (curr_proc != AMOUNT_OF_CPU) {}

		while (1) {
			uart_send(uart_recv());
		}
	}
}
