/*---------------------------------------------------------------------------*/
Example of CTIMER and its callbacks
/*---------------------------------------------------------------------------*/
#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
/*---------------------------------------------------------------------------*/
PROCESS(my_process, "Ctimer");
AUTOSTART_PROCESSES(&my_process);
/*---------------------------------------------------------------------------*/
static struct ctimer ctim;

static void callback(void* data) {
    printf("%s\n", (char*) data);
    ctimer_set(&ctim, CLOCK_SECOND, callback, "Hello again...");
}   

PROCESS_THREAD(my_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();
  
  while(1) {
      ctimer_set(&ctim, CLOCK_SECOND, callback, "Hello Callback");
      PROCESS_WAIT_EVENT();
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
