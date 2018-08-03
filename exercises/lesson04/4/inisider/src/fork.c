#include "mm.h"
#include "sched.h"
#include "entry.h"
#include "printf.h"

int copy_process(unsigned long fn, unsigned long arg)
{
	preempt_disable();
	struct task_struct *p;
	struct task_struct *last;

	// printf("copy_process 1\r\n");

	p = (struct task_struct *) get_free_page();
	if (!p)
		return 1;

	// printf("copy_process 2\r\n");

	p->next = 0;
	p->priority = current->priority;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; //disable preemtion until schedule_tail

	p->cpu_context.x19 = fn;
	p->cpu_context.x20 = arg;
	p->cpu_context.pc = (unsigned long)ret_from_fork;
	p->cpu_context.sp = (unsigned long)p + THREAD_SIZE;
	// int pid = nr_tasks++;

	for (last = head; last->next != 0; last = last->next) {
		// printf("pid = %d\r\n", nr_tasks++);
	}
	last->next = p;

	preempt_enable();
	return 0;
}
