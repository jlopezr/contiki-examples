/*---------------------------------------------------------------------------*/
Uses two ETIMER and two PROCESSES to control two leds.
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
PROCESS(my_process, "Etimer");
PROCESS(my_process2, "Etimer 2");
AUTOSTART_PROCESSES(&my_process,&my_process2);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(my_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  
  static struct etimer etim;

  while(1) {
    printf("Uptime (seconds): %lu\n", clock_seconds());
    etimer_set(&etim, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etim));
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(my_process2, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  
  static struct etimer etim;

  while(1) {
    printf("EVEN Uptime (seconds): %lu\n", clock_seconds());
    etimer_set(&etim, CLOCK_SECOND*2);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etim));
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
