/******************************************************************
 *File:head.S
 *功能:设置SDRAM，将第二部分代码复制到SDRAM；设置页表，启动MMU，
  	然后跳到SDRAM执行
 ****************************************************************/
.text
.global _start
_start:
	ldr sp,=4096        //设置栈指针，片内4K
	bl disable_watch_dog
	bl memsetup
	bl copy_2th_to_sdram
	bl create_page_table
	bl mmu_init
	ldr sp,=0xB4000000   //栈指针指向SDRAM顶端
	ldr pc,=0xB0004000
halt_loop:
	b halt_loop
