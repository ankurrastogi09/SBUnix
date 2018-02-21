#include <stdio.h>
#include <sys/tarfs.h>
#include <sys/defs.h>
#include <sys/elf64.h>
#include <sys/kprintf.h>
#include <sys/string.h>
#include <sys/global_funcs.h>
#include <sys/global_vars.h>
#include <sys/struct_utility.h>

enum Elf_Ident {
    EI_MAG0        = 0, // 0x7F
    EI_MAG1        = 1, // 'E'
    EI_MAG2        = 2, // 'L'
    EI_MAG3        = 3, // 'F'
    EI_CLASS    = 4, // Architecture (32/64)
    EI_DATA        = 5, // Byte Order
    EI_VERSION    = 6, // ELF Version
    EI_OSABI    = 7, // OS Specific
    EI_ABIVERSION    = 8, // OS Specific
    EI_PAD        = 9  // Padding
};

# define ELFMAG0    0x7F // e_ident[EI_MAG0]
# define ELFMAG1    'E'  // e_ident[EI_MAG1]
# define ELFMAG2    'L'  // e_ident[EI_MAG2]
# define ELFMAG3    'F'  // e_ident[EI_MAG3]
 
# define ELFDATA2LSB    (1)  // Little Endian
# define ELFCLASS32    (1)  // 32-bit Architecture

extern int current_pcb_pointer;
extern struct task_struct pcb_list[PCB_LIST_LENGTH];
extern int free_pcb_index;

//Reference http://wiki.osdev.org/USTAR
int oct2bin(unsigned char *str, int size)
{
    int n = 0;
    unsigned char *c = str;
    while (size-- > 0) {
        n *= 8;
        n += *c - '0';
        c++;
    }
    return n;
}

//Reference http://wiki.osdev.org/ELF_Tutorial
uint64_t elf_check_file(Elf64_Ehdr* hdr) {
    if(!hdr) return 0;
    if(hdr->e_ident[EI_MAG0] != ELFMAG0) {
        kprintf("ELF Header EI_MAG0 incorrect.\n");
        return 0;
    }
    if(hdr->e_ident[EI_MAG1] != ELFMAG1) {
        kprintf("ELF Header EI_MAG1 incorrect.\n");
        return 0;
    }
    if(hdr->e_ident[EI_MAG2] != ELFMAG2) {
        kprintf("ELF Header EI_MAG2 incorrect.\n");
        return 0;
    }
    if(hdr->e_ident[EI_MAG3] != ELFMAG3) {
        kprintf("ELF Header EI_MAG3 incorrect.\n");
        return 0;
    }
    return 1;
}

uint64_t calNextFileAddress(unsigned char* size){
	uint64_t fileSize = oct2bin(size, 11);
	fileSize = (((fileSize + 511)/512) + 1) *512;
	return fileSize;
}

Elf64_Phdr* getProgramHeader(Elf64_Ehdr *hdr){
	return (Elf64_Phdr*) ((uint64_t) hdr + hdr->e_phoff);
}

void loadProgramHeader(Elf64_Ehdr *hdr){
	Elf64_Phdr* phdr = getProgramHeader(hdr);

	for(uint64_t i = 0; i < hdr->e_phnum; i++){
		Elf64_Phdr *section = &phdr[i];
		if(section->p_type == 1){}
		//kprintf("p_type %p p_filesz %p p_memsz %p p_flags %p p_align %p \n", section->p_type, section->p_filesz, section->p_memsz, section->p_flags, section->p_align);
		//kprintf("p_offset %p p_vaddr %p p_paddr %p \n", section->p_offset, section->p_vaddr, section->p_paddr);
	}
}

void processELF(uint64_t startAddress, char* fileSize, char* typeFlag){

	uint64_t size = oct2bin((unsigned char*) fileSize, 11);
	uint64_t type = oct2bin((unsigned char*) typeFlag, 1);

	if(size > 0 && type == 0){
		Elf64_Ehdr* hdr = (Elf64_Ehdr*) (startAddress + sizeof(struct posix_header_ustar));
		if(elf_check_file(hdr) == 1){
			loadProgramHeader(hdr);	
		}
	}
}

Elf64_Ehdr* returnElfHdr(uint64_t startAddress, char* fileSize, char* typeFlag){
	uint64_t size = oct2bin((unsigned char*) fileSize, 11);
        uint64_t type = oct2bin((unsigned char*) typeFlag, 1);

        if(size > 0 && type == 0){
		Elf64_Ehdr* hdr = (Elf64_Ehdr*) (startAddress + sizeof(struct posix_header_ustar));
		if(elf_check_file(hdr) == 1){
			return hdr;
		}	
	}

	return (Elf64_Ehdr*) NULL;	
}

//Recive pageTableAddress, Offset and return virtual address of the entry at offset
uint64_t mapPageTableEntryIfNotExists(uint64_t* pageTableAddress, uint64_t offset){
	uint64_t physicalAddress = *(pageTableAddress + offset);
	uint64_t newPageTable;
	if(physicalAddress == 0){
		newPageTable = (uint64_t) get_free_page();
		resetAllOffsets((uint64_t*) getVirtualAddressFromPhysicalAddress(newPageTable));
		*(pageTableAddress + offset) = newPageTable | USER_ACCESSIBLE_PAGES_BIT;
		return getVirtualAddressFromPhysicalAddress(newPageTable);
	} else if((physicalAddress & COW_PAGES_BIT) == COW_PAGES_BIT){
		//For COW we are setting a new page table to virutal address and then copying
		//Data from old parent data to Parent/Child whoever gets page fault
		uint64_t virSourcePhysicalAddress = getVirtualAddressFromPhysicalAddress(physicalAddress);
		newPageTable = (uint64_t) get_free_page();
		resetAllOffsets((uint64_t*) getVirtualAddressFromPhysicalAddress(newPageTable));
		*(pageTableAddress + offset) = newPageTable | USER_ACCESSIBLE_PAGES_BIT;
		uint64_t virDestPhysicalAddress = getVirtualAddressFromPhysicalAddress(newPageTable);
		memcpy((void*) virDestPhysicalAddress, (void*) virSourcePhysicalAddress, 4096);
	} else {
		return getVirtualAddressFromPhysicalAddress(physicalAddress);
	}
	return 0;
}

void checkAndMapVirtualAddressToPageTable(uint64_t sectionVirtualAddress){
	uint64_t firstOffset = f1(sectionVirtualAddress);
	uint64_t secondOffset = f2(sectionVirtualAddress);
	uint64_t thirdOffset = f3(sectionVirtualAddress);
	uint64_t fourthOffset = f4(sectionVirtualAddress);
	
	uint64_t* v_pml4 = (uint64_t*) getVirtualAddressFromPhysicalAddress(getCR3PhyAddress());
	uint64_t* v_pdp = (uint64_t*) mapPageTableEntryIfNotExists(v_pml4, firstOffset);
	uint64_t* v_pd = (uint64_t*) mapPageTableEntryIfNotExists(v_pdp, secondOffset);
	uint64_t* v_pt = (uint64_t*) mapPageTableEntryIfNotExists(v_pd, thirdOffset);
	mapPageTableEntryIfNotExists(v_pt, fourthOffset);
}

//This method is same as above just we can pass pml4 address in this
uint64_t checkAndMapVirtualAddressToPageTable2(uint64_t sectionVirtualAddress, uint64_t pml4_address){
	uint64_t firstOffset = f1(sectionVirtualAddress);
	uint64_t secondOffset = f2(sectionVirtualAddress);
	uint64_t thirdOffset = f3(sectionVirtualAddress);
	uint64_t fourthOffset = f4(sectionVirtualAddress);
	
	uint64_t* v_pml4 = (uint64_t*) getVirtualAddressFromPhysicalAddress(pml4_address);
	uint64_t* v_pdp = (uint64_t*) mapPageTableEntryIfNotExists(v_pml4, firstOffset);
	uint64_t* v_pd = (uint64_t*) mapPageTableEntryIfNotExists(v_pdp, secondOffset);
	uint64_t* v_pt = (uint64_t*) mapPageTableEntryIfNotExists(v_pd, thirdOffset);
	uint64_t v_phyPage = mapPageTableEntryIfNotExists(v_pt, fourthOffset);
	return v_phyPage;
}


void mapBinaryToPhysicalMemory(uint64_t fileStart, uint64_t sectionStartOffset, uint64_t sectionVirtualAddress, uint64_t sectionMemorySize, uint64_t faultingAddress){
	//mapping faulting address in virtual address space
	checkAndMapVirtualAddressToPageTable(faultingAddress);

	sectionMemorySize = (sectionMemorySize < 0x1000) ? sectionMemorySize : 0x1000;

	uint64_t startCurrentPage = faultingAddress & 0xFFFFFFFFFFFFF000; //copy destination
	//how far is current page from start of current section
	uint64_t distFromStartOfSectionStart = sectionVirtualAddress - startCurrentPage;
	uint64_t copySrc = fileStart + sectionStartOffset + distFromStartOfSectionStart;

	if(sectionMemorySize >0){
		memcpy((void*) startCurrentPage, (void*) copySrc, sectionMemorySize);
	}
}

void mapCOWToPhysicalMemory(uint64_t fileStart, uint64_t sectionStartOffset, uint64_t sectionVirtualAddress, uint64_t sectionMemorySize, uint64_t faultingAddress){
	//mapping faulting address in virtual address space
	checkAndMapVirtualAddressToPageTable(faultingAddress);

	sectionMemorySize = (sectionMemorySize < 0x1000) ? sectionMemorySize : 0x1000;

	uint64_t startCurrentPage = faultingAddress & 0xFFFFFFFFFFFFF000; //copy destination
	//how far is current page from start of current section
	uint64_t distFromStartOfSectionStart = sectionVirtualAddress - startCurrentPage;
	uint64_t copySrc = fileStart + sectionStartOffset + distFromStartOfSectionStart;

	if(sectionMemorySize >0){
		memcpy((void*) startCurrentPage, (void*) copySrc, sectionMemorySize);
	}
}

void setPcbVmaListFields(struct task_struct* pcb, Elf64_Ehdr *hdr, Elf64_Phdr *section, int count){
	pcb->vma_list[count].start_point = (uint64_t) section->p_vaddr;
        pcb->vma_list[count].end_point = (uint64_t) (section->p_vaddr + section->p_memsz);
	pcb->vma_list[count].size = (uint64_t) section->p_memsz;
	pcb->vma_list[count].file_offset = (uint64_t) section->p_offset;
	pcb->vma_list[count].file_start = (uint64_t) hdr;
//        kprintf("p_type %p p_filesz %p p_memsz %p p_flags %p p_align %p \n", section->p_type, section->p_filesz, section->p_memsz, section->p_flags, section->p_align);
//        kprintf("p_offset %p p_vaddr %p p_paddr %p \n", section->p_offset, section->p_vaddr, section->p_paddr);
}

void setStackInPcbVmaList(struct task_struct* pcb, int count){
	pcb->vma_list[count].start_point = (uint64_t) USER_STACK_ADDRESS_END_LIMIT;
        pcb->vma_list[count].end_point = (uint64_t) USER_STACK_ADDRESS_START;
        pcb->vma_list[count].size = 0;
        pcb->vma_list[count].file_offset = 0;
        pcb->vma_list[count].file_start = 0;
}

void populateVMAs(struct task_struct* pcb, Elf64_Ehdr* hdr, Elf64_Phdr* phdr){
	int count = 0;
        //kprintf("elf entry %p \n", hdr->e_entry);
        pcb->rip = (uint64_t) hdr->e_entry;
	for(uint64_t i = 0; i < hdr->e_phnum; i++){
                Elf64_Phdr *section = &phdr[i];
		if(section->p_type == 1){
			setPcbVmaListFields(pcb, hdr, section, count);
                	count++;
		}
        }
	setStackInPcbVmaList(pcb, count);
}

// Load a file from tarfs by file name for ex load sbush */
void loadFileFromTarfs(char* fileName, struct task_struct* pcb){
        uint64_t startAddress = (uint64_t) &_binary_tarfs_start;
        struct posix_header_ustar* phu;
	
	//Traversing over all the files in tarfs
        while(startAddress < (uint64_t) &_binary_tarfs_end){
                phu = (struct posix_header_ustar*) startAddress;

                if(memcmp(phu->name, fileName, strlen(fileName)) == 0){
                        //kprintf("file name is %s\n", phu->name);
			Elf64_Ehdr* hdr = returnElfHdr(startAddress, phu->size, phu->typeflag);
			Elf64_Phdr* phdr = getProgramHeader(hdr);
			//populating VMAs for each program header
			populateVMAs(pcb, hdr, phdr);
			break;
                }

		startAddress += calNextFileAddress((unsigned char*)phu->size);
        }
}

void traverseTarfs(){
//	kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

	uint64_t startAddress = (uint64_t) &_binary_tarfs_start;

	struct posix_header_ustar* phu;

	phu = (struct posix_header_ustar*) startAddress;

	//kprintf("name %s size %s type %s \n", phu->name, phu->size, phu->typeflag);
	kprintf("name %s \n", phu->name);

	processELF(startAddress, phu->size, phu->typeflag);

	int count = 0;
	
	while(startAddress < (uint64_t) &_binary_tarfs_end){
		startAddress += calNextFileAddress((unsigned char*)phu->size);

	        phu = (struct posix_header_ustar*) (startAddress);

		//kprintf("abcd name %s size %s type %s linkname %s magic %s \n", phu->name, phu->size, phu->typeflag, phu->linkname, phu->magic);
		kprintf("name %s \n", phu->name);

		processELF(startAddress, phu->size, phu->typeflag);

		count++;

		if(count > 5) break;

		if(memcmp(phu->magic, (char*) "ustar", strlen("ustar")) == 0){
			//kprintf("Yes they are matching %d \n",1);
		}

/*		if(phu->magic[0] != (char) "u"){
			kprintf("fdhfjkd fdj fjdl\n");
		}*/
	}

}

void mapVirtualAddressToPageTableOnTheBasisOfOffsets(uint64_t child_pml4, uint64_t firstOffset, uint64_t secondOffset, uint64_t thirdOffset, uint64_t fourthOffset, uint64_t parentPhysicalTableAddress){
	uint64_t* v_pml4 = (uint64_t*) getVirtualAddressFromPhysicalAddress(child_pml4);
	uint64_t* v_pdp = (uint64_t*) mapPageTableEntryIfNotExists(v_pml4, firstOffset);
	uint64_t* v_pd = (uint64_t*) mapPageTableEntryIfNotExists(v_pdp, secondOffset);
	uint64_t* v_pt = (uint64_t*) mapPageTableEntryIfNotExists(v_pd, thirdOffset);
	*(v_pt + fourthOffset) = (parentPhysicalTableAddress & 0xFFFFFFFFFFFFF000) | COW_PAGES_BIT;
	//parentPhysicalTableAddress is the final physcial address on the physical page table
}

void traverseTree(uint64_t* start_address, uint64_t offsets[], int level, uint64_t child_pml4){
	
	if(level == 4){
		mapVirtualAddressToPageTableOnTheBasisOfOffsets(child_pml4, offsets[0], offsets[1], offsets[2], offsets[3], offsets[4]);
		return;
	}

	for(int i = 0; i < 512; i++){
		if(*(start_address + i) != 0){
			offsets[level] = i;
			//For last offset I am sending the final physical address as well to mapVirtualAddressToPageTableOnTheBasisOfOffsets method
			if(level == 3){
				offsets[level + 1] = *(start_address + i);
				uint64_t parentPhyAddress = *(start_address + i);//Makind Parent Physical address as COW as well
				parentPhyAddress = (parentPhyAddress & 0xFFFFFFFFFFFFF000) | COW_PAGES_BIT;
				*(start_address + i) = parentPhyAddress;
			}
			traverseTree((uint64_t*) getVirtualAddressFromPhysicalAddress(*(start_address + i)), offsets, level + 1, child_pml4);
		}
	}

}

uint64_t copyPageTables(uint64_t parent_cr3){
	uint64_t* parent_pml4_v_pointer 	= (uint64_t*) getVirtualAddressFromPhysicalAddress(parent_cr3);
	uint64_t child_pml4 				= (uint64_t) get_free_page();
	resetAllOffsets((uint64_t*) getVirtualAddressFromPhysicalAddress(child_pml4));
	uint64_t* child_pml4_v_pointer 		= (uint64_t*) getVirtualAddressFromPhysicalAddress(child_pml4);
	
	int level = 0;
	uint64_t offsets[5];
	//Mapping kernel address space into user address space
	*(child_pml4_v_pointer + 511) = *(parent_pml4_v_pointer + 511);
	//Now mapping the rest of the user address space
	for(int i = 0; i < 511; i++){
		if(*(parent_pml4_v_pointer + i) != 0){
			offsets[level] = i;
			traverseTree((uint64_t*) getVirtualAddressFromPhysicalAddress(*(parent_pml4_v_pointer + i)), offsets, level + 1, child_pml4);
		}
	}

	return child_pml4;
}


int copyParentPCBOnFork(struct task_struct* parent_pcb){
	kprintf("inside copyParentPCBOnFork cp %d\n",2);
	uint64_t child_pml4 = copyPageTables(parent_pcb->cr3);
	uint64_t child_v_pml4 = getVirtualAddressFromPhysicalAddress(child_pml4);
	if(child_v_pml4 == 0){}

	int c_index = free_pcb_index;
	
	pcb_list[free_pcb_index].cr3 = child_pml4;
	pcb_list[free_pcb_index].rip = parent_pcb->rip; //used for setting child rip i.e. e_entry
	pcb_list[free_pcb_index].rsp = parent_pcb->rsp; //copying k stack rsp

	//Copying VMA Lists
	for(int i = 0; i < 32; i++){
		pcb_list[free_pcb_index].vma_list[i].start_point = parent_pcb->vma_list[i].start_point;
		pcb_list[free_pcb_index].vma_list[i].end_point = parent_pcb->vma_list[i].end_point;
		pcb_list[free_pcb_index].vma_list[i].next = parent_pcb->vma_list[i].next;
		pcb_list[free_pcb_index].vma_list[i].size = parent_pcb->vma_list[i].size;
		pcb_list[free_pcb_index].vma_list[i].file_offset = parent_pcb->vma_list[i].file_offset;
		pcb_list[free_pcb_index].vma_list[i].file_start = parent_pcb->vma_list[i].file_start;
	}

	//Copying kStack
	for(int i = 0; i < UINT64_T_STACK_SIZE; i++){
		pcb_list[free_pcb_index].kstack[i] = 0;
		pcb_list[free_pcb_index].kstack[i] = parent_pcb->kstack[i];
	}

	//Copying Ustack
	pcb_list[free_pcb_index].user_rsp = parent_pcb->user_rsp;
	//uint64_t v_user_rsp = checkAndMapVirtualAddressToPageTable2(pcb_list[free_pcb_index].user_rsp, child_pml4);
	//memcpy((void*) (v_user_rsp & 0xFFFFFFFFFFFFF000), (void*) (parent_pcb->user_rsp & 0xFFFFFFFFFFFFF000), 4096);

	pcb_list[free_pcb_index].state = 1;
	pcb_list[free_pcb_index].pid = free_pcb_index;

	free_pcb_index++;

	return c_index;
}

