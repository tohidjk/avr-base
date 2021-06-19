
avr-gcc -mmcu=atmega8 -DF_CPU=8000000UL -Os -o _test.elf _test.c
avr-size _test.elf
del _test.elf
pause

