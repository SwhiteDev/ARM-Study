key_leds.bin : crt.S key_leds.c
	arm-linux-gcc -g -c -o crt.o crt.S
	arm-linux-gcc -g -c -o key_leds.o key_leds.c
	arm-linux-ld -Ttext 0x00000000 -g crt.o key_leds.o -o key_leds_elf
	arm-linux-objcopy -O binary -S key_leds_elf key_leds.bin
	arm-linux-objdump -D -m arm key_leds_elf > key_leds.dis
clean : 
	rm -f key_leds.dis key_leds.bin key_leds_elf *.o
