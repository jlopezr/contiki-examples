#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
#include "dev/button-sensor.h"
/*---------------------------------------------------------------------------*/
PROCESS(counter, "Counter");
PROCESS(button, "Button Control");
AUTOSTART_PROCESSES(&counter,&button);
/*---------------------------------------------------------------------------*/
static char active = 0; // Shared between the two processes
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(counter, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  
  static struct etimer etim;
  static int counter = 0;

  while(1) {
    PROCESS_WAIT_UNTIL(active);
    printf("Counter: %u\n", counter++); 
    etimer_set(&etim, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etim));
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(button, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event && data == &button_sensor);
    leds_toggle(LEDS_ALL);
    printf("Button pressed!\n");
    active = !active;
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
