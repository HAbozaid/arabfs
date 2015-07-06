#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{ 
  if(mkdir(test_dir"/test-mkdir",0777) >= 0)
      printf("%s ... [SUCCESS]\n",argv[0]);
  else
      printf("%s ... [FAIL   ]\n",argv[0]);
  return 0;
}
