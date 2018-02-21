#include <stdio.h>
#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/struct_utility.h>
#include <sys/global_funcs.h>
#include <sys/global_vars.h>

extern struct task_struct* current_user_pcb;
int copyParentPCBOnFork(struct task_struct* parent_pcb);

extern int current_pcb_pointer;
extern struct task_struct pcb_list[PCB_LIST_LENGTH];
extern int free_pcb_index;

extern volatile int timerForSleep;

//char* bufferRead();
char* terminalRead();

uint64_t parent_rsp_value, rip_value, rsp_point;
int findFileInTarfs(char* pathname, struct task_struct* pcb);
void closeFileDescriptor(int fd, struct task_struct* pcb);
char* tarfsFileRead(int fd, struct task_struct* pcb);
void printCurrentDirectorChildrens();
void changeDirectory(char* path);

// void strcat_sc(char* firstStr, const char* secondStr){
// 	int i = 0; 
// 	int j = 0;
	
// 	while(*(firstStr + i) != '\0'){
// 		i++;
// 	}

// 	while(*(secondStr + j)  != '\0'){
// 		*(firstStr + i + j) = *(secondStr + j);
// 		j++;
// 	}

// 	*(firstStr + i + j) = '\0';
// }

// void reverse_sc(char* str){
// 	int len = 0;
// 	while(*(str + len) != '\0'){
// 		len++;
// 	}

// 	char temp;
// 	for(int i = 0; i < len/2; i++){
// 		temp = '\0';
// 		temp = *(str + i);
// 		*(str + i) = *(str + len -i);
// 		*(str + len -i) =  temp;
// 	}
// }

// //Refernce http://www.geeksforgeeks.org/implement-itoa/
// char* itoa_sc(int num, char* str, int base)
// {
//     int i = 0;
//     int isNegative = 0;
 
//     /* Handle 0 explicitely, otherwise empty string is printed for 0 */
//     if (num == 0)
//     {
//         str[i++] = '0';
//         str[i] = '\0';
//         return str;
//     }
 
//     // In standard itoa(), negative numbers are handled only with 
//     // base 10. Otherwise numbers are considered unsigned.
//     if (num < 0 && base == 10)
//     {
//         isNegative = 1;
//         num = -num;
//     }
 
//     // Process individual digits
//     while (num != 0)
//     {
//         int rem = num % base;
//         str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
//         num = num/base;
//     }
 
//     // If number is negative, append '-'
//     if (isNegative)
//         str[i++] = '-';
 
//     str[i] = '\0'; // Append string terminator
 
//     // Reverse the string
//     reverse_sc(str);
 
//     return str;
// }

void syscallEntry(){
	__asm__ __volatile__ (
		"swapgs;"
		"movq %%rsp, %%gs:4128;"
		"movq %%gs:4104, %%rsp;"
		//"pushq %%rax;"
		"pushq %%rbx;"
                "pushq %%rcx;"
                "pushq %%rdx;"
                "pushq %%rsi;"
                "pushq %%rdi;"
                "pushq %%rbp;"
                "pushq %%r8;"
                "pushq %%r9;"
                "pushq %%r10;"
                "pushq %%r11;"
                "pushq %%r12;"
                "pushq %%r13;"
                "pushq %%r14;"
                "pushq %%r15;"
                "movq %%rsp, %%r14;"
                "leaq (%%rip), %%r15;"
		//"call syscallHandler;"
		"call *sysCallTable(, %%rax, 8);"
                "popq %%r15;"
                "popq %%r14;"
                "popq %%r13;"
                "popq %%r12;"
                "popq %%r11;"
                "popq %%r10;"
                "popq %%r9;"
                "popq %%r8;"
                "popq %%rbp;"
                "popq %%rdi;"
                "popq %%rsi;"
                "popq %%rdx;"
                "popq %%rcx;"
                "popq %%rbx;"
                //"popq %%rax;"
		"movq %%rsp, %%gs:4104;"
		"movq %%gs:4128, %%rsp;"
		"swapgs;"
		"sysretq;"
		: : : "rax","rdi","rsi","rdx", "r8","r9","r10"
	);
}

void sysRead(){
	uint64_t fd;
	volatile char* buf;
	uint64_t size;

	volatile char* terminalOutput = NULL;

	__asm__ __volatile__ (
		"movq %%rdi, %0;"
		"movq %%rsi, %1;"
		"movq %%rdx, %2;"
		: "=r"(fd), "=r"(buf), "=r"(size)
		:
		:"rdi", "rsi", "rdx"
	);

	if(fd == 0){
		terminalOutput = terminalRead();
		int i = 0;
		while(*(terminalOutput+i) != '\n' && i < size){
			*(buf+i) = *(terminalOutput+i);
			i++;
		}
		*(buf+i) = '\0';
	} else if(fd == 1 || fd == 2){

	} else if(fd >= 3){
		terminalOutput = tarfsFileRead(fd, &pcb_list[current_pcb_pointer]);
		int i = 0;
		while(i < size){
			*(buf+i) = *(terminalOutput+i);
			i++;
		}
		*(buf+i) = '\0';
	}
	

	if(*(terminalOutput) == '\0'){
		//kprintf1("terminalOputput is coming as NULL\n");
	} else {
		//kprintf1("Terminal Output is not coming as NULL %s \n", terminalOutput);
		// int i = 0;
		// while(*(terminalOutput+i) != '\n' && i < size){
		// 	*(buf+i) = *(terminalOutput+i);
		// 	i++;
		// }
		// *(buf+i) = '\0';

		__asm__ __volatile__ (
			"movq %0, %%rax;"
			: 
			: "r" (buf)
			: "rax"
		);
	}

	yield();
}

void sysWrite(){
	uint64_t fd;
	char* buf;
	uint64_t size;
	__asm__ __volatile__ (
		"movq %%rdi, %0;"
		"movq %%rsi, %1;"
		"movq %%rdx, %2;"
		: "=r"(fd), "=r"(buf), "=m"(size)
		:
		: "rdi", "rsi", "rdx"
	);

	if(fd == 1 ||  fd == 2){
		//kprintf1("I am inside write \n");
		//kprintf1("%s \n", buf);
		int i = 0;
		// while(i < size){
		// 	kprintf1("abcde\n");
		// 	i++;	
		// }
		// i = 0;
		while(i < size && *(buf + i) != '\0'){
			char c[2];
			c[0] = *(buf + i);
			c[1] = '\0';
			if(c[0] == '\n'){
				kprintf1("\n");
			} else {
				kprintf1("%s",c);
			}
			i++;
		}
		//kprintf1("\n");

		// __asm__ __volatile__ (
		// 	"movq %0, %%rax;"
		// 	: 
		// 	: "r" (i)
		// 	: "rax"
		// );

		//kprintf1("value of fd is %d \n", fd);
		//kprintf1("value of size is %d \n", 12); //size
		//yield();
	}
}

void sysFork(){
	//kprintf("I am fork cp %d\n",1);
	uint64_t child_index = copyParentPCBOnFork(&pcb_list[current_pcb_pointer]);

	__asm__ __volatile__ (
		"movq %%r14, %0;"
		"movq %%r15, %1;"
		"movq %2, %%rax;"
		: "=r" (parent_rsp_value), "=r" (rip_value)
		: "r" (child_index)
		: "r14", "r15", "rax"
	);

	pcb_list[child_index].rsp = (uint64_t) &pcb_list[child_index].kstack[0] + ( parent_rsp_value - (uint64_t) &pcb_list[current_pcb_pointer].kstack[0]);
	pcb_list[child_index].rip = rip_value + 5;

	rsp_point = (parent_rsp_value - (uint64_t) &pcb_list[current_pcb_pointer].kstack[0])/8;
	
	//pcb_list[child_index].kstack[rsp_point--] = 0;

	rsp_point--;
	pcb_list[child_index].kstack[rsp_point--] = pcb_list[child_index].rip;
	
	for(int i = 0; i < 15; i++){
		pcb_list[child_index].kstack[rsp_point--] = 0;
	}

	pcb_list[child_index].rsp = (uint64_t) &pcb_list[child_index].kstack[rsp_point+1];

	pcb_list[child_index].child_flag = 1;

	// pcb_list[current_pcb_pointer].kstack[rsp_point+1] = 0x01;
	// pcb_list[current_pcb_pointer].kstack[rsp_point+2] = 0x11;
	// pcb_list[current_pcb_pointer].kstack[rsp_point+3] = child_index;
	// pcb_list[current_pcb_pointer].kstack[rsp_point+4] = 0x21;
	// pcb_list[current_pcb_pointer].kstack[rsp_point+5] = 0x31;
	// pcb_list[current_pcb_pointer].kstack[rsp_point+6] = 0x41;
	// pcb_list[current_pcb_pointer].kstack[rsp_point+7] = 0x51;

	//yield();

}

void sysExitMe(){
	pcb_list[current_pcb_pointer].exit_status = 1;
	pcb_list[current_pcb_pointer].state = 4;
	//yield();
}

void sysOpen(){
	char* pathname;
	uint64_t flag;
	uint64_t fd;

	__asm__ __volatile__ (
		"movq %%rdi, %0;"
		"movq %%rsi, %1;"
		: "=r"(pathname), "=r"(flag)
		:
		: "rdi", "rsi"
	);
	kprintf("value of pathname is %s \n", pathname);
	
	fd = findFileInTarfs(pathname, &pcb_list[current_pcb_pointer]);
	//kprintf("Start VA of Address is %p \n", pcb_list[current_pcb_pointer].fd_list[fd].start_va);
	
	__asm__ __volatile__ (
		"movq %0, %%rax;"
		: 
		: "r" (fd)
		: "rax"
	);

	//yield();
}
 
void sysClose(){
	uint64_t fd;

	__asm__ __volatile__ (
		"movq %%rdi, %0"
		: "=r" (fd)
		: 
		: "rdi"
		);
	
	closeFileDescriptor((int) fd, &pcb_list[current_pcb_pointer]);

}

void defaultMethod(){

}

void sysSleep(){
	uint64_t seconds = 0;
	//int local_timer = 0;
	__asm__ __volatile__ (
		"movq %%rdi, %0;"
		: "=r"(seconds)
		:
		: "rdi"
	);


	//kprintf1("I am inside sleep syscall \n");
	volatile int local_timer = timerForSleep;

	while(1){
		if(timerForSleep > (local_timer + seconds)) {
			break;
		}
	}

	//kprintf1("I am inside sleep syscall but after a long sleep \n");

	//kprintf1("value of seconds is %s \n", (char) seconds); 
	// int i = 0;
	// while(i < seconds){
	// 	kprintf1("Hello Shello\n");
	// 	i++;
	// }

	// local_timer = timerForSleep;

	// // while(1){
	// // 	//kprintf("b %d \n", local_timer);
	// // 	if(local_timer == 1){}
	// // 	kprintf("c %d \n", timerForSleep);
	// // }
	
	// while(1){
	// 	if(timerForSleep > (local_timer + seconds)){
	// 		break;
	// 	} else {
	// 		//kprintf1("Hello World \n");
	// 	}
	// }
	// int i = 0;
	// while(i < size){
	// //	kprintf1("abcde\n");
	// 	i++;	
	// }
}

void sysPs(){
	// uint64_t pid = 0;
	// char* psDetails = '\0';
	// //int local_timer = 0;
	// __asm__ __volatile__ (
	// 	"movq %%rdi, %0;"
	// 	: "=r"(pid)
	// 	:
	// 	: "rdi"
	// );

	// char* temp = '\0';
	// for(int i = 0; i < PCB_LIST_LENGTH; i++){
	// 	if(pcb_list[i].state == 0){
	// 		strcat_sc(psDetails, "PID: ");
	// 		itoa_sc(pcb_list[i].pid, temp, 10);
	// 		strcat_sc(psDetails, temp);
	// 		strcat_sc(psDetails, ", ");
	// 	}

	// 	kprintf1("%s\n",psDetails);

	// 	__asm__ __volatile__ (
	// 		"movq %0, %%rax;"
	// 		: 
	// 		: "r" (psDetails)
	// 		: "rax"
	// 	);
	// }
	

}

void sysReaddir1(){
	printCurrentDirectorChildrens();
}

void sysChdir(){
	char* buf;

	//volatile char* terminalOutput = NULL;

	__asm__ __volatile__ (
		"movq %%rdi, %0;"
		: "=r"(buf)
		:
		:"rdi"
	);

	changeDirectory(buf);
}

void* sysCallTable[100] = {
        //[0] = sysRead,
        //[1] = sysWrite
        [0] = sysRead,		//0
	[1] = sysWrite, 	//1
	[2] = sysOpen,		//2
	[3] = sysFork, 		//3
	[4] = sysExitMe, 	//4
	[5] = sysClose,		//5
	[6] = sysSleep,		//6
	[7] = sysPs, 		//7
	[8] = sysReaddir1,	//8
	[9] = sysChdir		//9
};

void (*sysCallTable1[100])(void) = { 
	sysRead,	//0
	sysWrite, 	//1
	sysOpen,	//2
	sysFork, 	//3
	sysExitMe, 	//4
	sysClose,	//5
	sysSleep	//6
};

void syscallHandler(){

	uint64_t syscall_index;

	__asm__ __volatile__(
		"movq %%rax, %0;"
		: "=r"(syscall_index)
		:
		: "rax"
	);

	(*sysCallTable1[syscall_index])();

	//yield();
}