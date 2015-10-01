/*---------------------------------------------------------------------------*/
Uses an ETIMER and clock_seconds() to show the number of seconds the system is
up
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
PROCESS(my_process, "Etimer");
AUTOSTART_PROCESSES(&my_process);
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
