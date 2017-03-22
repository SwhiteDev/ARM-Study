/*********************************************************
 *File : head.S
 *功能 : 设置SDRAM,将程序复制到SDRAM,然后跳到SDRAM继续执行
 ********************************************************/

.equ    MEM_CTL_BASE,    0x48000000
.equ    SDRAM_BASE,      0x30000000

.text
.global _start
_start:
 	bl disable_watch_dog
	bl memsetup
	bl copy_steppingstone_to_sdram
	ldr pc,=on_sdram
on_sdram:
	ldr sp,=0x34000000
	bl main
halt_loop:
	b halt_loop
disable_watch_dog:
	mov r1,#0x53000000
	mov r2,#0x0
	str r2,[r1]
	mov pc,lr
copy_steppingstone_to_sdram:
	mov r1,#0
	ldr r2,=SDRAM_BASE
	mov r3,#4*1024
1:
	ldr r4,[r1],#4           //从片内内存读4字节数据，源地址自加4
	str r4,[r2],#4 		 //将此4字节数据复制到SDRAM，目的自加4
	cmp r1,r3          //判断是否完成复制，源地址和片内末地址
	bne 1b
	mov pc,lr
memsetup:
	mov r1,#MEM_CTL_BASE      //存储器13个寄存器起始地址
	adrl r2,mem_cfg_val
	add r3,r1,#52
1:
	ldr r4,[r2],#4
	str r4,[r1],#4
	cmp r1,r3,
	bne 1b
	mov pc,lr

.align 4
mem_cfg_val:
	.long 0x22011110
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00000700
	.long 0x00018005
	.long 0x00018005
	.long 0x008c07a3
	.long 0x000000b1
	.long 0x00000030
	.long 0x00000030
