#include <sys/defs.h>
#include <sys/gdt.h>

struct idt_entry {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
   uint8_t type_attr; // type and attributes
   uint16_t offset_2; // offset bits 16..31
   uint32_t offset_3; // offset bits 32..63
   uint32_t zero;     // reserved
};

struct idt_ptr {
  uint16_t size;
  uint64_t addr;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void isr_timer();
extern void isr_default();
extern void isr_fault();
extern void isr_keyboard(int keyno);

void idt_set_intr_value(uint64_t num, uint64_t base, uint64_t selector, uint64_t flags){
   idt[num].offset_1 = base & 0xFFFF;
   idt[num].offset_2 = (base >> 16) & 0xFFFF;
   idt[num].offset_3 = (base >> 32) & 0xFFFFFFFF;
   idt[num].selector     = selector;
   idt[num].type_attr   = flags;
   idt[num].ist = 0;
   idt[num].zero = 0;
}

void idt_install(){
  idtp.size = (sizeof (struct idt_entry) * 256) - 1;
  idtp.addr = (uint64_t) &idt;

  for(int i = 0; i< 14; i++){
    idt_set_intr_value(i, (uint64_t) isr_default, 0x08, 0x8E);
  }

  idt_set_intr_value( 14, (uint64_t) isr_fault, 0x08, 0x8E);

  for(int i = 15; i< 32; i++){
    idt_set_intr_value(i, (uint64_t) isr_default, 0x08, 0x8E);
  }
  

  idt_set_intr_value( 32, (uint64_t) isr_timer, 0x08, 0x8E);
  idt_set_intr_value( 33, (uint64_t) isr_keyboard, 0x08, 0x8E);  

  for(int i = 34; i< 256; i++){
    idt_set_intr_value(i, (uint64_t) isr_default, 0x08, 0x8E);
  }

  __asm__ ( "lidt (%0)" : : "p"(&idtp) );

}
