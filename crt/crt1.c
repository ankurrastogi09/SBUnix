#include <stdlib.h>
#include <stdio.h>

void _start(uint64_t argc, char* argv[], char *envp[]) {
//	int status;
	main(argc, argv, envp);
//	exit(status);

	// call main() and exit() here
	// int status;
	// int i = 14;
	// int count = 0;

	// uint64_t a3 = *(&argc+i);

	// char* argv1[a3];
	// i++;
	// while(count < a3){
	//       argv1[count] = (char *) *(&argc+i);
	//       i++;
	//       count++;
	// }

	// i++;
	// count = 0;
	// while(*(&argc+i) != 0){
	//   count++;
	//   i++;
	// }

	// i -= count;
	// char* envp1[count];

	// count = 0;
	// while(*(&argc+i) != 0){
	//   envp1[count] = (char *) *(&argc+i);
	//   env_global_variable[count] = (char *) *(&argc+i);
	//   count++;
	//   i++;
	// }
	// envp1[count] = NULL;
	// env_global_variable[count] = NULL;

	// status = main((int) a3, argv1, envp1);

	// exit(status);
}
