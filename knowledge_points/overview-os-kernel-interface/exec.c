// exec.c: replace a process with an executable file
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int
main()
{
  char *argv[] = { "strecho", "hello", 0 };

  execl("strecho", argv[0],argv[1],argv[2]);

  printf("exec failed!\n");

  exit(0);
}
