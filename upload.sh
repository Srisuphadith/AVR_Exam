fileName=$1
avrdude -c usbasp -p m328p -b 11250 -U flash:w:./${fileName}.hex