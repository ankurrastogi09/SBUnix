#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/global_funcs.h>

char terminal_buffer[10][128];
volatile int readBuffer = 0;
int writeBuffer = 0;
int currentWriteIndex = 0;
volatile int valid[10];

void teminalInit(){
	readBuffer = 0;
	writeBuffer = 0;
	currentWriteIndex = 0;

	for(int i = 0; i < 10; i++){
		valid[i] = 0;
	}

	for(int i = 0; i < 10; i++){
		for(int j = 0; j < 128; j++){
			terminal_buffer[i][j] = '\0';
		}
	}

}

void terminalWrite(char c){
	if(currentWriteIndex >= 128) return;
	if(valid[writeBuffer] == 1) return;
	
	terminal_buffer[writeBuffer][currentWriteIndex] = c;
	currentWriteIndex++;
	
	if(c == '\n'){
		valid[writeBuffer] = 1;
		writeBuffer = (writeBuffer + 1) % 10;
		currentWriteIndex = 0;
	}
}

char* terminalRead(){
	while(1){
		if(valid[readBuffer] == 1) break;
	}
	
	char* resultBuffer;
	resultBuffer = (char*) &terminal_buffer[readBuffer];
	valid[readBuffer] = 0;
	readBuffer = (readBuffer + 1) % 10;
	return resultBuffer;
}

// char** input;
// uint64_t* valid;
// uint64_t currentBuffer;
// uint64_t currentIndex;
// uint64_t size;
// uint64_t flag;
// uint64_t flag1;


// void bufferInitialize()
// {
// 	size = 5;
// 	input = (char **) kmalloc(1); 
// 	for(int i = 0; i < size; i++)
// 	{
// 		input[i] = (char *) kmalloc(1);
// 	}
// 	valid = (uint64_t *) kmalloc(1);
// 	for(int j = 0; j < size; j++)
// 	{
// 		valid[j] = 0;
// 	}
// 	currentBuffer = 0;
// 	currentIndex = 0;
// 	flag = 0;
// 	flag1 = 0;
	
// }

// void bufferWrite(char c)
// {

// 	input[0][currentIndex++] = c;
// 	if(c == '\n') flag = 1;
// 	// if(c != '\n')
// 	// {	
// 	// 	if(flag == 0)
// 	// 	{
// 	// 		input[0][currentIndex++] = c; 
// 	// 	}
// 	// 	else
// 	// 	{
// 	// 		input[1][currentIndex++] = c;
// 	// 	}
// 	// }
// 	// else
// 	// {
// 	// 	if(flag == 0)
// 	// 		{
// 	// 			input[0][currentIndex++] = c;
// 	// 			flag = 1; 
// 	// 			currentIndex = 0;
// 	// 		}
// 	// 		else
// 	// 		{
// 	// 			input[1][currentIndex++] = c;
// 	// 			flag1 = 1;
// 	// 			currentIndex = 0;
// 	// 		}	
// 	// }	
//  }
//  char* bufferRead()
//  {
//  	while(flag == 0) kprintf("");
//  	flag = 0;
//  	currentIndex = 0;
 	
//  	return input[0];
//   	// while((flag == 0) && (flag1 == 0));
//   	// if(flag == 1)
//   	// {
//   	// 	flag = 0;
//   	// 	return input[0];
//   	// }
//   	// else
//   	// {
//   	// 	flag1 = 0;
//   	// 	return input[1];
//   	// }

//  }
// // void bufferWrite(char c)
// // {
// // 	if(valid[currentBuffer] == 1) return;
// // 	if(c != '\n')
// // 	{	
// // 		if(valid[currentBuffer] == 1) return;
// // 		input[currentBuffer][currentIndex++] = c;
// // 	}	
// // 	else
// // 	{
// // 		input[currentBuffer][currentIndex] = '\n';
// // 		//kprintf("%s",input[currentBuffer]);
// // 		valid[currentBuffer] = 1;
// // 		currentBuffer = (currentBuffer + 1) % size;
// // 		currentIndex = 0;
// // 	}
// // }

// // char* bufferRead()
// // {
// // 	uint64_t index = (currentBuffer + 1) % size; //curernt buffer jismai mai likh raha hun
// // 	if(valid[index] == 1)
// // 	{
// // 		//int temp = currentBuffer;
// // 		valid[index] = 0;
// // 		//currentBuffer = index;
// // 		return input[index];
// // 	}
// // 	else //if(valid[index] == 0)
// // 	{	
// // 		while(((index + 1) != currentBuffer) && (valid[index] == 0))
// // 		{
// // 			index = (index + 1) %size;
// // 		}
// // 		if(valid[index] == 1)
// // 		{
// // 			valid[index] = 0;
// // 			return input[index];
// // 		}
// // 		else
// // 		{
// // 			index = currentBuffer;
// // 			while(1)
// // 			{
// // 				//kprintf1("Yahan atak gaya \n");
// // 				if(valid[index] == 0) 
// // 					continue;
// // 				else
// // 					{
// // 						valid[index] = 0;
// // 						return input[index];
// // 					}
// // 			}
// // 		}
// // 	}
// // 	return input[0];
// // 	/*else
// // 	{
// // 		while((valid[index] == 1) && (index != currentBuffer))
// // 		{
// // 			index = (index +size  - 1) % size;
// // 		}
// // 		return input[index+1];
// // 	}*/
// // }
