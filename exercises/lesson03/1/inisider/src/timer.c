#include "utils.h"
#include "printf.h"
#include "peripherals/timer.h"

const unsigned int local_timer_interval = 19200000;
const unsigned int interval = 200000;
unsigned int curVal = 0;

void timer_init ( void )
{
	curVal = get32(TIMER_CLO);
	curVal += interval;
	put32(TIMER_C1, curVal);
}

void handle_timer_irq( void ) 
{
	curVal += interval;
	put32(TIMER_C1, curVal);
	put32(TIMER_CS, TIMER_CS_M1);
	printf("Timer interrupt received\n\r");
}

void local_timer_init( void )
{
	put32(LTIMER_CTRL, (local_timer_interval | LTIMER_CNTRL_INIT_VAL));
}

void handle_local_timer_irq( void )
{
	printf("handle_local_timer_irq\n\r");
	put32(LTIMER_CLR, LTIMER_CLR_ACK);
}