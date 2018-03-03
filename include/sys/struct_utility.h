truct vm_structifndef _STRUCT_UTILITY_H
#define _ELF64_H

#define INITIAL_STACK_SIZE 512

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

struct task_struct {
	uint64_t kstack[INITIAL_STACK_SIZE];
	uint64_t pid;
	uint64_t rsp;
	uint64_t rip;
	uint64_t temp_user_rsp;
	struct vm_struct vma_list[32];
	enum {RUNNING, SLEEPING, ZOMBIE} state;
	int exit_status;
};

#endif
