#include <sys/kprintf.h>

void kprintf2(const char *fmt, ...);

volatile int timerForSleep = 0;
int th_timer = 0;
int timerInSec;
void PIC_sendEOI(unsigned char irq);

void timer_handler(){
  th_timer += 54;
  if(th_timer > 1000){
    timerInSec += 1;
    timerForSleep += 1;
    th_timer = th_timer - 1000;
    kprintf2("%d", timerInSec);
    //kprintf("a %d \n", timerForSleep);
  }
  PIC_sendEOI(0);
}
