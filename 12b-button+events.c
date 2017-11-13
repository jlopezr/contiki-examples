/*---------------------------------------------------------------------------
 * Uses an user defined process to signal a process from another.
 * In this version the active status is stored in button process and passed 
 * through the event data pointer
 *
 * Based on http://senstools.gforge.inria.fr/doku.php?id=contiki:examples#event_post
 * ---------------------------------------------------------------------------*/
#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
#include "dev/button-sensor.h"
/*---------------------------------------------------------------------------*/
PROCESS(counter, "Counter");
PROCESS(button, "Button Control");
AUTOSTART_PROCESSES(&counter,&button);
/*---------------------------------------------------------------------------*/
static process_event_t event_data_ready; // Shared between the two processes
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(counter, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  
  static struct etimer etim;
  static int counter = 0;
  static int active = 0;
  
  while(1) {
    //PROCESS_WAIT_EVENT_UNTIL(ev == event_data_ready);
    PROCESS_WAIT_EVENT(); // <--- Button events are received for ALL processes
    printf("[Counter] Event number %d\n", ev); 
    if(ev == event_data_ready) {
        active = (int)data;
        if(active) {
            printf("Starting timer\n");
            etimer_set(&etim, CLOCK_SECOND);
        } else {
            printf("Stopping timer\n");
            etimer_stop(&etim);
        }
    } else if(ev == PROCESS_EVENT_TIMER && etimer_expired(&etim)) {
        printf("Counter: %u\n", counter++); 
        etimer_set(&etim, CLOCK_SECOND);
    } 
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(button, ev, data)
{
  static int active = 0;

  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  // Allocates a new event number
  event_data_ready = process_alloc_event();

  while(1) {
    //PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
    PROCESS_WAIT_EVENT();
    printf("[Button] Event number %d\n", ev);                           
    if(ev == sensors_event && data == &button_sensor) { 
      leds_toggle(LEDS_ALL);
      active = !active;
      process_post(&counter, event_data_ready, (void*)active);
      printf("Button pressed!\n");
    } 
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
