// #include <stdio.h>
// #include<unistd.h>
// #define O_RDONLY	0x0000

// void executeCommand(char command[]);
// char* promptTextMessage;

// void cd(char command[])
// {
//   int index = 0;
//   while(command[index] != ' ')
//   {
//     index++;
//   }
//   while(command[index] == ' ')
//   {
//     index++;
//   }

//   if(chdir(command + index) == 0)
//   {
//   }
//   else
//   {
//   }
// }

// void executeBinaries(char command[])
// {
//   char* token[10];
//   int count = 0;
//   int index =0;
//   int first = 0;
//   while(command[index] != '\0')
//   {
//     while((command[index] != '\0') && (command[index] != ' '))
//     {
//        index++;
//     }
//     if(command[index] == ' ')
//     {
//        command[index] = '\0';
//        index++;
//        token[count++] = command + first;
//        first = index;
//     }
//     else
//     {
//        token[count++] = command + first;
//        break;
//     }
//   }
//   token[count] = NULL;
//   for(int i = 0 ;i < count ; i++)
//   {
//     // puts("TOKEN");
//     // puts(token[i]);
//   }

//   char* pathVar = getenv("PATH");


//   int k = 0;

//   while(pathVar[k] != '\0'){
//     k++;
//   }

//   int j = 0;
//   while(token[0][j] != '\0'){
//     pathVar[k] = token[0][j];
//         k++;
//         j++;
//   }

//   pathVar[k] = '\0';


//   int y = access(pathVar, 0);

//   if(y == 0){

//   token[0] = pathVar;


//   execvp(token[0], token);
//  } else {
//     puts("Not a valid path");
//     //execvp(token[0], token);
//    pathVar[0] = '\0';
// }
// }

// void shellScript(char command[])
// {
//   char* token;
//   // int index = 0;
//   // while(command[index] != ' ')
//   // {
//   //   index++;
//   // }
//   // while(command[index] == ' ')
//   // {
//   //   index++;
//   // }

//   token = command ; //+ index;
//   // FILE *filePointer;
//   // puts(token);
//   char line[255];
//   // printf("%s\n", token);
//   //filePointer = fopen(token,"r");
//   int file = open(token, O_RDONLY);
//   if(read(file, line, 255) != 0)
//   {
//      int count = 0;
//      char* s = line;
//      if(s[0] == '#')
//      {
//         while(s[count] != '\n')
//         {
//           count++;
//         }
//         s = s + count +1;
//      }
//      while(s[count] != '\0')
//      {
//         char* first = s+count;
//         while((s[count] != '\0') && (s[count] != '\n'))
//         {
//           count++;
//         }
//         if(s[count] == '\0')
//         {
//           // printf("CHAR : %c\n",s[count-1]);
//           // printf("%s",first);
//           executeCommand(first);
//           break;
//         }
//         else
//         {
//           // printf("CHAR : %c",s[count-1]);
//           s[count] = '\0';
//           // printf("%s",first);

//           pid_t childpid;
//           childpid = fork();
//           if(childpid == 0)
//           {
//             executeCommand(first);
//           }
//           count++;
//         }
//      }
//   }
// }


// void setEnvVar(char command[]){
//   char envVar[50];
//   char envVal[50];

//   int index = 0;
//   int ivar = 0;
//   int ival = 0;

//   while(command[index] != ' '){
//     index++;
//   }

//   index++;

//   while(command[index] != '='){
//     envVar[ivar] = command[index];
//     index++;
//     ivar++;
//   }

//   envVar[ivar] = '\0';

//   index++;

//   while(command[index] != ' ' && command[index] != '\0'){
//     envVal[ival] = command[index];
//     index++;
//     ival++;
//   }

//   envVal[ival] = '\0';

//   puts(envVar);
//   puts(envVal);

//    //printf("envVar is %s \n", envVar);
//   // printf("envVal is %s \n", envVal);

//   //int iPTM = 0;

//   if(envVar[0] == 'P' && envVar[1] == 'S' && envVar[2] == '1') {
//     //promptTextMessage = envVal;
//     /*while(iPTM != ivar){
//       promptTextMessage[iPTM] = envVal[iPTM];
// 	iPTM++;
// 	puts(promptTextMessage);
//     }*/

//   }

//   setenv((char *)envVar, envVal, 1);

// }

// void executeCommand(char command[])
// {
//   if(((command[0]) == 's') && ((command[1]) == 'h'))
//   {
//     char* token;
//     int index = 0;
//     while(command[index] != ' ')
//     {
//       index++;
//     }
//     while(command[index] == ' ')
//     {
//       index++;
//     }

//     token = command + index;
//     shellScript(token);
//   }
//   else if(((command[0]) == '.') && ((command[1]) == '/'))
//   {
//     shellScript(command + 1);
//   }
//   else if(((command[0]) == 'c') && ((command[1]) == 'd'))
//   {
//     cd(command);
//   }
//   else if(command[0] == 'e' && command[1] == 'x' && command[2] == 'p' && command[3] == 'o' && command[4] == 'r' && command[5] == 't'){
//      setEnvVar(command);
//   }
//   else
//   {
//     executeBinaries(command);
//   }

// }


// void executeShellCommand(char command[])
// {
//   if(((command[0]) == 'c') && ((command[1]) == 'd'))
//   {
//     cd(command);
//   }
//   else
//   {
//     pid_t childpid;
//     childpid = fork();
//     if(childpid == 0)
//     {
//       executeCommand(command);
//     }
//     else
//     {
//       waitpid(0,NULL);
//     }
//   }
// }

// void background(char command[])
// {
//   pid_t childpid;
//   childpid = fork();
//   if(childpid == 0)
//   {
//     executeCommand(command);
//   }

// }


// void executePipe(char command[])
// {
//   int fd[2];
//   int pid;
//   int count = 0;
//   pipe(fd);
//   char* first = command;
//   pid = fork();
//   while(command[count] != '\0')
//   {


//     if(command[count + 1] == '\0')
//     {
//       if(pid != 0)
//       {



//       waitpid(0,NULL);
//       dup2(fd[0],0);
//         close(fd[1]);
//       int index = 0;
//       while(first[index] == ' ')
//       {
//         index++;
//       }
//       first = first + index;
//       index = count;
//       while(first[index] == ' ')
//       {
//         index--;
//       }
//       first[index+1] = '\0';

//       // puts(first);
//       //   puts("Parent Process");

//         executeBinaries(first);
//         break;
//       }

//     }

//     if(command[count] == '|')
//     {

//       if(pid == 0)
//       {
//         dup2(fd[1],1);
//          close(fd[0]);
//         command[count] = '\0';
//       int index = 0;
//       while(first[index] == ' ')
//       {
//         index++;
//       }
//       first = first + index;
//       index = count-1;
//       while(first[index] == ' ')
//       {
//         index--;
//       }
//       first[index+1] = '\0';

//       // puts(first);
//       executeBinaries(first);
//       first = command + count +1;
//         // puts("Child Process");


//       }
//       first = command + count +1;

//     }
//     count++;
//   }

//   // int fd[2];
//   // int pid;
//   // char *ls_args[] = {"ls", "-al", NULL};
//   // char *grep_args[] = {"grep", "bin", NULL};
//   // pipe(fd);
//   // pid = fork();
//   // if(pid != 0)
//   // {
//   //   wait(NULL);
//   //   puts("Parent Process");
//   //   dup2(fd[0],0);
//   //   close(fd[1]);
//   //   execvp(grep_args[0],grep_args);
//   // }
//   // else
//   // {
//   //   puts("Child Process");
//   //   dup2(fd[1],1);
//   //   close(fd[0]);
//   //   execvp(ls_args[0],ls_args);
//   // }
// }


// int main(int argc, char* argv[]) {
// promptTextMessage = "sbush> ";
// setenv("PATH","./rootfs/bin/",1);

//   // printf("sbush> ");
//   char command[300];
//   while(1)
//   {
//     puts(promptTextMessage);
//     if (gets(command) == NULL)
//     {
//       puts("ERROR");
//     }
//     int flag =0;

//     int cnt = 0;
//     while(command[cnt] != '\0')
//     {
//       if(command[cnt] == '|')
//       {
//         flag++;
//       }
//       cnt++;
//     }
//     if(command[cnt-1] == '\n')
//     {
//       command[cnt-1] = '\0';
//     }

//     if(flag > 0)
//     {
//       executePipe(command);
//       continue;
//     }

//     if(command[cnt-2] == '&')
//     {
//       command[cnt-2] = '\0';

//       background(command);
//     }
//     else
//     {
//       executeShellCommand(command);
//     }
//   }
//   return 0;
// }