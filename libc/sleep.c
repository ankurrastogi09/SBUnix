#include <unistd.h>
#include <sys/syscall.h>

unsigned int sleep(unsigned int seconds){
	uint64_t sc = 6;
	return (ssize_t) system_generic_2(sc, (uint64_t) seconds);
}