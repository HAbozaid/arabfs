#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd=open(test_dir"/test-truncate.file", O_RDWR|O_CREAT|O_LARGEFILE, 0777);
    
    off_t s=100000;
    if(truncate(test_dir"/test-truncate.file",s) >= 0)
	printf("%s ...[SUCCESS]\n",argv[0]);
    else
	printf("%s ...[FAIL]\n",argv[0]);
    
    return 0;
}
