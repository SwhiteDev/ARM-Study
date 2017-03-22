#define GPFCON    (*(volatile unsigned long *) 0x56000050)
#define GPFDAT    (*(volatile unsigned long *) 0x56000054)

#define GPGCON    (*(volatile unsigned long *) 0x56000060)
#define GPGDAT    (*(volatile unsigned long *) 0x56000064)

/*
 *LED1,2,4
 */
#define GPF4_out    (1 << (4 * 2))
#define GPF5_out    (1 << (5 * 2))
#define GPF6_out    (1 << (6 * 2))

/*
 *KEY2,3,4
 */
#define GPF0_in    (0 << (0 * 2))
#define GPF2_in    (0 << (2 * 2))
#define GPG3_in    (0 << (3 * 2))

void Delay(volatile unsigned long dly)
{
	for(;dly > 0;dly--);
}

int main()
{
	unsigned long value_key = 0;
	GPFCON |= GPF4_out | GPF5_out | GPF6_out;
	GPFCON |= GPF0_in | GPF2_in;
	GPGCON |= GPG3_in;
	while(1)
	{
		value_key = GPFDAT;
		if(value_key & (1 << 0))
			GPFDAT |= (1 << 4);
		else
			GPFDAT &= ~(1 << 4);
		if(value_key & (1 << 2))
			GPFDAT |= (1 << 5);
		else
			GPFDAT &= ~(1 << 5);
		value_key = GPGDAT;
		if(value_key & (1 << 3))
			GPFDAT |= (1 << 6);
		else
			GPFDAT &= ~(1 << 6);
	}
	return 0;
}
