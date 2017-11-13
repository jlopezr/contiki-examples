/*---------------------------------------------------------------------------
One process counts and the other controls the button, switching on and off
the counter.
Have a look in line 54 that makes the soft not working
---------------------------------------------------------------------------*/
#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "Test button + Thread");
AUTOSTART_PROCESSES(&test_button_process);
/*---------------------------------------------------------------------------*/
struct data {
  struct pt pt;
  struct etimer et;
  int counter;
  char active;
};
/*---------------------------------------------------------------------------*/
static char
counter_pthread(void *ptr)
{
  struct data *f = ptr;

  PT_BEGIN(&f->pt);
  f->counter = 0;
  etimer_set(&f->et, CLOCK_SECOND);

  printf("Start thread\r\n");
  while(1) {
      PT_WAIT_UNTIL(&f->pt,f->active && etimer_expired(&f->et));
      printf("Counter %d\r\n",f->counter++);
      etimer_reset(&f->et);
  }
  printf("End thread\r\n");

  PT_END(&f->pt);
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_button_process, ev, data)
{
  static struct data th1;
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  PT_INIT(&th1.pt);
  th1.active = 0;

  while(1) {
    counter_pthread(&th1); 
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor); //<---- This line makes the process wait for events, so
    leds_toggle(LEDS_ALL);                                                   //      the counter prototh is not called continuosly,
    th1.active = !th1.active;                                                //      only when button is pressed. => Should be a process!
    printf("Button pressed!\n");
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
