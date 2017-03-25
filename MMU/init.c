/*
 *init.c:初始化，在片内运行
 *和head.S同属与第一部分程序，此时MMU未开启，使用物理地址
 */

 #define WTCON    (*(volatile unsigned long *)0x53000000)
 #define MEM_CTL_BASE    (*(volatile unsigned long *)0x48000000)

 void disable_watch_dog(void)
 {
 	WTCON = 0;
 }

 void memsetup(void)
 {
 	unsigned long const mem_cfg_val[] = {0x22011110,    //BWSCON
					     0x00000700,    //BANKCON0
					     0x00000700,    //BANKCON0
					     0x00000700,    //BANKCON0
					     0x00000700,    //BANKCON0
					     0x00000700,    //BANKCON0
					     0x00000700,    //BANKCON0
					     0x00018005,    //BANKCON6
					     0x00018005,    //BANKCON7
					     0x008c07a3,    //REFRESH
					     0x000000b1,    //BANKSIZE
					     0x00000030,    //MRSRB6
					    };
	int i = 0;
	volatile unsigned long *p = MEM_CTL_BASE;
	for(i = 0;i < 13;i++)
 		p[i] = mem_cfg_val[i];
 }

 /*
  *复制第二部分代码
  */
 void copy_2th_to_sdram(void)
 {
 	unsigned int *pdwSrc = (unsigned int *)2048;
	unsigned int *pdwDest = (unsigned int *)0x30004000;

	while(pdwSrc < (unsigned int *)4096)
	{
		*pdwDest = *pdwSrc;
		pdwSrc++;
		pdwDest++;
	}

 }

/*
 *设置页表
 */
void create_page_table(void)
{
#define MMU_FULL_ACCESS    (3 << 10) //访问权限
#define MMU_DOMAIN 	   (0 << 5)  //属于哪个域
#define MMU_SPECIAL 	   (1 << 4)  //必须是1
#define MMU_CACHEABLE 	   (1 << 3)  //cache使能
#define MMU_BUFFERABLE     (1 << 2)  //buffer使能
#define MMU_SECTION  	   (2)       //表示这是段描述符
#define MMU_SECDESC 	   (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION)
#define MMU_SECDESC_WB     (MMU_FULL_ACCESS | MMU_DOMAIN | MMU_SPECIAL | MMU_SECTION | \
			    MMU_CACHEABLE | MMU_BUFFERABLE)
#define MMU_SECTION_SIZE   0x00100000

	unsigned long v_addr,p_addr;
	unsigned long *mmu_tlb_base = (unsigned long *)0x30000000;

	v_addr = p_addr = 0;
	/*
	 *虚拟地址的[31:20]用于索引一级页表，找到它对应的描述符(v_addr >> 20).
	 *段描述符中[31:20]保存段的物理地址，对应(p_addr & 0xfff00000)
	 */
	*(mmu_tlb_base + (v_addr >> 20)) = (p_addr & 0xfff00000) | MMU_SECDESC_WB;

	v_addr = 0xa0000000;
	p_addr = 0x56000000;
	*(mmu_tlb_base + (v_addr >> 20)) = (p_addr & 0xfff00000) | MMU_SECDESC;

	v_addr = 0xb0000000;
	p_addr = 0x30000000;
	while(v_addr < 0xb4000000)
	{
		*(mmu_tlb_base + (v_addr >> 20)) = (p_addr & 0xfff00000) | MMU_SECDESC_WB;
		v_addr += 0x100000;
		p_addr += 0x100000;
	}
}

/*
 *启动MMU
 */
void mmu_init(void)
{
	unsigned long ttb = 0x30000000;
__asm__(
	"mov r0,#0\n"
	"mcr p15,0,r0,c7,c7,0\n"
	"mcr p15,0,r0,c7,c10,4\n"
	"mcr p15,0,r0,c8,c7,0\n"
	"mov r4,%0\n"
	"mcr p15,0,r4,c2,c0,0\n"
	"mvn r0.#0\n"
	"mcr p15,0,r0,c3,c0,0\n"
	"mcr p15,0,r0,c1,c0,0\n"
	"bic r0,r0,#0x3000\n"
	"bic r0,r0,#0x0300\n"
	"bic r0,r0,#0x0087\n"
	"orr r0,r0,#0x0002\n"
	"orr r0,r0,#0x0004\n"
	"orr r0,r0,#0x1000\n"
	"orr r0,r0,#0x0001\n"
	"mcr p15,0,r0,c1,c0,0\n"
	: 
	: "r" (ttb) )

}
