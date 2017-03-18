/****************************************************
 *File : crt.S
 *功能 : 通过它转入C程序
 ***************************************************/


.text
.global _start
_start:
	LDR    R0,=0x56000010 		//WATCHDOG
	MOV    R1,#0x0
	STR    R1,[R0]

	LDR    SP,=1024*4 		//设置堆栈，4K可用内存
	BL     main
HALT_LOOP:
	B    HALT_LOOP
