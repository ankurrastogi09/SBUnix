#include <stdio.h>
#include <sys/kprintf.h>
#include <sys/defs.h>

uint64_t getCR3PhyAddress(){
	uint64_t cr3PhyAddress;
	__asm__ __volatile__ (
		"movq %%cr3, %0;"
		: "=r" (cr3PhyAddress)
		:
		:
	);

	return cr3PhyAddress;
}
