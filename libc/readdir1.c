#include <unistd.h>
#include <sys/syscall.h>

int readdir1(){
	uint64_t sc = 8;
	return (ssize_t) system_generic_1(sc);
}