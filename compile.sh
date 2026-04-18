fileName=$1
avr-gcc -mmcu=atmega328p -Os -o ./${fileName}.elf ./${fileName}.c ./delay_module.c ./pin_module.c ./lcd_module.c ./keypad_module.c
avr-objcopy -O ihex -R.eeprom ./${fileName}.elf ./${fileName}.hex