#include <unistd.h>
#include <sys/syscall.h>

int close(int fd){
	uint64_t sc = 5;
	return (ssize_t) system_generic_2(sc, (uint64_t) fd);
}