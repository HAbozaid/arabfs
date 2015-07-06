#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
int main(int argc,char *argv[])
{
    open(test_dir"/test-symlink.file",  O_RDWR|O_CREAT|O_LARGEFILE, 0777);
    int result = symlink(test_dir"/test-symlink.file",
			 test_dir"/test-symlink-2.file");
    if(result == 0)
	printf("%s ... [SUCCESS]\n", argv[0]);
    else
	printf("%s ... %d %d [FAIL   ]\n", argv[0], result,errno);
    return 0;
}
