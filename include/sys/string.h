#ifndef _STRING_H
#define _STRING_H

#include <stdio.h>
#include <sys/defs.h>

size_t strlen(const char *str){
	const char *cstr;
	for(cstr = str; *cstr; ++cstr);
	return (cstr-str);
}


int memcmp(const void* str1, const void* str2, size_t size){
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

void memcpy(void *dst, void *src, size_t count){
  char *local_dst = (char*) dst;
  char *local_src = (char*) src;

  for(int i = 0; i < count; i++){
    local_dst[i] = local_src[i];
  }
}

int atoi(char* strForm){
	int numberForm = 0;

	for(int i = 0; strForm[i] != '\0'; i++){
		numberForm = numberForm * 10 + strForm[i] - '0';
	}

	return numberForm;
}

void strcat(char* firstStr, const char* secondStr){
	int i = 0; 
	int j = 0;
	
	while(*(firstStr + i) != '\0'){
		i++;
	}

	while(*(secondStr + j)  != '\0'){
		*(firstStr + i + j) = *(secondStr + j);
		j++;
	}

	*(firstStr + i + j) = '\0';
}

void reverse(char* str){
	int len = 0;
	while(*(str + len) != '\0'){
		len++;
	}

	char temp;
	for(int i = 0; i < len/2; i++){
		temp = '\0';
		temp = *(str + i);
		*(str + i) = *(str + len -i);
		*(str + len -i) =  temp;
	}
}

//Refernce http://www.geeksforgeeks.org/implement-itoa/
char* itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;
 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str);
 
    return str;
}

/*void memcpy1(void* dest, void* src, size_t n){
	char* src_pointer = (char*) src;
	char* dest_pointer = (char*) dest;

	while(uint64_t i = 0; i < n; i++){
		src_pointer[i] = dest_pointer[i];
	}
}*/

#endif
