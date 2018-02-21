#include <unistd.h>
#include <sys/syscall.h>

int open(const char *pathname, int flags){
	uint64_t sc = 2;
	return (int) system_generic_3(sc,(uint64_t) pathname,(uint64_t) flags);
}