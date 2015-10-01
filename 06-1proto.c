/*---------------------------------------------------------------------------*/
Example of 1 PROTOTHREAD that counts
It is very important that the process always have PROCESS_WAIT_EVENT() if not
the watchdog timer is not reset and the system is halt :)
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include <stdio.h>
#include "sys/etimer.h"

/*---------------------------------------------------------------------------*/
PROCESS(test_threads_process, "Test 1 Thread");
AUTOSTART_PROCESSES(&test_threads_process);

struct data {
  struct pt pt;
  struct etimer et;
  int counter;
};
/*---------------------------------------------------------------------------*/
static char
my_pthread(void *ptr)
{
  struct data *f = ptr;

  PT_BEGIN(&f->pt);
  f->counter = 0;
  etimer_set(&f->et, CLOCK_SECOND);

  printf("Start thread\r\n");
  while(1) {
      PT_WAIT_UNTIL(&f->pt,etimer_expired(&f->et));
      printf("Counter %d\r\n",f->counter++);
      etimer_reset(&f->et);
  }
  printf("End thread\r\n");

  PT_END(&f->pt);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_threads_process, ev, data)
{
  static struct data th1;
  PROCESS_BEGIN();

  printf("Start process\r\n");
  PT_INIT(&th1.pt);

  while(1) {
      my_pthread(&th1);
      PROCESS_WAIT_EVENT(); //<--- If we remove this line, it does not work because nobody process the timer event
  }
  printf("End process\r\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
