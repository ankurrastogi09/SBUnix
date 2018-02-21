#include <stdio.h>

void traverseTarfs();
uint64_t* kmalloc(size_t size);
void idt_install();
uint64_t f1(uint64_t memory);
uint64_t f2(uint64_t memory);
uint64_t f3(uint64_t memory);
uint64_t f4(uint64_t memory);
uint64_t getCR3PhyAddress();
uint64_t getVirtualAddressFromPhysicalAddress(uint64_t physical_address);
void* get_free_page();
void set_tss_rsp(void *rsp);
void mapBinaryToPhysicalMemory(uint64_t fileStart, uint64_t sectionStartOffset, uint64_t sectionVirtualAddress, uint64_t sectionMemorySize, uint64_t faultingAddress);
void checkAndMapVirtualAddressToPageTable(uint64_t sectionVirtualAddress);
void schedulerInit();
void resetAllOffsets(uint64_t* table_base_address);
void yield();
void tlb_flush();