#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define O_RDONLY 0x0000

//void _start(int argc, char* argv[], char *envp[]) {
  // call main() and exit() here
//  int status;

//  status = main(argc, argv, envp);

//  exit(status);

//}

int main(int argc, char* argv[], char *envp[])
{
  char* token;
  token  = "/home/ankrastogi/workdir/test.sh";
  
  if(argc>=1){
    puts("Hello World");
    puts(argv[1]);
    token = argv[1];
  }

  char s[255];
  int file = open(token, O_RDONLY);
 if(read(file, s, 255) != 0)
   {
    int count = 0;
    while(s[count] != '\0')
    {
      char* first = s+count;
      while((s[count] != '\0') && (s[count] != '\n'))
      {
          count++;
      }
      if(s[count] == '\0')
      {
          puts(first);
          break;
      }
      else
      {
        s[count] = '\0';
        puts(first);
        count++;
      }
    }
   }
}
