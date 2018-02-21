#ifndef _STRUCT_UTILITY_H
#define _ELF64_H

#define INITIAL_STACK_SIZE 512

#include <sys/global_vars.h>

struct node
{
        struct node* next;
        struct node* previous;
        uint64_t address;
        uint64_t used;
};

struct vpblock {
	uint64_t p_start;
	uint64_t v_start;
};

struct vm_struct {
        uint64_t start_point; //section start point
        uint64_t end_point;
        uint64_t next;
	uint64_t size;
	uint64_t file_offset;
	uint64_t file_start; // starting point of file inside tarfs
};

struct fd_struct {
	int free;
	int type;
	uint64_t start_va;
	uint64_t fs_offset;
	uint64_t fd_offset;
};

struct task_struct {
	uint64_t kstack[INITIAL_STACK_SIZE]; 	//0
	uint64_t pid;				//4096
	uint64_t rsp;				//4104
	uint64_t rip;				//4112
	uint64_t temp_user_rsp;			//4120
	uint64_t user_rsp;			//4128
	struct vm_struct vma_list[32];
	enum {EXIT, RUNNING, READY, SLEEPING, ZOMBIE} state;
	int exit_status;
	uint64_t cr3;
	int child_flag;
	struct fd_struct fd_list[PCB_FD_LIST_LENGTH];
	char cwd[128];
	int readdir_index;
};

struct tarfs_node {
	struct tarfs_node* childrens[TARFS_CHILDREN_NODE_LIMIT];
	char name[128];
	int type;
	int size;
};

#endif
