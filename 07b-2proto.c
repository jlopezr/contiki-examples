#include "contiki.h"
#include <stdio.h>
#include "sys/etimer.h"

//TODO Usar el evento para esperar en vez de etimer_expired

/*---------------------------------------------------------------------------*/
PROCESS(test_threads_process, "Test 2 Threads");
AUTOSTART_PROCESSES(&test_threads_process);

struct data {
  struct pt pt;
  struct etimer et;
  int counter;
  int threadNumber;
};
/*---------------------------------------------------------------------------*/
static char
led_pthread(void *ptr)
{
  struct data *f = ptr;

  PT_BEGIN(&f->pt);
  f->counter = 0;
  etimer_set(&f->et, CLOCK_SECOND * f->threadNumber);

  printf("Start thread %d\r\n",f->threadNumber);
  while(1) {
      PT_WAIT_UNTIL(&f->pt,etimer_expired(&f->et));
      printf("Thread %d Counter %d\r\n",f->threadNumber, f->counter++);
      etimer_reset(&f->et);
  }
  printf("End thread\r\n");

  PT_END(&f->pt);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_threads_process, ev, data)
{
  static struct data th1;
  static struct data th2;

  PROCESS_BEGIN();

  printf("Start process\r\n");
  PT_INIT(&th1.pt);
  th1.threadNumber = 1;
  PT_INIT(&th2.pt);
  th2.threadNumber = 2;

  while(1) {
      led_pthread(&th1);
      led_pthread(&th2);
      PROCESS_WAIT_EVENT(); //<- This is mandatory for listening the timer event.
  }
  printf("End process\r\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
