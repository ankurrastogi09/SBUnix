#include <unistd.h>
#include <sys/syscall.h>

int chdir(const char *path){
	uint64_t sc = 9;
	return (int) system_generic_2(sc, (uint64_t) path);
}