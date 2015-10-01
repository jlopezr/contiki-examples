#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(test_sensor_process, "Test light sensor");
AUTOSTART_PROCESSES(&test_sensor_process);
/*---------------------------------------------------------------------------*/
static uint8_t active;
PROCESS_THREAD(test_sensor_process, ev, data)
{
  PROCESS_BEGIN();
  SENSORS_ACTIVATE(light_sensor);

  while(1) {
      printf("Light: %d\n", light_sensor.value(0));
  }
  SENSORS_DEACTIVATE(light_sensor);
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
