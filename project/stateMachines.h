#ifndef stateMachine_included
#define stateMachine_included

extern char lcdState;

void lcd_update();
void lcd_state(int color);
void lcd_clear();
void state_advance();
void dim_leds(char x);
char turn_green_on();
char turn_red_on();
void buzzer_advance();
void state_up();
void state_down();

#endif // included
