
// forkexec.c: fork then exec

#include <unistd.h>

int
main()
{
  int pid;

  pid = fork();
  if(pid == 0){
   char *argv[] = { "strecho", "hello2", 0 };
    execl("strecho", argv[0],argv[1],argv[2]);
    printf("exec failed!\n");
    exit(1);
  } else {
    printf("parent waiting\n");
    wait();
    printf("the child exited\n");
  }

  exit(0);
}
