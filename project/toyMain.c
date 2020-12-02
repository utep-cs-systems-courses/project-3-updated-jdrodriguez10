#include <msp430.h>
#include <libTimer.h>
#include "led.h"
#include "buzzer.h"
#include "switches.h"
#include "stateMachines.h"
#include "lcdutils.h"
#include "lcddraw.h"

static short previous_state = 0;
short redrawScreen = 1;
u_char centerWidth = screenWidth/2 + 1;
u_char centerHeight = screenHeight/2 +1;

int isNotNegative(int value);

void draw_diamond(u_char col, u_char row, u_char size, u_int color){
  for (u_char r = 0; r < size; r++){
    for (u_char c = 0; c <= r; c++){
      drawPixel(col-c, row-r-1, color);         //lower left
      drawPixel(col-c, row+r-(2*size), color);  //top left
      drawPixel(col+c, row-r-1, color);         //lower right
      drawPixel(col+c, row+r-(2*size), color);  //top right
    }
  }
}

//p2 interrupt handler and counter that sets redrawScreen=1 every sec
void wdt_c_handler()
{
  static int s1Count = 0;
  static int s2Count = 0;
  static int sCount = 0;
  static int secCount = 0;
  
  if (super_state != 3 && previous_state != super_state) redrawScreen = 1;
  if(super_state == 1){
    if (++ s1Count == 125) {
      state_advance();
      s1Count = 0;
    }
  } else if (super_state == 2) {
    if ( (++ s2Count % 25) == 0) buzzer_advance();
    if (s2Count == 250){
      state_advance();
      s2Count = 0;
    }
  } else{
    state_advance();
  }
}

void main()
{
  led_init();
  P1OUT |= LED_GREEN;
  configureClocks();
  buzzer_init();
  switch_init();
  lcd_init();
  enableWDTInterrupts();       /* enable periodic interrupt */
  or_sr(0x08);                 /* GIE*/
  clearScreen(COLOR_BLACK);

  if (isNotNegative(6) != 0) {
    drawString5x7(0, 0, "6 is Not Negative", COLOR_LIME_GREEN, COLOR_BLACK);
  }
  if (isNotNegative(-1) == 0) {
    drawString5x7(0, 10, "-1 is Negative", COLOR_GOLD, COLOR_BLACK);
  }
  while(1) {
    if (redrawScreen = 1) {
      redrawScreen = 0;
      static char color_state = 0;

      switch(super_state){
      case 0: drawString5x7(centerWidth/2, screenHeight -10, "Hello World!", COLOR_WHITE, COLOR_BLACK); break;
      case 1:
	lcd_state(COLOR_GREEN);
	previous_state = 1;
	break;
      case 2:
	lcd_state(COLOR_RED);
	previous_state = 2;
	break;
      case 3:
	previous_state = 3;
	break;
      case 4:
	if (previous_state != 4) lcd_clear();
	previous_state = 4;
	break;
      }
    }
    P1OUT &= ~LED_GREEN;
    or_sr(0x10);
    P1OUT |= LED_GREEN;
  }
}
