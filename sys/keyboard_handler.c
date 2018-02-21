#include <sys/kprintf.h>

void kprintf1(const char *fmt, ...);
//void bufferWrite(char c);
void terminalWrite(char c);
void PIC_sendEOI(unsigned char irq);
unsigned char inportb(unsigned int port);
int shiftFlag = 0;
int controlFlag = 0;

unsigned char map[128] = {0,27,'1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q', 'w', 'e', 'r','t', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',0,'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
'\'','`',0,'\\', 'z', 'x', 'c', 'v', 'b', 'n','m', ',', '.', '/',   0,'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'-',0,0,0,'+',0,0,0,0,0,0,0,0,0,0,0,};

void keyboard_handler(){
	unsigned char scancode;
	scancode = inportb(0x60);
        char mychar;
	mychar = map[scancode]; 
      /* if((scancode == 42) || (scancode == 54)) 
        {
          shiftFlag++;
         
        }
        else if(scancode == 29)
        {
          controlFlag++;
        }
        else
        {
          if(shiftFlag % 2 == 1)
          {
            mychar = map[scancode] - 32;
            kprintf("%d", mychar);
            shiftFlag = 0;
          }
          else if(controlFlag % 2 == 1)
          {
            mychar = map[scancode] - 32;
            //kprintf("%d", '^');
            kprintf("%d", mychar);
            controlFlag = 0;
          }
          else
          {
            mychar = map[scancode];
            //if(mychar == 1){}
            kprintf("%d", mychar); 
          }
          //shiftFlag = 0;
          //controlFlag = 0;
	  //kprintf("%i", mychar);
        }
	*/
	if((scancode >= 0) && (scancode < 128))
	{
		char temp[2];
		temp[0] = mychar;
		temp[1] = '\0';
		kprintf1(temp);
		//bufferWrite(mychar);
    terminalWrite(mychar);
	}

        PIC_sendEOI(1);
}
