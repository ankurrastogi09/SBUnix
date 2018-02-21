#include <unistd.h>
#include <sys/syscall.h>

char* ps(int pid){
	uint64_t sc = 7;
	return (char*) system_generic_2(sc, (uint64_t) pid);
}