#include <stdio.h>
#include <unistd.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/struct_utility.h>
#include <sys/syscall.h>
#include <sys/tarfs.h>
#include <sys/global_vars.h>
#include <sys/global_funcs.h>

#define EFER 0xC0000080
#define STAR 0xC0000081
#define LSTAR 0xC0000082
#define CSTAR 0xC0000083
#define SFMASK 0xC0000084
#define GSBASE 0xC0000102

void contextSwitch(struct task_struct *pcb1, struct task_struct *pcb2);
void userContextSwitch(struct task_struct *pcb);
void firstContextSwitch(struct task_struct *pcb);

uint64_t* kmalloc(size_t size);
void syscallEntry();
void loadFileFromTarfs(char* fileName, struct task_struct* pcb);

void resetAllOffsets(uint64_t* table_base_address);

struct task_struct pcb1;
struct task_struct pcb2;
struct task_struct* pcb;
void kernel_thread1();
void kernel_thread2();

extern uint64_t kernel_page_tables[4];
void* get_free_page();
uint64_t getVirtualAddressFromPhysicalAddress(uint64_t physical_address);
void enable_paging(uint64_t pml_address);

struct task_struct* current_user_pcb;
void addPcb(struct task_struct* pcb);
int isPcbListEmpty();
int nextPcbToRun();
extern int current_pcb_pointer;
extern struct task_struct pcb_list[PCB_LIST_LENGTH];
extern int free_pcb_index;

ssize_t write(int fd, const void *buf, size_t count){
        uint64_t sys_call_no = 1;
        kprintf("value of buf is %p \n", &buf);
	system_generic_4(sys_call_no,(uint64_t) fd, (uint64_t) &buf, (uint64_t) count);
        return 0;
}


void rdmsr(uint32_t msr_id, uint32_t *low, uint32_t *hi){
	__asm__ __volatile__ ( "rdmsr"
				: "=a" (*low), "=d" (*hi)
				: "c"(msr_id)
				);
}

void wrmsr(uint32_t msr_id, uint32_t low, uint32_t hi){
	__asm__ __volatile__ ("wrmsr"
				: 
				: "a" (low), "b" (hi), "c"(msr_id));
}


void setEFER(){
	uint32_t a_low = 0;
	uint32_t a_high = 0;
	rdmsr(EFER, &a_low, &a_high);
	a_low = a_low | 0x1;
	wrmsr(EFER, a_low, a_high);
}

void setLSTAR(uint64_t systemHandlerFunctionPointer){
	uint32_t a_low = 0;
        uint32_t a_high = 0;
        a_low = (uint32_t) systemHandlerFunctionPointer;
	a_high = (uint32_t) (systemHandlerFunctionPointer >> 32);
        wrmsr(LSTAR, a_low, a_high);
}

void setSTAR(){
	uint32_t a_low = 0;
	uint32_t a_high = 0;
	a_high = ((0x1B << 16) | (0x8));
	wrmsr(STAR, a_low, a_high);
}

void setGSBASE(struct task_struct* pcb){
	uint32_t a_low = 0;
	uint32_t a_high = 0;
	//rdmsr(GSBASE, &a_low, &a_high);
	//kprintf("GSBASE Values a_low %p a_high %p \n", a_low, a_high);
	a_low = (uint64_t) pcb;
	a_high = (uint32_t) ((uint64_t) pcb >> 32); 
	wrmsr(GSBASE, a_low, a_high);
	//rdmsr(GSBASE, &a_low, &a_high);
	//kprintf("GSBASE Values a_low %p a_high %p \n", a_low, a_high);
}

void loadNewTables(uint64_t* pml4){
	//kprintf("pml4 %p pdp %p pd %p pt %p \n", kernel_page_tables[0], kernel_page_tables[1], kernel_page_tables[2], kernel_page_tables[3]);	
	//getting virtual address from pml4 physical address
	uint64_t* v_pml4 = (uint64_t*) getVirtualAddressFromPhysicalAddress((uint64_t) pml4);
	resetAllOffsets(v_pml4);
	*(v_pml4 + 511) = kernel_page_tables[1] | USER_ACCESSIBLE_PAGES_BIT;
}



void switch_to_ring3(struct task_struct* current, struct task_struct* next){
	kprintf1("inside switch_to_ring3 \n");
	//TODO here I will write the logic of removing the last element
	current_user_pcb = next;
	userContextSwitch(next);
}

void switch_to(struct task_struct* current, struct task_struct* next){
	// kprintf("inside switch_to %p %p \n", current->rsp, next->rsp);
	//current_user_pcb = next; //this will show current pcb running, current_pcb_pointer will show current indes in pcb list
	kprintf("Doing Context Switch \n");
	contextSwitch(current, next);
}

void yield(){
	//kprintf("I am inside yeild \n");
	struct task_struct* me = &pcb_list[current_pcb_pointer];
	nextPcbToRun();
	struct task_struct* next = &pcb_list[current_pcb_pointer];
	//set_tss_rsp(&pcb_list[current_pcb_pointer].kstack);
	if(pcb_list[current_pcb_pointer].cr3 != 0x0){
		enable_paging(pcb_list[current_pcb_pointer].cr3);
		tlb_flush();
	}
	//changing the gsbase for swapgs to new user process
	//set_tss_rsp(&pcb_list[current_pcb_pointer].kstack[50]);
	setGSBASE(next);
	
	if(pcb_list[current_pcb_pointer].child_flag == 1){
		__asm__ __volatile__ ("swapgs;");
	}

	switch_to(me, next);
}

void preparePCB2(){
	for(int i = 450; i < 465; i++){
                pcb2.kstack[i] = 0;
        }
        //pcb2.kstack[4066] = (uint64_t) &thread2;
        pcb2.kstack[465] = (uint64_t) &kernel_thread2;
        pcb2.rsp = (uint64_t) &pcb2.kstack[450];
}

void kernel_thread1(){
	//switchMainStackToPCB1Stack();
	preparePCB2();
	int i = 0;
	while(i<4){
		kprintf("I am kernel thread %d\n",1);
		i++;
		//current_user_pcb = (struct task_struct*) &pcb1;
		//enQueue(pcb2);

		//yield();
		contextSwitch(&pcb1, &pcb2);
	}
}


void kernel_thread2(){
	int j = 0;
	while(j<4){
		//kprintf("I am kernel thread %d \n", 2);
		j++;

		//current_user_pcb = (struct task_struct*) &pcb2;
		//enQueue(pcb1);

		//yield();

		//contextSwitch(&pcb2, &pcb1);

		//preparePCB3();

		//setting them for syscall
		setEFER();
		setLSTAR((uint64_t) &syscallEntry);
		setSTAR();
		//setting KERNEL_GS_BASE msr register for swapgs
		setGSBASE(&pcb2);

		//setting this for interrupt handlers, This is how they will get stack
		
		//uint64_t new_istack[UINT64_T_STACK_SIZE];
		//set_tss_rsp(&new_istack[UINT64_T_STACK_SIZE - 1]);


		//load page table for kernel process
		uint64_t* pml4 = (uint64_t*) get_free_page();
		loadNewTables(pml4);	
		enable_paging((uint64_t) pml4);
		tlb_flush();

		pcb2.cr3 = (uint64_t) pml4;

		//setting VMAs for bin hello binary
		loadFileFromTarfs("bin/hello", &pcb2);

		checkAndMapVirtualAddressToPageTable(USER_STACK_ADDRESS_START);

		pcb2.user_rsp = USER_STACK_ADDRESS_START;
		//enQueue(pcb4);

		//testScheduler();

		current_user_pcb = &pcb2;

		//yield();

		

		userContextSwitch(&pcb2);


		while(1);
	}
}

void n_kernel_thread1(){

	while(1){
		kprintf("I am coming to n_kernel_thread 1 and then will switch back to n_kernel_thread_2 \n");
		yield();
	}
}

void n_kernel_thread2(){
	
	kprintf("I am coming to n_kernel_thread 2 \n");

	struct task_struct* pcb = &pcb_list[current_pcb_pointer];

	setEFER();
	setLSTAR((uint64_t) &syscallEntry);
	setSTAR();
	//setting KERNEL_GS_BASE msr register for swapgs
	setGSBASE(pcb);

	//setting this for interrupt handlers, This is how they will get stack
	
	uint64_t new_istack[UINT64_T_STACK_SIZE];
	set_tss_rsp(&new_istack[UINT64_T_STACK_SIZE - 1]);

	//load page table for kernel process
	uint64_t* pml4 = (uint64_t*) get_free_page();
	loadNewTables(pml4);	
	enable_paging((uint64_t) pml4);
	tlb_flush();

	pcb->cr3 = (uint64_t) pml4;

	//setting VMAs for bin hello binary
	loadFileFromTarfs("bin/hello", pcb);

	checkAndMapVirtualAddressToPageTable(USER_STACK_ADDRESS_START);

	pcb->user_rsp = USER_STACK_ADDRESS_START;
	//enQueue(pcb4);

	//testScheduler();

	// current_user_pcb = pcb;

	//yield();

	

	userContextSwitch(pcb);


	while(1);
}

void createThread(void* func_pointer){
	
	for(int i = 450; i < 465; i++){
                pcb_list[free_pcb_index].kstack[i] = 0;
        }
        //pcb2.kstack[4066] = (uint64_t) &thread2;
        pcb_list[free_pcb_index].kstack[465] = (uint64_t) func_pointer;
        pcb_list[free_pcb_index].rsp = (uint64_t) &pcb_list[free_pcb_index].kstack[450];
        pcb_list[free_pcb_index].state = 1;
        pcb_list[free_pcb_index].pid = free_pcb_index;
        pcb_list[free_pcb_index].cwd[0] = '/';
        pcb_list[free_pcb_index].readdir_index = 0;
        free_pcb_index++;
}

void moveToFirstKernelthread(){
	nextPcbToRun();
	// kprintf("abcdc %d %p \n", current_pcb_pointer, pcb_list[current_pcb_pointer].rsp);
	firstContextSwitch(&pcb_list[current_pcb_pointer]);
}