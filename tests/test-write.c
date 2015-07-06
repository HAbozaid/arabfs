#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
  int fd=open(test_dir"/test-write.file", O_WRONLY|O_CREAT|O_LARGEFILE, 0777);//  O_WRONLY the flag of 
  if(write(fd,"buf",3) >= 0)
      printf("%s ... [SUCCESS]\n",argv[0]);
  else
      printf("%s ... [FAIL   ]\n",argv[0]);
  return 0;
}
