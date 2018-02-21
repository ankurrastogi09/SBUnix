#include <stdio.h>
#include <stdlib.h>
#include <sys/kprintf.h>
#include <sys/struct_utility.h>

extern void* pointer;
struct node* free_page_pointer;
void* actual_physfree;
struct vpblock* vpblock_map;
uint64_t vpblock_map1[20];

void init_pagedesc(uint64_t* amem, int size)
{
	uint64_t aval_mem[size]; 
	free_page_pointer = NULL;
	vpblock_map = NULL;
        struct node* start = pointer;
	for(int i = 0; i < 20; i++){
		vpblock_map1[i] = (uint64_t) 0xFFFFFFFFFFFFFFFF;
	}
	for(int i=0; i<size; i++)
	{
		aval_mem[i] = amem[i];
		vpblock_map1[i] = amem[i];
	}
	int k = 0;
	uint64_t startMemory;
	uint64_t endMemory;
	//start points to physfree
	struct node* tempstart = start;
	struct node* prev = NULL;
	while(k < size)
	{
		startMemory = aval_mem[k++];
		endMemory = aval_mem[k++];
		while(startMemory <= endMemory)
		{
			tempstart->previous = prev;
			if(prev != NULL)
			{
				prev->next = tempstart;
			}
			tempstart->address = startMemory;
//			kprintf("startMemory", startMemory);
			tempstart->used = 0;
			prev = tempstart;
			//kprintf("tempstart: %p %p\n", tempstart->address, tempstart->used);
			tempstart = tempstart + 1;
			startMemory = startMemory + 0x1000;	
		}
	}
	prev->next = NULL;
	//actual_physfree =  prev; //changed for reboot problem
	//actual_physfree = prev + 1;
	actual_physfree = tempstart;
//        kprintf("start details %p, %p, %p \n", start->address, pointer, actual_physfree);

	//Initializing each page descriptor node with used or free flag
	while(start != NULL){
		if((uint64_t) actual_physfree > start->address){
			start->used = 1;
		} else {
			start->used = 0;
			if(actual_physfree == (void*) start->address){
				//kprintf("yes we have actual physfree == start address %p", start->address);
			}
			if(free_page_pointer == NULL){
				free_page_pointer = start;
			}
		}
		start = start->next;
	}
	//kprintf("start details1 %p, %p, %p \n", start->address, pointer, actual_physfree);	
}

