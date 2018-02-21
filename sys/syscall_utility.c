#include <stdio.h>
#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/elf64.h>
#include <sys/kprintf.h>
// #include <sys/string.h>
#include <sys/global_funcs.h>
#include <sys/global_vars.h>
#include <sys/struct_utility.h>

Elf64_Phdr* getProgramHeader(Elf64_Ehdr *hdr);
Elf64_Ehdr* returnElfHdr(uint64_t startAddress, char* fileSize, char* typeFlag);
uint64_t calNextFileAddress(unsigned char* size);
void addNodeToFileStr(struct tarfs_node* node, char* path, int type, int size);
void checkForAdditionOfNode(char* path, int type, int size);

extern struct tarfs_node* root;

size_t strlen_su(const char *str){
        const char *cstr;
        for(cstr = str; *cstr; ++cstr);
        return (cstr-str);
}

int memcmp_su(const void* str1, const void* str2, size_t size){
        const unsigned char *lstr1 = str1, *lstr2 = str2;

        while(size--){
          if(*lstr1 != *lstr2){
            return *lstr1 - *lstr2;
          } else{
            lstr1++, lstr2++;
          }
        }

        return 0;
}

int atoi_su(char* strForm){
	int numberForm = 0;

	for(int i = 0; strForm[i] != '\0'; i++){
		numberForm = numberForm * 10 + strForm[i] - '0';
	}

	return numberForm;
}

int returnFreeFd(struct task_struct* pcb){
	for(int i = 3; i < PCB_FD_LIST_LENGTH; i++){
		if(pcb->fd_list[i].free != PCB_FD_NON_FREE_POINTER){
			return i;
		}
	}
	return 0;
}

int populateFDs1(struct task_struct* pcb, Elf64_Ehdr* hdr, Elf64_Phdr* phdr){
	int free_fd_pointer = returnFreeFd(pcb);
        //kprintf("elf entry %p \n", hdr->e_entry);
        pcb->rip = (uint64_t) hdr->e_entry;
	for(uint64_t i = 0; i < hdr->e_phnum; i++){
                Elf64_Phdr *section = &phdr[i];
		if(section->p_type == 1){
			pcb->fd_list[free_fd_pointer].free = PCB_FD_NON_FREE_POINTER;
			pcb->fd_list[free_fd_pointer].type = PCB_FD_TYPE_FILE_BACKED;
			pcb->fd_list[free_fd_pointer].start_va = (uint64_t) section->p_vaddr;
			pcb->fd_list[free_fd_pointer].fs_offset = (uint64_t) section->p_offset;
			pcb->fd_list[free_fd_pointer].fd_offset = 0;
                	return free_fd_pointer;
		}
        }
        return 0;
}

int populateFDs(uint64_t readableFileStart, struct task_struct* pcb){
	int free_fd_pointer = returnFreeFd(pcb);
	pcb->fd_list[free_fd_pointer].free = PCB_FD_NON_FREE_POINTER;
	pcb->fd_list[free_fd_pointer].type = PCB_FD_TYPE_FILE_BACKED;
	pcb->fd_list[free_fd_pointer].start_va = readableFileStart;
	pcb->fd_list[free_fd_pointer].fs_offset = 0;
	pcb->fd_list[free_fd_pointer].fd_offset = 0;
	return free_fd_pointer;
}

int findFileInTarfs(char* pathname, struct task_struct* pcb){
	uint64_t startAddress = (uint64_t) &_binary_tarfs_start;
        struct posix_header_ustar* phu;
	
	//Traversing over all the files in tarfs
        while(startAddress < (uint64_t) &_binary_tarfs_end){
                phu = (struct posix_header_ustar*) startAddress;
                //kprintf("Names are %s %s \n", phu->name, pathname);
                if(memcmp_su(phu->name, pathname, strlen_su(pathname)) == 0){
                        //kprintf1("file name is %s\n", phu->name);
                        uint64_t readableFileStart = startAddress + sizeof(struct posix_header_ustar);
                        //char* readableFilePointer = (char*) readableFileStart;
                        // int i = 0;
                        //kprintf1("%s \n", readableFilePointer);
                        return populateFDs(readableFileStart, pcb);
                }

		startAddress += calNextFileAddress((unsigned char*)phu->size);
        }
        return 0;
}

char* tarfsFileRead(int fd, struct task_struct* pcb){
	return (char*) pcb->fd_list[fd].start_va + pcb->fd_list[fd].fd_offset;
}

void closeFileDescriptor(int fd, struct task_struct* pcb){
	if(fd < 3) {
		fd = 3;
	}
	pcb->fd_list[fd].free = PCB_FD_FREE_POINTER;
	pcb->fd_list[fd].type = 0;
	pcb->fd_list[fd].start_va = 0;
	pcb->fd_list[fd].fs_offset = 0;
	pcb->fd_list[fd].fd_offset = 0;
}

void traverseAndBuildTarfsDirStructure(){
	uint64_t startAddress = (uint64_t) &_binary_tarfs_start;
        struct posix_header_ustar* phu;
	
	//Traversing over all the files in tarfs
        while(startAddress < (uint64_t) &_binary_tarfs_end){
                phu = (struct posix_header_ustar*) startAddress;
                //kprintf("abcd name %s size %s type %s linkname %s magic %s \n", phu->name, phu->size, phu->typeflag, phu->linkname, phu->magic);
                //addNodeToFileStr();
                if(atoi_su(phu->typeflag)>=0 && strlen_su(phu->name) > 0){
                	checkForAdditionOfNode((char*) phu->name, atoi_su(phu->typeflag), atoi_su(phu->size));
                }
                //kprintf1("Names are %s \n", phu->name);
                // if(memcmp_su(phu->name, pathname, strlen_su(pathname)) == 0){
                //         //kprintf1("file name is %s\n", phu->name);
                //         uint64_t readableFileStart = startAddress + sizeof(struct posix_header_ustar);
                //         //char* readableFilePointer = (char*) readableFileStart;
                //         // int i = 0;
                //         //kprintf1("%s \n", readableFilePointer);
                //         //return populateFDs(readableFileStart, pcb);
                // }
		startAddress += calNextFileAddress((unsigned char*)phu->size);
        }
}

void traverseTarfsAllFiles(){
	uint64_t startAddress = (uint64_t) &_binary_tarfs_start;
        struct posix_header_ustar* phu;
	
	//Traversing over all the files in tarfs
        while(startAddress < (uint64_t) &_binary_tarfs_end){
                phu = (struct posix_header_ustar*) startAddress;
                kprintf("abcd name %s size %s type %s linkname %s magic %s \n", phu->name, phu->size, phu->typeflag, phu->linkname, phu->magic);
                
                //kprintf1("Names are %s \n", phu->name);
                // if(memcmp_su(phu->name, pathname, strlen_su(pathname)) == 0){
                //         //kprintf1("file name is %s\n", phu->name);
                //         uint64_t readableFileStart = startAddress + sizeof(struct posix_header_ustar);
                //         //char* readableFilePointer = (char*) readableFileStart;
                //         // int i = 0;
                //         //kprintf1("%s \n", readableFilePointer);
                //         //return populateFDs(readableFileStart, pcb);
                // }

		startAddress += calNextFileAddress((unsigned char*)phu->size);
        }
}