/*---------------------------------------------------------------------------
Example of RTIMER. This works for small waits
---------------------------------------------------------------------------*/
#include "contiki.h"
#include "rtimer.h"
#include <stdio.h>
#include "dev/leds.h"
#include "sys/rtimer.h"
/*---------------------------------------------------------------------------*/
PROCESS(my_process, "Rtimer");
AUTOSTART_PROCESSES(&my_process);
/*---------------------------------------------------------------------------*/

static void callback(struct rtimer* rt, void* data) {
    printf("%s\n", (char*) data);
    rtimer_set(rt, RTIMER_TIME(rt)+(RTIMER_SECOND/2), 1, callback, "See you in 500 miliseconds...");
}

PROCESS_THREAD(my_process, ev, data)
{
  PROCESS_EXITHANDLER(goto exit;)
  PROCESS_BEGIN();

  static struct rtimer rtim;
  
  while(1) {
      // third argument is unused (http://dak664.github.io/contiki-doxygen/a01629.html#ga6a437542171f242142c80b4c3d8e4bfa)
      rtimer_set(&rtim, RTIMER_TIME(&rtim)+(RTIMER_SECOND/10), 10, callback, "Realtime only works for small waits!");
      PROCESS_WAIT_EVENT();
  }

 exit:
  printf("bye bye!\n");
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
