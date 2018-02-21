#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/global_funcs.h>
#define INITIAL_STACK_SIZE 4096
void* pointer;
void init_pagedesc(uint64_t* aval_mem, int size);
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;
extern void* actual_physfree;
void* get_free_page();
void* get_free_pages(uint64_t count);
void init_kernel_virtual_phys_mapping(uint64_t physbase, uint64_t physfree, uint64_t actual_physfree, uint64_t kernmem);
void kernel_thread1();
void thread3();
void thread5();
void traverseTarfs();

void n_kernel_thread1();
void n_kernel_thread2();
void createThread(void* func_pointer);
void moveToFirstKernelthread();
void kprintf2(const char *fmt, ...);
void teminalInit();
void traverseAndBuildTarfsDirStructure();
void tarfsDirStrInit();


void start(uint32_t *modulep, void *physbase, void *physfree)
{
	pointer = physfree;
	uint64_t* aval_mem = physfree;
	int count = 0;
	struct smap_t {
		uint64_t base, length;
		uint32_t type;
	}__attribute__((packed)) *smap;
	while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	for(smap = (struct smap_t*)(modulep+2); smap < (struct smap_t*)((char*)modulep+modulep[1]+2*4); ++smap) {
		if (smap->type == 1 /* memory */ && smap->length != 0) {
			*(aval_mem+count)  = smap->base;
			count++;
			*(aval_mem+count) = smap->base + smap->length;
			count++;
		}
	}
	//Initializing page descriptor table
	init_pagedesc(aval_mem, count);

	init_kernel_virtual_phys_mapping((uint64_t)physbase, (uint64_t) physfree, (uint64_t) actual_physfree, (uint64_t) &kernmem);

	idt_install();

	teminalInit();
	tarfsDirStrInit();

	traverseAndBuildTarfsDirStructure();
	createThread(&n_kernel_thread1);
	createThread(&n_kernel_thread2);

	moveToFirstKernelthread();


	while(1);
}

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	register char *temp1, *temp2;

	for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
	__asm__ volatile (
			"cli;"
			"movq %%rsp, %0;"
			"movq %1, %%rsp;"
			:"=g"(loader_stack)
			:"r"(&initial_stack[INITIAL_STACK_SIZE])
			);
	init_gdt();

	// kprintf("size of loader_stack %d\n", sizeof(loader_stack));

	//  for(int i = 0; i < sizeof(loader_stack); i++){
	//    kprintf("i %x %x\n", i, loader_stack[i]);
	//  }

	start(
			(uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
			(uint64_t*)&physbase,
			(uint64_t*)(uint64_t)loader_stack[4]
	     );

	for(
			temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
			*temp1;
			temp1 += 1, temp2 += 2
	   ) *temp2 = *temp1;
	while(1) __asm__ volatile ("hlt");
}
