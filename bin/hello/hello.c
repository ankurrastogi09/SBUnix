#include <stdio.h>
#include <sys/defs.h>
#include <sys/syscall.h>
#include <sys/global_vars.h>
#include <unistd.h>
#include <sys/string.h>
#include <sys/struct_utility.h>

int main(int argc, char* argv[]){

	//char buf2[20] = "I am parent process\n";

	//char buf[11] = "I am child\n";
	char* openpathname;
	int openflag = 2;
	int fd;

	char readbufFromFile[FILE_READ_BUFFER_LENGTH];


	//write(1, (void*) &buf, 10);	

	//write(1, (void*) &buf2, 19);

	// pid_t pid = fork();

	// if(pid == 0){
	// 	write(1, (void*) &buf, 10);	
	// } else {
	// 	write(1, (void*) &buf2, 19);
	// }
	//if(pid == 0){}
	
	//write(1, (void*) &buf2, 19);
	//write(1, (void*) &buf, 10);

	// char* openpathname = "etc/abc.txt";
	// int openflag = 2;
	// int fd = open(openpathname, openflag);

	// if(fd > 2){
	// 	for(int i = 0; i < FILE_READ_BUFFER_LENGTH; i++){
	// 		readbufFromFile[i] = '\0';
	// 	}
	// 	//reading from file
	// 	read(fd, (void*) readbufFromFile, FILE_READ_BUFFER_LENGTH);
	// 	//writing on to terminal
	// 	write(1, (void*) readbufFromFile, FILE_READ_BUFFER_LENGTH);
	// }
	
	char readbuf[NORMAL_READ_BUFFER_LENGTH];

	write(1, (void*) "Note: ls, cd etc, cd etc/test, cat etc/abc.txt (It will require full path), echo hello, sleep 100, ps\n", 200);

	while(1){
		
		for(int i = 0; i < NORMAL_READ_BUFFER_LENGTH; i++){
			readbuf[i] = '\0';
		}

		write(1, (void*) "\nsbush> ", 8);
		
		read(0, &readbuf,NORMAL_READ_BUFFER_LENGTH);
		
		if(readbuf[0] != '\0'){
			//write(1, (void*) &readbuf, 20);
			
			if(readbuf[0] == 'e' && readbuf[1] ==  'c' && readbuf[2] == 'h' && readbuf[3] == 'o' && readbuf[4] == ' '){
				int i = 5;
				
				char temp_buf[NORMAL_READ_BUFFER_LENGTH];
				for(int i = 0; i < NORMAL_READ_BUFFER_LENGTH; i++){
					temp_buf[i] = '\0';
				}

				while(*(readbuf+i) != '\0'){
					temp_buf[i-5] = readbuf[i];
					i++;
				}

				*(temp_buf+i) = '\0';
				
				write(1, (void*) &temp_buf, NORMAL_READ_BUFFER_LENGTH);
			}

			else if(readbuf[0] == 's' && readbuf[1] == 'l' && readbuf[2] == 'e' && readbuf[3] == 'e' && readbuf[4] == 'p' && readbuf[5] == ' '){
				int i = 6;
				char temp_buf[NORMAL_READ_BUFFER_LENGTH];
				for(int i = 0; i < NORMAL_READ_BUFFER_LENGTH; i++){
					temp_buf[i] = '\0';
				}

				while(*(readbuf+i) != '\0'){
					temp_buf[i-6] = readbuf[i];
					i++;
				}
				*(temp_buf+i) = '\0';

				//write(1, (void*) &temp_buf, NORMAL_READ_BUFFER_LENGTH);
				sleep(atoi(temp_buf));
			}

			else if(readbuf[0] == 'p' && readbuf[1] == 's'){
				// char* psDetails;
				// psDetails = ps(1000);
				// write(1, (void*) psDetails, 100);
				write(1, (void*) "PID 0\n", 6);
				write(1, (void*) "PID 1", 5);
			}

			else if(readbuf[0] == 'c' && readbuf[1] == 'a' && readbuf[2] == 't' && readbuf[3] == ' '){
				
				int i = 4;
				char temp_buf[NORMAL_READ_BUFFER_LENGTH];
				for(int i = 0; i < NORMAL_READ_BUFFER_LENGTH; i++){
					temp_buf[i] = '\0';
				}

				if(*temp_buf == '/' || *temp_buf == '\\'){
					i = 5;
				}

				while(*(readbuf+i) != '\0'){
					temp_buf[i-4] = readbuf[i];
					i++;
				}

				*(temp_buf+i) = '\0';

				openpathname = temp_buf;
				fd = open(openpathname, openflag);

				if(fd == 0){
					write(1, (void*) "No such file found!!!", 21);
				}
				
				if(fd > 2){
					for(int i = 0; i < FILE_READ_BUFFER_LENGTH; i++){
						readbufFromFile[i] = '\0';
					}
					//reading from file
					read(fd, (void*) readbufFromFile, FILE_READ_BUFFER_LENGTH);
					//writing on to terminal
					write(1, (void*) readbufFromFile, FILE_READ_BUFFER_LENGTH);
					close(fd);
				}
			}
			else if(readbuf[0] == 'l' && readbuf[1] == 's' && readbuf[2] == '\0'){
				readdir1();
			}
			else if(readbuf[0] == 'c' && readbuf[1] == 'd' && readbuf[2] == ' '){
				int i = 3;
				char temp_buf[NORMAL_READ_BUFFER_LENGTH];
				
				for(int i = 0; i < NORMAL_READ_BUFFER_LENGTH; i++){
					temp_buf[i] = '\0';
				}

				while(*(readbuf+i) != '\0'){
					temp_buf[i-3] = readbuf[i];
					i++;
				}

				*(temp_buf+i) = '\0';

				openpathname = temp_buf;

				chdir(openpathname);
			}
			else{

			}

		}
	}

	

	// char readbuf[20];
	// read(1, &readbuf, 20);

	// exit()
	// //wait()
	// sleep()
	// echo()
	// seek()
	// segv()
	// opendir()
	// closedir()
	// readdir()
	// cat 
	// ls 
	// kill -9
	// ps


	// write(1, (void*) &buf2, 19);

	// write(1, (void*) &buf2, 19);

	// write(1, (void*) &buf2, 19);

	// read(1, (void*) &buf, 10);


	// fork();
	// // if(pid1 == 0){}
	// write(1, (void*) &buf2, 19);

	// fork();
	// // if(pid2 == 0){}
	// write(1, (void*) &buf2, 19);

	while(1);
}
