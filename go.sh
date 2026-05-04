fileName=$1
avr-gcc -mmcu=atmega328p -c ./src/analog_module.c -o ./src/analog_module.o 
avr-gcc -mmcu=atmega328p -c ./src/delay_module.c -o ./src/delay_module.o 
avr-gcc -mmcu=atmega328p -c ./src/ds1307_module.c -o ./src/ds1307_module.o
avr-gcc -mmcu=atmega328p -c ./src/keypad_module.c -o ./src/keypad_module.o
avr-gcc -mmcu=atmega328p -c ./src/lcd_module.c -o ./src/lcd_module.o
avr-gcc -mmcu=atmega328p -c ./src/mcp_module.c -o ./src/mcp_module.o
avr-gcc -mmcu=atmega328p -c ./src/pin_module.c -o ./src/pin_module.o
avr-gcc -mmcu=atmega328p -c ./src/ultrasonic_module.c -o ./src/ultrasonic_module.o
echo "*********** complie .o success ***********"
avr-ar rcs ./lib/libmy-avr-lib.a ./src/analog_module.o ./src/delay_module.o ./src/keypad_module.o \
    ./src/lcd_module.o ./src/mcp_module.o ./src/pin_module.o ./src/ultrasonic_module.o ./src/ds1307_module.o
rm ./src/*.o
echo "*********** make lib success ***********"
avr-gcc -mmcu=atmega328p -c -Os ${fileName}.c -o ${fileName}.o
echo "*********** compile ${fileName}.c ***********"
avr-gcc -mmcu=atmega328p ${fileName}.o -Llib -lmy-avr-lib -o ${fileName}.elf
echo "*********** link ${fileName}.c with lib ***********"
avr-objcopy -O ihex -R.eeprom ./${fileName}.elf ./${fileName}.hex
echo "*********** convert to intel hex ***********"
avrdude -c usbasp -p m328p -B 2 -b 11250 -U flash:w:./${fileName}.hex
rm ${fileName}.o
rm ${fileName}.elf
rm ${fileName}.hex
