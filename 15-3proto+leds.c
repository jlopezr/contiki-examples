/*
#include "contiki.h"
#include <stdio.h>
#include "sys/timer.h"
#include "dev/leds.h"

#include "dev/watchdog.h"
*/
  
#include "contiki.h"
#include "sys/timer.h"
#include "dev/leds.h"
#include "pt.h"

//TODO Probar a quitar los printf y porque peta?
//     si quito el watchdog va
//     que hace el PT_THREAD ???
//     pq le funciona sin el PROCESS_PAUSE() ???

/*---------------------------------------------------------------------------*/
PROCESS(test_threads_process, "Test 2 Threads");
AUTOSTART_PROCESSES(&test_threads_process);

struct data {
  struct pt pt;
  struct timer et;
  int counter;
  int threadNumber;
  int led;
  int seconds;
  char state;
};
/*---------------------------------------------------------------------------*/
//static char
//led_pthread(struct data *f)
static PT_THREAD(led_pthread(struct data *f))
{
  PT_BEGIN(&f->pt);
  f->counter = 0;
  timer_set(&f->et, CLOCK_SECOND * f->seconds);

  //printf("Start thread %d\r\n",f->threadNumber);
  while(1) {
      leds_toggle(f->led);
      PT_WAIT_UNTIL(&f->pt,timer_expired(&f->et));
      //printf("Thread %d Counter %d\r\n",f->threadNumber, f->counter++);
      timer_reset(&f->et);
  }
  //printf("End thread\r\n");

  PT_END(&f->pt);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_threads_process, ev, data)
{
  static struct data th1;
  static struct data th2;
  static struct data th3;

  PROCESS_BEGIN();

//  watchdog_stop();

  //printf("Start process\r\n");
  PT_INIT(&th1.pt);
  th1.threadNumber = 1;
  th1.led = LEDS_RED;
  th1.seconds = 1;
  th1.state = 0;
  PT_INIT(&th2.pt);
  th2.threadNumber = 2;
  th2.led = LEDS_GREEN;
  th2.seconds = 1;
  th2.state = 0;
  PT_INIT(&th3.pt);
  th3.threadNumber = 3;
  th3.led = LEDS_BLUE;
  th3.seconds = 1;
  th3.state = 0;

  while(1) {
      led_pthread(&th1);
      led_pthread(&th2);
      led_pthread(&th3);
      //PROCESS_WAIT_EVENT(); //<- This is mandatory for listening the timer event.
      //PROCESS_YIELD(); // <-- Solo se ejecuta el primer toggle --- nunca vuelve
      //PROCESS_PAUSE(); // <-- con esto va. lo que hace es añade un evento de continuar y lo espera
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
