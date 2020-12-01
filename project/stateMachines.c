#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"
#include "switches.h"

static short freq = 500;
static short state2_status = 1;

char super_state = 0;

// turns on/off red led
char turn_red_on()
{
  static char stateS1 = 1;
  switch (stateS1) {
  case 0:
    red_on = 1;
    buzzer_set_period(1000);
    stateS1 = 1;
    break;
  case 1:
    red_on = 0;
    buzzer_set_period(0);
    stateS1 = 0;
    break;
  }
  return 1;
}


// Translated to assembly
/*
// increases/decreases pitch
void buzzer_advance(){
  if (state2_status) freq += 225;
  else freq -= 450;
  short cycles = 2000000/freq;
  buzzer_set_period(cycles);
}
*/


void buzzer_advance();


short get_period(short freq){
  return 2000000/freq;
}


void state_up()
{
  state2_status = 1; red_on = 1;
  led_changed = 1; led_update();
}


void state_down()
{
  state2_status = 0; red_on = 0;
  led_changed= 1; led_update();
}


//leds are on 1/x
void dim_leds(char x){
  static short dimCount = 0;
  switch(dimCount % x){
  case 0: red_on = 1;  dimCount++; break;
  case 1: red_on = 0;  dimCount++; break;
  default: red_on = 0; dimCount++; break;
  }
  led_changed = 1;
  led_update();
}


// turns on/off leds
char state1()
{
  char changed = 0;
  changed = turn_red_on;
  led_changed = changed;
  led_update();
}


// pitch rises from 500-5000Hz with red led on
// pitch decreases from 5000-500Hz with green led on
char state2()
{
  static short stateS2 = 0;
  switch(stateS2){
  case 0:
  case 1: state_up(); stateS2++; break;
  case 2: state_down(); stateS2 = 0;
  default: break;
  }
  return 1;
}


// changes from 0% dim to 50% to 33% to 13% to 6% and buzzer pitch decreases with it
char state3()
{
  static short s3Counter = 0;
  static short stateS3 = 0;
  if (++s3Counter == 125) { stateS3++; s3Counter = 0;}
  switch (stateS3){
  case 0: dim_leds(1); buzzer_set_period(0); break;
  case 1: dim_leds(2); buzzer_set_period(500); break;
  case 2: dim_leds(3); buzzer_set_period(1500); break;
  case 3: dim_leds(4); buzzer_set_period(6000); break;
  case 4: dim_leds(8); buzzer_set_period(0); stateS3 = 0; break;
  }
}


// turns off leds and buzzer
char state4(){
  buzzer_set_period(0);
  red_on = 0;
  green_on = 0;
  led_changed = 1;
  led_update();
  return 1;
}


// Translated into assembly
/*
// changes states
void state_advance()
{
  char changed = 0;
  switch(super_state){
  case 1:
    changed = state1(); // turn on/off leds and buzzer.
    break;
  case 2:
    changed = state2(); // siren
    break;
  case 3:
    changed = state3(); // diming leds
    break;
  case 4:
    changed = state4(); // turns leds and buzzer off.
    break;
  }
  led_changed = changed;
  led_update();
}
*/
