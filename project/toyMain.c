#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"
#include "switches.h"
#include "stateMachines.h"
#include "lcdutils.h"
#include "lcddraw.h"

short redrawScreen = 1;

int isNotNegative(int value);

void wdt_c_handler()
{
  static int secCount = 0;
  secCount ++;
  if (secCount == 1) {
    secCount = 0;
    state_advance();
    redrawScreen = 1;
  }
}

void main()
{
  configureClocks();           /* setup master oscillator, CPU & peripheral clocks */
  led_init();
  buzzer_init();
  switch_init();
  lcd_init();
  enableWDTInterrupts();       /* enable periodic interrupt */
  or_sr(0x08);                 /* GIE*/
  clearScreen(COLOR_BLACK);
}
