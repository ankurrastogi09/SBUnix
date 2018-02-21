/**
	Page Utility File
	This file will contain page related methods like _get_free_page(), _get_free_pages(),
	alloc_pages(), alloc_pages_node()
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/kprintf.h>
#include <sys/struct_utility.h>

extern void* free_page_pointer;

void* get_free_pages(uint64_t count);
int enumerate_free_page_list(struct node* curr_node, int count);
void mark_pages_as_used(struct node* curr_node, int count);


/**
	Return address of single free page
*/
void* get_free_page(){
	struct node* free_node = free_page_pointer;
//	kprintf("start_point %p %d %p %p %d %d\n", free_node->address, free_node->used, free_node->next->address, free_node->previous->address, free_node->next->used, free_node->previous->used);
	free_node->used = 1;
//	kprintf("start_point %p %d %p %p %d %d\n", free_node->address, free_node->used, free_node->next->address, free_node->previous->address, free_node->next->used, free_node->previous->used);
	free_page_pointer = free_node->next;
	return (void*) free_node->address;
}

/**
	Return Contigous free pages
	****** there is one thing left in this that when we allocate contigous pages than at that time
	we need to set the preivious node next to next page after the allocated page
*/
void* get_free_pages(uint64_t count){
	void* start_address = free_page_pointer;
	struct node* curr_node = start_address;
	int set_free_page_pointer_flag = 1;
	struct node* prev_curr_node;

	while(1){
		int e_count = enumerate_free_page_list(curr_node, count);
		if(e_count == 0){
			mark_pages_as_used(curr_node, count);
			//if set_free_page_pointer_flag is 1 then setting free_page_pointer to curr_node->next as new free_page_pointer
			if(set_free_page_pointer_flag){
				//free_page_pointer = (curr_node + count - 1)->next;
				free_page_pointer = (curr_node + count - 1)->next;
			} else {
				prev_curr_node->next = (curr_node + count - 1)->next;
			}
			return (void*) curr_node->address;
		} else {
			curr_node = curr_node + e_count - 1;
			prev_curr_node = curr_node;
			curr_node = curr_node->next;
			set_free_page_pointer_flag = 0;
		}
	}
	return NULL;
}

void mark_pages_as_used(struct node* curr_node, int count){
	int temp_counter = 0;
	while(temp_counter < count){
		curr_node->used = 1;
		curr_node = curr_node + 1;
		temp_counter++;
	}	
}

//contigouse pages are available if it returns 0 other wise not
int enumerate_free_page_list(struct node* curr_node, int count){
	//struct node* curr_node = start_address;
	int temp_counter = 0;
	while(temp_counter < count){
		if(curr_node->used == 0){
			curr_node = curr_node + 1;
		} else {
			return temp_counter;
		}
		temp_counter++;
	}
	return 0;
}

void alloc_pages(){}
