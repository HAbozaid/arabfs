#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd=open(test_dir"/test-read.file", O_RDWR|O_CREAT|O_LARGEFILE, 0777);
    printf("fd=%x\n",fd);
    if(write(fd,"buf",3) < 0)	
	printf("%s ...[write-failed]\n",argv[0]);
    char buf[10]="";
    if(read(fd,buf,4)!=-1)
	printf("%s ...[SUCCESS]\n",argv[0]);
    else
	printf("%s ...[FAIL]\n",argv[0]);
    
    return 0;
}
