fileName=$1
#avr-gcc -mmcu=atmega328p -Os -o ./${fileName}.elf ./${fileName}.c ./delay_module.c ./pin_module.c ./lcd_module.c \
#    ./keypad_module.c ./analog_module.c ./ultrasonic_module.c ./mcp_module.c
avr-gcc -mmcu=atmega328p -c ${fileName}.c -o ${fileName}.o
avr-gcc -mmcu=atmega328p ${fileName}.o -Llib -lmy-avr-lib -o ${fileName}.elf
avr-objcopy -O ihex -R.eeprom ./${fileName}.elf ./${fileName}.hex