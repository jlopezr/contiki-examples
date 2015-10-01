#include "contiki.h"
#include "sys/timer.h"
#include "dev/leds.h"
#include "pt.h"

//TODO PORQUE ESTE SI FUNCIONA SIN EL PROCESS_PAUSE ? 

/*---------------------------------------------------------------------------*/
PROCESS(test_etimer_process, "Test ET");
AUTOSTART_PROCESSES(&test_etimer_process);

static struct leds{
  int led;
  int clock;
  struct timer timer;
  struct pt *pt;
};

static struct leds led1,led2, led3;


/*---------------------------------------------------------------------------*/
static PT_THREAD(ledprocess(struct leds *leds))
{
  
  PT_BEGIN (leds -> pt);
  
  while(1) {
    leds_toggle(leds -> led);
    timer_set(&leds->timer,leds->clock * CLOCK_SECOND);
    PT_WAIT_UNTIL(leds->pt,timer_expired(&leds -> timer));

  }      
 PT_END(leds -> pt);
}
/*-----------------------------------------------------------------------------*/
PROCESS_THREAD(test_etimer_process, ev, data)
{
  PROCESS_BEGIN();

  led1.led=LEDS_BLUE;
  led1.clock=1;
  PT_INIT (led1.pt);

  led2.led=LEDS_GREEN;
  led2.clock=2;
  PT_INIT (led2.pt);

  led3.led=LEDS_RED;
  led3.clock=3;
  PT_INIT (led3.pt);

  while(1){
    ledprocess(&led1);
    ledprocess(&led2);
    ledprocess(&led3);
    //PROCESS_WAIT_EVENT();
    PROCESS_PAUSE();
  }
  PROCESS_END();
}

