#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/struct_utility.h>
#include <sys/global_funcs.h>
#include <sys/global_vars.h>

// extern struct task_struct* current_user_pcb;
extern int current_pcb_pointer;
extern struct task_struct pcb_list[PCB_LIST_LENGTH];

uint64_t returnCR2Value(){
        uint64_t cr2Value;
        __asm__ __volatile__ ("movq %%cr2, %0"
                                : "=r" (cr2Value)
                                :
                                :
        );

        return cr2Value;
}

//TODO have to change vma_list array to linked list
uint64_t returnVMAForFaultingAddress(uint64_t faultingAddress){
	struct task_struct* current_user_pcb = &pcb_list[current_pcb_pointer];
	//kprintf("Fault Hanlder: %p\n", faultingAddress);
	for(int i = 0; i < 8; i++){
		if(faultingAddress >= current_user_pcb->vma_list[i].start_point && faultingAddress < current_user_pcb->vma_list[i].end_point){
			return i;
		}
	}
	return -1;
}



void fault_handler(){
    	kprintf("Fault Hanlder: %p\n", returnCR2Value());
    	//kprintf("pcb %p\n", current_user_pcb->vma_list[0].start_point);

//    	struct vm_struct *vmaToMap;
//	vmaToMap = &pcb4.vma_list[returnVMAForFaultingAddress()];
	struct task_struct* current_user_pcb = &pcb_list[current_pcb_pointer];
    	uint64_t faultingAddress = returnCR2Value();
	uint64_t currentVMA = returnVMAForFaultingAddress(faultingAddress);
	if(currentVMA == -1){
		kprintf("Trying to access unavailable memory %p \n",faultingAddress);
		while(1); //TODO Have to send error at this point of time
	}
	//kprintf("currentVMA index %p\n", currentVMA);
//	mapBinaryToPhysicalMemory((uint64_t) hdr, section->p_offset, section->p_vaddr, section->p_memsz);
	mapBinaryToPhysicalMemory(current_user_pcb->vma_list[currentVMA].file_start, current_user_pcb->vma_list[currentVMA].file_offset, current_user_pcb->vma_list[currentVMA].start_point, current_user_pcb->vma_list[currentVMA].size, faultingAddress);
//    	while(1);
}

