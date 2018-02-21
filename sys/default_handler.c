#include <sys/kprintf.h>

int timer;
//void PIC_sendEOI(unsigned char irq);

void default_handler(){
	//while(1);
  timer += 1;
  if(timer < 5){
    kprintf("Timer Default: %d\n", timer);
  } else {
	while(1);	
  }
//  PIC_sendEOI(0);
}
