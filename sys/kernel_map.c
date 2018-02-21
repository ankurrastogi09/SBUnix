#include <stdio.h>
#include <stdlib.h>
#include <sys/kprintf.h>
#include <sys/defs.h>

void mapContigousMemory(uint64_t startVirAddress, uint64_t  startPhysAddress, uint64_t endPhysAddress);
void enable_paging(uint64_t kernmem);
void enable_paging1(uint64_t kernmem);
extern uint64_t start_page_table;
uint64_t getPhysicalAddress(uint64_t virtualAddress);
void changePDTMappings();
extern uint64_t vpblock_map1[20];
uint64_t* kmalloc(size_t size);

void init_kernel_virtual_phys_mapping(uint64_t physbase, uint64_t physfree, uint64_t actual_physfree, uint64_t kernmem){
	mapContigousMemory(kernmem, physbase, actual_physfree);
	enable_paging(start_page_table);
	changePDTMappings();

	uint64_t *hello;
	hello = kmalloc(4);
	*hello = 1234;
//	thread1();
//	getPhysicalAddress((uint64_t) 0xFFFFFFFF802DA000);
}

void enable_paging(uint64_t pml4_address){
	__asm__ __volatile__ (
		"movq %0, %%cr3;"
		: 
		: "r" (pml4_address) 
		: );
}

void tlb_flush(){
	__asm__ __volatile__ (
		"movq %%cr3, %%rax;"
		"movq %%rax, %%cr3;"
		:::
		);
}

void enable_paging1(uint64_t pml4_address){
        __asm__ __volatile__ (
                "movq %0, %%rax;"
                "movq %%rax, %%cr3;"
                "movq %%cr0, %%rax;"
                "or $0x80000001, %%eax;"
                "movq %%rax, %%cr0;"
                :
                : "r" (pml4_address)
                : "rax");
}
