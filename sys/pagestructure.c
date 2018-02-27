#include <stdio.h>
#include <stdlib.h>
#include <sys/kprintf.h>
#include <sys/struct_utility.h>
#include <sys/global_vars.h>
#include <sys/global_funcs.h>
//#include <math.h>

void* get_free_page();
void* get_free_pages(size_t size);
uint64_t start_page_table;
void resetAllOffsets(uint64_t* table_base_address);
extern char physbase, kernmem;
extern void* pointer;
extern void* actual_physfree;
extern struct node* free_page_pointer;
uint64_t physfreeVirtualAddress, actualPhysfreeVirtualAddress;
extern uint64_t vpblock_map1[20];
uint64_t video_card_pointer;
char* point;
char* startPoint;
char* startEndLine;
char* temp;

uint64_t kernel_page_tables[4];

// Virtual Address Page Table First Offset
uint64_t f1(uint64_t memory)
{
        memory = memory << 16;
        memory = memory >> 55;
        return memory;
}

// Virtual Address Page Table Second Offset
uint64_t f2(uint64_t memory)
{
        memory = memory << 25;
        memory = memory >> 55;
        return memory;
}

// Virtual Address Page Table third Offset
uint64_t f3(uint64_t memory)
{
        memory = memory << 34;
        memory = memory >> 55;
        return memory;
}

// Virtual Address Page Table Fourth Offset
uint64_t f4(uint64_t memory)
{
        memory = memory << 43;
        memory = memory >> 55;
        return memory;
}

// Virtual Address Page Table Fifth Offset i.e. acutal offset inside physical page table
uint64_t f5(uint64_t memory){
	memory = memory << 52;
	memory = memory >> 52;
	return memory;
}

uint64_t getPhysicalAddress(uint64_t virtualAddress){
	/*uint64_t firstOffset = f1(virtualAddress);
        uint64_t secondOffset = f2(virtualAddress);
        uint64_t thirdOffset = f3(virtualAddress);
        uint64_t fourthOffset = f4(virtualAddress);
	uint64_t* plm = (uint64_t*) start_page_table;
	uint64_t* pdp = (uint64_t*) *(plm + firstOffset);
	//kprintf("plm: %p pdp: %p", (uint64_t) plm, (uint64_t) pdp);
	return 0;
	uint64_t* pd = (uint64_t*) *(pdp + secondOffset);
	uint64_t* pt = (uint64_t*) *(pd + thirdOffset);
	return (uint64_t) pt;
	return 0;
}

// This method will map one virtual address to one pysical address
void mapMemory(uint64_t  virtualAddress, uint64_t physAddress)
{
	uint64_t firstOffset = f1(virtualAddress);
	uint64_t secondOffset = f2(virtualAddress);
	uint64_t thirdOffset = f3(virtualAddress);
	uint64_t fourthOffset = f4(virtualAddress);
	uint64_t* plm = (uint64_t*) get_free_page();
	uint64_t* pdp = (uint64_t*) get_free_page();
	uint64_t* pd = (uint64_t*) get_free_page();
	uint64_t* pt = (uint64_t*) get_free_page();

	resetAllOffsets((uint64_t*) getVirtualAddressFromPhysicalAddress((uint64_t)plm));
	resetAllOffsets((uint64_t*) getVirtualAddressFromPhysicalAddress((uint64_t)pdp));
	resetAllOffsets((uint64_t*) getVirtualAddressFromPhysicalAddress((uint64_t)pd));
	resetAllOffsets((uint64_t*) getVirtualAddressFromPhysicalAddress((uint64_t)pt));

	*(plm + firstOffset) = (uint64_t)pdp;
	*(pdp + secondOffset) = (uint64_t)pd;
	*(pd + thirdOffset) = (uint64_t)pt;
	*(pt + fourthOffset) = physAddress;
}

/*
	Method to calculate virtual address from all 4 offsets
*/
uint64_t getVirtualAddressFromOffset(uint64_t firstOffset, uint64_t secondOffset, uint64_t thirdOffset, uint64_t fourthOffset){
	uint64_t virAddress = 0;
	virAddress = virAddress + 0xFFFF000000000000;
	firstOffset = firstOffset << 39;
	secondOffset = secondOffset << 30;
	thirdOffset = thirdOffset << 21;
	fourthOffset = fourthOffset << 12;
	virAddress = virAddress + firstOffset + secondOffset + thirdOffset + fourthOffset;
	return virAddress;
}

void moveVideoMemoryPointer(uint64_t* plm){
	uint64_t virVideoAddress = VIDEO_MEMORY_POINTER_ADDRESS;
	uint64_t firstOffset = f1(virVideoAddress);
        uint64_t secondOffset = f2(virVideoAddress);
        uint64_t thirdOffset = f3(virVideoAddress);
        uint64_t fourthOffset = f4(virVideoAddress);

        uint64_t* pdp = (uint64_t*) get_free_page();
        uint64_t* pd = (uint64_t*) get_free_page();
        uint64_t* pt = (uint64_t*) get_free_page();

        if(*(plm + firstOffset) == 0){
        	*(plm + firstOffset) = (uint64_t)pdp + KERNEL_ACCESSIBLE_PAGES_BIT;
        }
        if(*(pdp + secondOffset) == 0){
        	*(pdp + secondOffset) = (uint64_t)pd + KERNEL_ACCESSIBLE_PAGES_BIT;	
        }
        if(*(pd + thirdOffset) == 0){
        	*(pd + thirdOffset) = (uint64_t)pt + KERNEL_ACCESSIBLE_PAGES_BIT;	
        }
        *(pt + fourthOffset) = (uint64_t)0xB8000 + KERNEL_ACCESSIBLE_PAGES_BIT;

        video_card_pointer = getVirtualAddressFromOffset(firstOffset, secondOffset, thirdOffset, fourthOffset);
	point = (char *) video_card_pointer;
	startPoint = (char *) video_card_pointer;
	startEndLine = (char *) video_card_pointer;
	temp = (char *) video_card_pointer; 
}

// This function will map range of physical memory to virtual memory
void mapContigousMemory(uint64_t startVirAddress, uint64_t  startPhysAddress, uint64_t endPhysAddress)
{
//	kprintf("startVirAddress %p startPhysAddress %p endPhysAddress %p\n", startVirAddress, startPhysAddress, endPhysAddress);
        uint64_t firstOffset = f1(startVirAddress);
        uint64_t secondOffset = f2(startVirAddress);
        uint64_t thirdOffset = f3(startVirAddress);
        uint64_t fourthOffset = f4(startVirAddress);
//	uint64_t fifthOffset = f5(virtualAddress);
	uint64_t* plm = (uint64_t*) get_free_page();
        uint64_t* pdp = (uint64_t*) get_free_page();
        uint64_t* pd = (uint64_t*) get_free_page();
        uint64_t* pt = (uint64_t*) get_free_page();
	//Having global pointers for kernel initiall page tables i.e 511, 510, 1, 0
	kernel_page_tables[0] = (uint64_t) plm;
	kernel_page_tables[1] = (uint64_t) pdp;
	kernel_page_tables[2] = (uint64_t) pd;
	kernel_page_tables[3] = (uint64_t) pt;

	start_page_table = (uint64_t)plm;
	//kprintf("Start Page Table: %p\n", start_page_table);
	//Resetting all 512 offsets to 0
	resetAllOffsets(plm);
	resetAllOffsets(pdp);
	resetAllOffsets(pd);
	resetAllOffsets(pt);
	*(plm + firstOffset) = (uint64_t)pdp + KERNEL_ACCESSIBLE_PAGES_BIT;
        *(pdp + secondOffset) = (uint64_t)pd + KERNEL_ACCESSIBLE_PAGES_BIT;
        *(pd + thirdOffset) = (uint64_t)pt + KERNEL_ACCESSIBLE_PAGES_BIT;
        for(uint64_t i = startPhysAddress; i <= endPhysAddress; i = i + 0x1000)
        {
                *(pt + fourthOffset) = i + KERNEL_ACCESSIBLE_PAGES_BIT;
		fourthOffset++;
		if(fourthOffset > 511){
			thirdOffset++;
			if(thirdOffset > 511){
				secondOffset++;
				if(secondOffset > 511){
					firstOffset++;
					secondOffset = 0;
					pdp = (uint64_t*) get_free_page();
					*(plm + firstOffset) = (uint64_t) pdp + KERNEL_ACCESSIBLE_PAGES_BIT;
				}
				thirdOffset = 0;
				pd = (uint64_t*) get_free_page();
				*(pdp + secondOffset) = (uint64_t)pd + KERNEL_ACCESSIBLE_PAGES_BIT;
			}
			fourthOffset = 0;
                	pt = (uint64_t*) get_free_page();
                	*(pd + thirdOffset) = (uint64_t)pt + KERNEL_ACCESSIBLE_PAGES_BIT;
		}
        }
	//IN KPRINTF we have used 0xB80A0 address so mapping this
	*(pt + fourthOffset) = (uint64_t)0xB8000 + KERNEL_ACCESSIBLE_PAGES_BIT; //0xFFFFFFFF802DA000
	
	/* global variables for kprintf start here */
	video_card_pointer = getVirtualAddressFromOffset(firstOffset, secondOffset, thirdOffset, fourthOffset);
	point = (char *) video_card_pointer;
	startPoint = (char *) video_card_pointer;
	startEndLine = (char *) video_card_pointer;
	temp = (char *) video_card_pointer; 
	/* global variables for kprintf end here */
	fourthOffset++;

	if(fourthOffset > 511){
		thirdOffset++;
		if(thirdOffset > 511){
			secondOffset++;
			if(secondOffset > 511){
				firstOffset++;
				secondOffset = 0;
				pdp = (uint64_t*) get_free_page();
				*(plm + firstOffset) = (uint64_t) pdp + KERNEL_ACCESSIBLE_PAGES_BIT;
			}
			thirdOffset = 0;
			pd = (uint64_t*) get_free_page();
			*(pdp + secondOffset) = (uint64_t)pd + KERNEL_ACCESSIBLE_PAGES_BIT;
		}
		fourthOffset = 0;
        	pt = (uint64_t*) get_free_page();
        	*(pd + thirdOffset) = (uint64_t)pt + KERNEL_ACCESSIBLE_PAGES_BIT;
	}
	
	//1:1 Mapping of page tables
	//TODO Have to switch this to some new place.
	struct node* pdtFirstNode = (struct node*) pointer;
	struct node* pdtLastNode = (struct node*) actual_physfree - 1;
	uint64_t counter = 0;
	uint64_t vp_counter = 0;
	
	while(1){
		*(pt + fourthOffset) = (uint64_t) pdtFirstNode->address + KERNEL_ACCESSIBLE_PAGES_BIT;
		//keeping vpblock_map1 to calcualte virtual address of physical address in kmalloc
		if(vpblock_map1[vp_counter] == (uint64_t) pdtFirstNode->address){
			vpblock_map1[vp_counter + 1] = getVirtualAddressFromOffset(firstOffset, secondOffset, thirdOffset, fourthOffset);
			vp_counter += 2;
		}
		fourthOffset++;
		//code to increase the offset in pd Basically our pt page table got full
		//so fetching a new pt page table and mapping it to next entry in pd by incrementing thirdOffset
		if(fourthOffset > 511){
			thirdOffset++;
			if(thirdOffset > 511){
				secondOffset++;
				if(secondOffset > 511){
					firstOffset++;
					secondOffset = 0;
					pdp = (uint64_t*) get_free_page();
					*(plm + firstOffset) = (uint64_t) pdp + KERNEL_ACCESSIBLE_PAGES_BIT;
				}
				thirdOffset = 0;
				pd = (uint64_t*) get_free_page();
				*(pdp + secondOffset) = (uint64_t)pd + KERNEL_ACCESSIBLE_PAGES_BIT;
			}
			fourthOffset = 0;
                	pt = (uint64_t*) get_free_page();
                	*(pd + thirdOffset) = (uint64_t)pt + KERNEL_ACCESSIBLE_PAGES_BIT;
        	}
		if(pdtFirstNode->address == pdtLastNode->address){
			break;
		}
		if((firstOffset == 511) && (secondOffset == 511) && (thirdOffset == 511) & (fourthOffset == 511)){
			break;
		}
		pdtFirstNode = pdtFirstNode + 1;
		counter++;
	}	
	kprintf("counter is %d \n", counter);
}

//Change virtual pointer
void changePDTMappings(){
	physfreeVirtualAddress 		= (uint64_t) &kernmem + ((uint64_t) pointer - (uint64_t) &physbase);
	actualPhysfreeVirtualAddress 	= (uint64_t) &kernmem + ((uint64_t) actual_physfree - (uint64_t) &physbase);
	free_page_pointer 		= (void*) ((uint64_t) &kernmem + ((uint64_t) free_page_pointer - (uint64_t) &physbase));
	
	struct node* abc 		= (struct node*) physfreeVirtualAddress;
	struct node* xyz 		= (struct node*) actualPhysfreeVirtualAddress - 2;
	//First Node
	abc->previous 			= NULL;
	abc->next 			= abc + 1;
	abc 				= abc + 1;
	//Middle Nodes
	while(1){
		abc->previous = abc - 1;
		abc->next = abc + 1;
		if(abc->address == xyz->address){
			break;
		}
		abc = abc + 1;
	}
	//Last Node
	abc->previous = abc - 1;
	abc->next = NULL;
	
}

void setLastUsedVirtualAddress(uint64_t plm, uint64_t pdp, uint64_t pd, uint64_t pt){}

//Reset All 512 offsets to 0
void resetAllOffsets(uint64_t* table_base_address){
	int i = 0;
	while(i < 512){
		*(table_base_address + i) = 0;
		i++;
	}
}


/* Return virtual address for physical address */
uint64_t getVirtualAddressFromPhysicalAddress(uint64_t physical_address){
	int counter = 0;
        uint64_t phys_block_start = vpblock_map1[0];
        uint64_t vir_block_start = vpblock_map1[1];
        uint64_t vpblock_pages;
        uint64_t v1, v2, v3, v4, new_v3, new_v4, vir_offset;
        //kprintf("%p %p %p %p %p %p\n", vpblock_map1[0], vpblock_map1[1], vpblock_map1[2], vpblock_map1[3], vpblock_map1[4], vpblock_map1[5]);
        //if length of vpblock_map1 is 0 than this will throw error
        while(vpblock_map1[counter] < 0xFFFFFFFFFFFFFFFF){
        //      kprintf("inside kmalloc loop: %p %p \n", physicalAddress, vpblock_map1[counter]);
                if(physical_address > vpblock_map1[counter]){
                        phys_block_start = vpblock_map1[counter];
                        vir_block_start = vpblock_map1[counter + 1];
                } else {
                        break;
                }
                counter = counter + 2;
        }
        vpblock_pages = (physical_address - phys_block_start)/0x1000;
        //virtual_address = vir_block_start + (sizeof pdtFirstNode) * vpblock_pages;
        v1 = f1(vir_block_start);
        v2 = f2(vir_block_start);
        v3 = f3(vir_block_start);
        v4 = f4(vir_block_start);
        //Finding the offset to physical_address
	vir_offset = v4 + vpblock_pages;
	new_v3 = v3 + ( vir_offset / 512);
	new_v4 = vir_offset % 512;

	//kprintf("phys_block_start %p vir_block_start %p vpblock_pages %d v1 %d v2 %d v3 %d v4 %d new_v3 %d new_v4 %d \n", phys_block_start, vir_block_start, vpblock_pages, v1, v2, v3, v4, new_v3, new_v4);
	return getVirtualAddressFromOffset(v1, v2, new_v3, new_v4);        

}

uint64_t ceil(uint64_t size)
{
	int count = 0;
	int value = 0;
	while(value < size)
	{
		value = value + 4096;
		count++;
	}
	return count;
}

uint64_t* kmalloc(size_t size){
	//int no_of_pages = Math.ceil(size/4096);
	uint64_t physical_address = (uint64_t) get_free_pages(size);
	//kprintf("Physical Address %p Virtual Address %p \n", physical_address, getVirtualAddressFromPhysicalAddress(physical_address));
	return (uint64_t*) getVirtualAddressFromPhysicalAddress(physical_address);
	return 0; 
}

//void map_page(void * physaddr, void * virtualaddr, unsigned int flags){}

//void * get_physaddr(void * virtualaddr){}

