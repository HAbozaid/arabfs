#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    open(test_dir"/test-unlink.file", O_WRONLY|O_CREAT|O_LARGEFILE, 0777);
    if(unlink(test_dir"/test-unlink.file") >= 0)
	printf("%s ... [SUCCESS]\n",argv[0]);
    else
	printf("%s ... [FAIL   ]\n",argv[0]);
    return 0;
}
