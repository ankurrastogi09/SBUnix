#include <stdio.h>
#include <unistd.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/struct_utility.h>
#include <sys/syscall.h>
#include <sys/tarfs.h>
#include <sys/global_vars.h>

struct task_struct pcb_list[PCB_LIST_LENGTH];
int pcb_pointer = -1;
int current_pcb_pointer = -1;

int free_pcb_index = 0;

// int isPcbListEmpty(){
// 	if(current_pcb_pointer == pcb_pointer){
// 		return 1;
// 	}
// 	return 0;
// }

// void addPcb(struct task_struct pcb){
// 	struct task_struct *p = &pcb_list[0];			// kernel 0 thread pcb
// 	struct task_struct *p1 = &pcb_list[1];			//kernel 1 thread pcb - user also

// 	// free_pcb_index = 2;
// 	struct task_struct *new_pcb = &pcb_list[free_pcb_index];
// 	free_pcb_index ++;

// 	if(pcb_pointer >= 1024){
// 		kprintf("Cannot add more %d \n",1);
// 	}
// 	pcb_pointer = pcb_pointer + 1;
// 	pcb_list[pcb_pointer] = pcb;
// }

void nextPcbToRun(){
	int gotPointerFlag = 0;
	for(int i = (current_pcb_pointer + 1); i < free_pcb_index; i++){
		if(pcb_list[i].state == 1){
			current_pcb_pointer = i;
			gotPointerFlag = 1;
			break;
		}
	}
	if(gotPointerFlag == 0){
		for(int i = 0; i < (current_pcb_pointer + 1); i++){
			if(pcb_list[i].state == 1){
				current_pcb_pointer = i;
				break;
			}
		}
	}
}
