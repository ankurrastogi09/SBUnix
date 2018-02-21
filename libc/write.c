#include <unistd.h>
#include <sys/syscall.h>

ssize_t write(int fd, const void* buf, size_t count){
	// char buffer[count];
	// int i;
	// for(i = 0; i < count; i++){
	// 	buffer[i] = *((char*)buf+i);
	// }
	// buffer[i+1] = '\n';
	//uint64_t fd1 = (uint64_t) fd;
	//uint64_t count1 = (uint64_t) count;
        uint64_t sys_call_no = 1;
	system_generic_4(sys_call_no,(uint64_t) fd, (uint64_t) buf, (uint64_t) count);
        return 0;
}
