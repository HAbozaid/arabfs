#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
  if(open(test_dir"/test-create.file", O_WRONLY|O_CREAT|O_LARGEFILE, 0666) >= 0)
      printf("%s ... [SUCCESS]\n",argv[0]);
  else
      printf("%s ... [FAIL   ]\n",argv[0]);
  return 0;
}
