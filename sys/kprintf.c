#include <stdio.h>
#include <sys/stdarg.h>
#include <sys/defs.h>

//Note for Anand: we can keep this uint64_t as well.
extern uint64_t video_card_pointer;
extern char* point;
char* point1 = NULL;
uint64_t newLineNumber = 0;
uint64_t newLineNumber1 = 14;
uint64_t newIndex = 0;
uint64_t newIndex1 = 0;
int i = 0;

void memcpy_kprintf(char* src, char* dest){
    *dest = *src;
    *src = '\0';
}



// This is just integer to alphabet implementation
char f(int a)
{
  if(a == 0)
  {
    return '0';
  }
  else if(a == 1)
  {
    return '1';
  }
  else if(a == 2)
  {
    return '2';
  }
  else if(a == 3)
  {
    return '3';
  }
  else if(a == 4)
  {
    return '4';
  }
  else if(a == 5)
  {
    return '5';
  }
  else if(a == 6)
  {
    return '6';
  }
  else if(a == 7)
  {
    return '7';
  }
  else if(a == 8)
  {
    return '8';
  }
  else if(a == 9)
  {
    return '9';
  }
  return '0';
}


// This is integer to alphabet in hexadecimal from 10-15 it returns A-F
char fx(int a)
{
  if(a == 0)
  {
    return '0';
  }
  else if(a == 1)
  {
    return '1';
  }
  else if(a == 2)
  {
    return '2';
  }
  else if(a == 3)
  {
    return '3';
  }
  else if(a == 4)
  {
    return '4';
  }
  else if(a == 5)
  {
    return '5';
  }
  else if(a == 6)
  {
    return '6';
  }
  else if(a == 7)
  {
    return '7';
  }
  else if(a == 8)
  {
    return '8';
  }
  else if(a == 9)
  {
    return '9';
  }
  else if(a == 10)
  {
    return 'A';
  }
  else if(a == 11)
  {
    return 'B';
  }
  else if(a == 12)
  {
    return 'C';
  }
  else if(a == 13)
  {
    return 'D';
  }
 else if(a == 14)
  {
    return 'E';
  }
  else if(a == 15)
  {
    return 'F';
  }
  return '0';
}

//Similar to above function
char fxLong(uint64_t a)
{
  if(a == 0)
  {
    return '0';
  }
  else if(a == 1)
  {
    return '1';
  }
  else if(a == 2)
  {
    return '2';
  }
  else if(a == 3)
  {
    return '3';
  }
  else if(a == 4)
  {
    return '4';
  }
  else if(a == 5)
  {
    return '5';
  }
  else if(a == 6)
  {
    return '6';
  }
  else if(a == 7)
  {
    return '7';
  }
  else if(a == 8)
  {
    return '8';
  }
  else if(a == 9)
  {
    return '9';
  }
  else if(a == 10)
  {
    return 'A';
  }
  else if(a == 11)
  {
    return 'B';
  }
  else if(a == 12)
  {
    return 'C';
  }
  else if(a == 13)
  {
    return 'D';
  }
 else if(a == 14)
  {
    return 'E';
  }
  else if(a == 15)
  {
    return 'F';
  }
  return '0';
}


// This function convert decimal number into string recursively and prints every character in each iteration
void convertDecimal(int number)
{

  char *point =  (char *) video_card_pointer;

  for(int l = 0; l < newLineNumber*160; l++)
  {
    point++;
  }

  for(int k = 0; k< newIndex; k++)
  {
        point++;
  }
 
 if(number <  10)
  {   
      char c = f(number);
      *point++ = c;
      *point++ = 31;
      newIndex += 2;
  }
  else
  {
    int k1 = number%10;
    convertDecimal(number/10);
    char c1 = f(k1);
  point =  (char *) video_card_pointer;
     for(int l = 0; l < newLineNumber*160; l++)
  {
    point++;
  }

  for(int k = 0; k< newIndex; k++)
  {
        point++;
  }

      *point++ = c1;
      *point++ = 31;
      newIndex += 2;
  }
}

// Similar to above function for Hexa Decimal
void convertHexaDecimal(int number)
{

  char *point =  (char *) video_card_pointer;

  for(int l = 0; l < newLineNumber*160; l++)
  {
    point++;
  }

  for(int k = 0; k< newIndex; k++)
  {
        point++;
  }


  if(number <  16)
  {
      char c = fx(number);
      *point++ = c;
      *point++ = 31;
      newIndex += 2;
  }
  else
  {
    int k = number%16;
    convertHexaDecimal(number/16);
    char c = fx(k);
      *point++ = c;
      *point++ = 31;
      newIndex += 2;
  }
}

//Similar to ablove function  For Hexa Decimal Long
void convertHexaDecimalLong(uint64_t number)
{
  char *point =  (char *) video_card_pointer;

  for(int l = 0; l < newLineNumber*160; l++)
  {
    point++;
  }

  for(int k = 0; k< newIndex; k++)
  {
        point++;
  }
  

 
  if(number <  16)
  {   
      char c = fxLong(number);
      *point++ = c;
      *point++ = 31;
      newIndex += 2;
  }
  else
  { 
    int k = number%16;
    convertHexaDecimal(number/16);
    char c = fxLong(k);
      *point++ = c;
      *point++ = 31;
       newIndex += 2;
  }
}


// This scrolls up lined by 12 lines
void newScrollup()
{
    char* newSrcStart = (char *) video_card_pointer;
    char* newDestStart = (char *) video_card_pointer;
    for(int i =0 ; i< 160; i++)
    {
        newSrcStart++;
    }

    for(int l = 0; l < 12*160 ; l++)
    {
            memcpy_kprintf(newSrcStart++, newDestStart++);
    }
}

void newScrollup1()
{
    char* newSrcStart1 = (char *) video_card_pointer;
    char* newDestStart1 = (char *) video_card_pointer;
    for(int i =0 ; i< 15*160; i++)
    {
        newSrcStart1++;
    }

    for(int i =0 ; i< 14*160; i++)
    {
        newDestStart1++;
    }

    for(int l = 0; l < 9*160 ; l++)
    {
            memcpy_kprintf(newSrcStart1++, newDestStart1++);
    }
}


//Refer to previous kprintf to implement %d, %x, %p




// This is normal KPrintf
// NewLineNumber shows at whhich line now the values should be printed and
// NewIndex shows at current line at which index values should be printed
//NewLineNumber starts from value 0 and writes till 12. At 13 it starts scrolling up 
void kprintf(const char *fmt, ...)
{ 
  // pointer at the start of video memory is taken
  point = (char*)video_card_pointer;
  // pointer is scrolled down by newLineNumber number of lines
  for(int l = 0; l < newLineNumber*160; l++)
  {
    point++;
  }
  // pointer is incremented newIndex times
  for(int k = 0; k< newIndex; k++)
  {
    point++;
  }
  // The whole string to be printed is copied in string fmt and it is traversed
  va_list arguments;
  va_start(arguments, fmt);
  // count variable maintains the index at which character is being written from fmt
  // all the characters are printed until we encouter any special character
  for(int count =0; *(fmt+count) != '\0'; count++)
  {
    while((*(fmt+count) != '%') && (*(fmt+count) != '\n') && (*(fmt+count) != '\0'))
    {
      *point++ = *(fmt+count);
      *point++ = 31;
      count++;
      newIndex += 2; //newIndex is incremented by 2 one for character and one for colour
     // If we write more than 160 characters in one line pointer is taken to next line and scrolling condition is also checked
      if(newIndex >= 160)
      {
    newIndex = 0;
    newLineNumber++;
    if(newLineNumber >= 13)
    {
             newScrollup();
                   newLineNumber = 12;
        }
    
      }
       
    }
    // if string ends we return
    if(*(fmt+count) == '\0') return;
    // if we encounter \n we need to go one line down and check scrolling condition as well
    else if(*(fmt+count) == '\n')
    {
  newIndex = 0;
  newLineNumber++;
    
      if(newLineNumber >= 13)
      {
    newScrollup();
    newLineNumber = 12;
      }
    }
    // This is for %s and it is working fine
    else if(*(fmt+count+1) == 's')
    {
      count++;
      char* sTemp  = va_arg(arguments, char*); // we copy argument passed after %s into sTemp and print it
      int tempCnt = 0;
      while(*(sTemp+tempCnt) != '\0')
      {
        *point++ = *(sTemp+tempCnt);
        tempCnt++;
        *point++ = 31;
        newIndex += 2;
      }
    }
   // This commented code is creating some problem, it is written for %d
  
  else if(*(fmt+count+1) == 'd')
   {
        count++;
     

  if(newIndex >= 160)
      {
    newIndex = 0;
    newLineNumber++;
    if(newLineNumber >= 13)
    {
             newScrollup();
                   newLineNumber = 12;
        }

      }






  i = va_arg(arguments, int);
       if(i<0)
       {
         i = -i;
         *point++ = '-';
         *point++ = 31;
    newIndex += 2;
       }
      convertDecimal(i);
   }
  }
  
}



// This is TermWrite (All variables are similar to earlier kprintf, have just added 1 at the end of every variable)
// NewLineNumber start from 14 and writes till 23, at 24 it starts scolling up
void kprintf1(const char *fmt, ...)
{
  point1 = (char*)video_card_pointer;
  for(int l = 0; l < newLineNumber1*160; l++)
  {
    point1++;
  }
  for(int k = 0; k< newIndex1; k++)
  {
        point1++;
  }


  va_list arguments;
  va_start(arguments, fmt);
  for(int count =0; *(fmt+count) != '\0'; count++)
  {
//    char *start = (char *)(fmt+count);
    while((*(fmt+count) != '%') && (*(fmt+count) != '\n') && (*(fmt+count) != '\0'))
    {
      *point1++ = *(fmt+count);
      *point1++ = 31;
      //point1++;
      count++;
      //*start = *(fmt+count);
        newIndex1 += 2; 
      if(newIndex1 >= 160)
      {
                newIndex1 = 0;
                newLineNumber1++; 
                if(newLineNumber1 >= 24)
                {
                         newScrollup1();
                         newLineNumber1 = 23;
                }
       
      }


    }
    if(*(fmt+count) == '\0') return;
    else if(*(fmt+count) == '\n')
    {
        newIndex1 = 0;
        newLineNumber1++;
        if(newLineNumber1 >= 24)
        {
                newScrollup1();
                newLineNumber1 = 23;
        }
    }
    else if(*(fmt+count+1) == 's')
    {
      count++;
      char* sTemp  = va_arg(arguments, char*); // we copy argument passed after %s into sTemp and print it
      int tempCnt = 0;
      while(*(sTemp+tempCnt) != '\0')
      {
        *point1++ = *(sTemp+tempCnt);
        tempCnt++;
        *point1++ = 31;
        //point1++;
        newIndex1 += 2;
        if(newIndex1 >= 160)
              {
                        newIndex1 = 0;
                        newLineNumber1++; 
                        if(newLineNumber1 >= 24)
                        {
                                 newScrollup1();
                                 newLineNumber1 = 23;
                        }
               
              }
      }
    }
    
    if(newLineNumber1 >= 24)
    {
         newScrollup1();
         newLineNumber1 = 23;
    }
    
  }

  

}

int newIndex2 = 0;

void convertDecimal2(int number)
{

  char *point2 =  (char *) video_card_pointer;

  for(int l = 0; l < 24*160; l++)
  {
    point2++;
  }

  for(int k = 0; k< newIndex2; k++)
  {
        point2++;
  }

 if(number <  10)
  {
      char c = f(number);
      *point2++ = c;
      *point2++ = 31;
      newIndex2 += 2;
  }
  else
  {
    int k1 = number%10;
    convertDecimal2(number/10);
    char c1 = f(k1);
  point2 =  (char *) video_card_pointer;
     for(int l = 0; l < 24*160; l++)
  {
    point2++;
  }

  for(int k = 0; k< newIndex2; k++)
  {
        point2++;
  }

      *point2++ = c1;
      *point2++ = 31;
      newIndex2 += 2;
  }
}



void kprintf2(const char *fmt, ...)
{
	va_list arguments;
	va_start(arguments, fmt);
	newIndex2 = 0;
	int i = va_arg(arguments, int);
	convertDecimal2(i);
}
