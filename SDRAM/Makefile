sdram.bin : head.S key_leds.c
	arm-linux-gcc -g -c -o head.o head.S
	arm-linux-gcc -g -c -o key_leds.o key_leds.c
	arm-linux-ld -Ttext 0x30000000 head.o key_leds.o -o sdram_elf
	arm-linux-objcopy -O binary -S sdram_elf sdram.bin
	arm-linux-objdump -D -m arm sdram_elf > sdram.dis
clean :
	rm -f sdram.dis sdram.bin sdram_elf *.o
