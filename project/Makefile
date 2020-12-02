# makfile configuration
CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:toy.elf

#additional rules for files
toy.elf: ${COMMON_OBJECTS} toyMain.o wdt_handler.o led.o switches.o buzzer.o p2_interrupt_handler.o stateMachines.o state_advance.s buzzer_advance.s is_not_negative.s
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd

load: toy.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
