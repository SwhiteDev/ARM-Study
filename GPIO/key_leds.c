#define GPFCON (*(volatile unsigned long *) 0x56000050)
#define GPFDAT (*(volatile unsigned long *) 0x56000054)
#define GPFUP  (*(volatile unsigned long *) 0x56000058)

#define GPF4_out (1 << (4 * 2))
#define GPF5_out (1 << (5 * 2))
#define GPF6_out (1 << (6 * 2))

void Delay(volatile unsigned long dly)
{
	for(;dly > 0;dly--);
}

int main()
{
	unsigned long i = 0;
	GPFCON = GPF4_out | GPF5_out | GPF6_out;
	while(1)
	{
		Delay(30000);
		GPFDAT = (i << 4);
		if(i++ == 8)
		i = 0;
	}
	return 0;
}
