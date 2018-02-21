#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
/*---------------------------------------------------------------------------*/
PROCESS(example_abc_process, "RIME ABC example");
AUTOSTART_PROCESSES(&example_abc_process);
/*---------------------------------------------------------------------------*/
static void
abc_recv(struct abc_conn *c)
{
  printf("abc message received '%s'\n", (char *)packetbuf_dataptr());
}
static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_abc_process, ev, data)
{
  static struct etimer et;
  static char buffer[20];
  static int len;
  static int counter = 0;

  PROCESS_EXITHANDLER(abc_close(&abc);)
  PROCESS_BEGIN();

  abc_open(&abc, 128, &abc_call);

  while(1) {
    etimer_set(&et, CLOCK_SECOND);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
    len = sprintf(buffer, "Hello %d from %d.%d", counter, rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);
    packetbuf_copyfrom(buffer, len);
    abc_send(&abc);
    printf("abc message sent '%s'\n", buffer);

    counter++;
    if(counter == 10) {
        counter = 0;
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
