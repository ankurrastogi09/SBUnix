#include <unistd.h>
#include <sys/syscall.h>

ssize_t read(int fd, void *buf, size_t count){
	uint64_t sc = 0;
	return (ssize_t) system_generic_4(sc, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
}