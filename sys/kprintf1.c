// #include <sys/kprintf.h>
// #include <stdio.h>
// #include <sys/stdarg.h>
// #include <sys/defs.h>

// //Note for Anand: we can keep this uint64_t as well.
// extern uint64_t video_card_pointer;
// extern char* point;
// extern char* startPoint;
// extern char* startEndLine;
// extern char* temp;

// //char* point = (char *) 0xFFFFFFFF802DA000;
// //char* startPoint = (char *) 0xFFFFFFFF802DA000;
// int increment = 0;
// //char* startEndLine = (char *) 0xFFFFFFFF802DA000;
// int i,j,k,q;
// //char* temp = (char *) 0xFFFFFFFF802DA000;
// int timerIncrement = 0;

// void memcpy_kprintf(char* src, char* dest){
//     *dest = *src;
//     *src = '\0';
// }


// char f(int a)
// {
//   if(a == 0)
//   {
//     return '0';
//   }
//   else if(a == 1)
//   {
//     return '1';
//   }
//   else if(a == 2)
//   {
//     return '2';
//   }
//   else if(a == 3)
//   {
//     return '3';
//   }
//   else if(a == 4)
//   {
//     return '4';
//   }
//   else if(a == 5)
//   {
//     return '5';
//   }
//   else if(a == 6)
//   {
//     return '6';
//   }
//   else if(a == 7)
//   {
//     return '7';
//   }
//   else if(a == 8)
//   {
//     return '8';
//   }
//   else if(a == 9)
//   {
//     return '9';
//   }
//   return '0';
// }

// char fx(int a)
// {
//   if(a == 0)
//   {
//     return '0';
//   }
//   else if(a == 1)
//   {
//     return '1';
//   }
//   else if(a == 2)
//   {
//     return '2';
//   }
//   else if(a == 3)
//   {
//     return '3';
//   }
//   else if(a == 4)
//   {
//     return '4';
//   }
//   else if(a == 5)
//   {
//     return '5';
//   }
//   else if(a == 6)
//   {
//     return '6';
//   }
//   else if(a == 7)
//   {
//     return '7';
//   }
//   else if(a == 8)
//   {
//     return '8';
//   }
//   else if(a == 9)
//   {
//     return '9';
//   }
//   else if(a == 10)
//   {
//     return 'A';
//   }
//   else if(a == 11)
//   {
//     return 'B';
//   }
//   else if(a == 12)
//   {
//     return 'C';
//   }
//   else if(a == 13)
//   {
//     return 'D';
//   }
//  else if(a == 14)
//   {
//     return 'E';
//   }
//   else if(a == 15)
//   {
//     return 'F';
//   }
//   return '0';
// }

// char fxLong(long a)
// {
//   if(a == 0)
//   {
//     return '0';
//   }
//   else if(a == 1)
//   {
//     return '1';
//   }
//   else if(a == 2)
//   {
//     return '2';
//   }
//   else if(a == 3)
//   {
//     return '3';
//   }
//   else if(a == 4)
//   {
//     return '4';
//   }
//   else if(a == 5)
//   {
//     return '5';
//   }
//   else if(a == 6)
//   {
//     return '6';
//   }
//   else if(a == 7)
//   {
//     return '7';
//   }
//   else if(a == 8)
//   {
//     return '8';
//   }
//   else if(a == 9)
//   {
//     return '9';
//   }
//   else if(a == 10)
//   {
//     return 'A';
//   }
//   else if(a == 11)
//   {
//     return 'B';
//   }
//   else if(a == 12)
//   {
//     return 'C';
//   }
//   else if(a == 13)
//   {
//     return 'D';
//   }
//  else if(a == 14)
//   {
//     return 'E';
//   }
//   else if(a == 15)
//   {
//     return 'F';
//   }
//   return '0';
// }

// void convertDecimal(int number)
// {
//   if(number <  10)
//   {   
//       char c = f(number);
//       *point++ = c;
//       increment++;
//       *point++ = 31;
//       increment++;
//   }
//   else
//   {
//     int k = number%10;
//     convertDecimal(number/10);
//     char c = f(k);
//       *point++ = c;
//       increment++;
//       *point++ = 31;
//       increment++;
//   }
// }

// void convertTimerDecimal(int number)
// {
//   if(number <  10)
//   {
//       char c = f(number);
//       *(startEndLine + timerIncrement) = c;
//       timerIncrement++;
//       *(startEndLine + timerIncrement) = 31;
//       timerIncrement++;
//   }
//   else
//   {
//     int k = number%10;
//     convertTimerDecimal(number/10);
//     char c = f(k);
//     *(startEndLine + timerIncrement) = c;
//     timerIncrement++;
//     *(startEndLine + timerIncrement) = 31;
//     timerIncrement++;

//   }
// }

// void convertHexaDecimal(int number)
// {
//   if(number <  16)
//   {
//       char c = fx(number);
//       *point++ = c;
//       increment++;
//       *point++ = 31;
//       increment++;
//   }
//   else
//   {
//     int k = number%16;
//     convertHexaDecimal(number/16);
//     char c = fx(k);
//       *point++ = c;
//       increment++;
//       *point++ = 31;
//       increment++;
//   }
// }

// void convertHexaDecimalLong(long number)
// { 
//   if(number <  16)
//   {   
//       char c = fxLong(number);
//       *point++ = c;
//       increment++;
//       *point++ = 31;
//       increment++;
//   }
//   else
//   { 
//     int k = number%16;
//     convertHexaDecimal(number/16);
//     char c = fxLong(k);
//       *point++ = c;
//       increment++;
//       *point++ = 31;
//       increment++;
//   }
// }



// void kprintf(const char *fmt, ...)
// { 
//   increment = 0;
//   for(i=0;i<3922;i++)
//   {
//         temp++;
//   }
//   //startEndLine = (char*) 0xFFFFFFFF802DA000;
//   for( k =0; k< 3944; k++) 
//   {
// 	startEndLine++;
//   }
//   timerIncrement = 0; 
//   char* start = startEndLine;
//   unsigned int i = 70 ;
//   va_list arguments;
//   va_start(arguments, fmt);
//   for(int count =0; *(fmt+count) != '\0'; count++)
//   {
//     *start = *(fmt+count);
//     while((*start != '%') && (*start != '\n'))
//     {
//       *point++ = *start;
//       increment++;
//       *point++ = 31;
//       increment++;
//       count++;
//       *start = *(fmt+count);

//     }
//     if(*start == '%')
//     {
//       count++;
//     *start = *(fmt+count);
//     if(*start == 'c')
//     {        
//       i = va_arg(arguments, int);
//       *point++ = i;
//       increment++;
//       *point++ = 31;
//       increment++;
//     }
//     else if(*start == 'd')
//     {
//       i = va_arg(arguments, int);
//       if(i<0)
//       {
//         i = -i;
//         *point++ = '-';
//         increment++;
//         *point++ = 31;
//         increment++;
//       }
//       convertDecimal(i);
//     }
//     else if(*start == 't')
//     {
//       i = va_arg(arguments, int);
//       convertTimerDecimal(i);
//     }
 
//     else if(*start == 'i')
//     {
//       i = va_arg(arguments, int);
//      // *(startEndLine -  8) = i;
//      // *(startEndLine -  7) = 31;  
//       //temp = (char*)  0xB8F64; 
//       *temp++ = i;
//       temp++;
//        //*temp++ = i;
//        if(temp == (char*)  0xB8F56)
//        {
//          temp = (char*)  0xB8F52; 
//        }
//      }

//     else if(*start == 'x')
//     {
//       i = va_arg(arguments, int);
//       if(i<0)
//       {
//         i = -i;
//         *point++ = '-';
//         increment++;
//         *point++ = 31;
//         increment++;
//       }
//       convertHexaDecimal(i);
//     }
//     else if(*start == 's')
//     {
//       char* sTemp  = va_arg(arguments, char*);
//       int tempCnt = 0;
//       while(*(sTemp+tempCnt) != '\0')
//       {
//         *point++ = *(sTemp+tempCnt);
//         tempCnt++;
//         increment++;
//         *point++ = 31;
//         increment++;
//       }
//     }
//     else if(*start == 'p')
//     {
//        i = va_arg(arguments, int);
//        *point++ = '0';
//         increment++;
//         *point++ = 31;
//         increment++;
//         *point++ = 'x';
//         increment++;
//         *point++ = 31;
//         increment++;
//        convertHexaDecimalLong(i);     
//     }

//     }
//     else if(*start == '\n')
//     {
//       int number = increment/160;
      
//      point = startPoint;	
//      for( q= 0; q < ((number+1)*160);q++) 
//      {
//      	point++;
//      }
//      startPoint = point;
//     }
//     else if(*start == '\r')
//     {
//       //int number = increment/160;
//       point = startPoint;  //+ ((number) * 0xA0);
//       startPoint = point;
//     }
//   }
// }
