#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(test_button_process, "Test button");
AUTOSTART_PROCESSES(&test_button_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_button_process, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(button_sensor);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&
			     data == &button_sensor);
    leds_toggle(LEDS_ALL);
    printf("Button pressed!\n");
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
