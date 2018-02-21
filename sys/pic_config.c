#include <sys/defs.h>

#define PIC1        0x20        /* IO base address for master PIC */
#define PIC2        0xA0        /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA    (PIC1+1)
#define PIC2_COMMAND    PIC2
#define PIC2_DATA    (PIC2+1)

void setOutPort(uint16_t out_port, uint8_t out_value){
	__asm__ volatile ("outb %0, %1":: "a"(out_value), "Nd"(out_port));
}

uint8_t setInb(uint16_t input_port){
	uint8_t result;
	__asm__ volatile ( "inb %1, %0"
				: "=a"(result)
				: "Nd"(input_port) 
	);
	
	return result;
}

void setIOWaitingTime(void){
	__asm__ volatile ( "jmp 1f\n\t"
				"1: jmp 2f\n\t"
				"2:");
}

void PIC_remap(int offset1_value, int offset2_value)
{
    unsigned char def1 = setInb(PIC1_DATA);
    unsigned char def2 = setInb(PIC2_DATA);
    setOutPort(PIC1_COMMAND, 0x10 + 0x01);
    setIOWaitingTime();
    setOutPort(PIC2_COMMAND, 0x10 + 0x01);
    setIOWaitingTime();
    setOutPort(PIC1_DATA, offset1_value);
    setIOWaitingTime();
    setOutPort(PIC2_DATA, offset2_value);
    setIOWaitingTime();
    setOutPort(PIC1_DATA, 4);
    setIOWaitingTime();
    setOutPort(PIC2_DATA, 2);      
    setIOWaitingTime();
    setOutPort(PIC1_DATA, 0x01);
    setIOWaitingTime();
    setOutPort(PIC2_DATA, 0x01);
    setIOWaitingTime();
    setOutPort(PIC1_DATA, def1);
    setOutPort(PIC2_DATA, def2);
}

void PIC_sendEOI(unsigned char irq_number)
{
    if(irq_number >= 8)
        setOutPort(PIC2_COMMAND,0x20);
 
    setOutPort(PIC1_COMMAND,0x20);
}

unsigned char inportb(unsigned int input_port)
{
   unsigned char result;
   __asm__ volatile ("inb %%dx,%%al":"=a" (result):"d" (input_port));
   return result;
}
