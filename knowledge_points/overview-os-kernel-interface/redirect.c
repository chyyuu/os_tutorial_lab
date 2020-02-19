
// redirect.c: run a command with output redirected

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int
main()
{
  int pid;

  pid = fork();
  if(pid == 0){
    close(1);
    open("output.txt", O_WRONLY|O_CREAT);

    char *argv[] = { "strecho", "hello redirect", 0 };
    execl("strecho", argv[0],argv[1],argv[2]);
    printf("exec failed!\n");
    exit(1);
  } else {
    wait();
  }

  exit(0);
}
