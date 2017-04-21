#include "contiki.h"
#include <stdio.h>
#include "sys/etimer.h"
#include "dev/leds.h"

PROCESS(test_threads_process, "Test 1 Thread");
AUTOSTART_PROCESSES(&test_threads_process);

struct data {
  struct pt pt;
  struct etimer et;
  int i;
  int contador;
};
/*---------------------------------------------------------------------------*/
static char
my_pthread(void *ptr)
{
  struct data *f = ptr;
  PT_BEGIN(&f->pt);
  f->i = 0;
  f->contador = 0;
  etimer_set(&f->et, CLOCK_SECOND);

  while(1) {
      PT_WAIT_UNTIL(&f->pt,etimer_expired(&f->et));
      leds_toggle(LEDS_RED);
      (f->i)++;
      if((f->i)%2==0) {
        printf("%d\n",(f->contador)++);
      }
      etimer_reset(&f->et);
  }

  PT_END(&f->pt);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_threads_process, ev, data)
{
  static struct data th1;
  PROCESS_BEGIN();

  PT_INIT(&th1.pt);
  
  while(1) {
      my_pthread(&th1);
      PROCESS_WAIT_EVENT();
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
