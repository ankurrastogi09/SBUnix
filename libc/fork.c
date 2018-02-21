#include <unistd.h>
#include <sys/syscall.h>

pid_t fork(){
	uint64_t sc = 57;
	return (pid_t) system_generic_1(sc); 
}