#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


//this test 
//not delet the file has an a process like read

int main(int argc, char *argv[])
{
    int fd=open(test_dir"/test-unlink-1-notremovefile-opened.file", O_RDWR|O_CREAT|O_LARGEFILE, 0777);
    char buf[10]="";
    if(write(fd,"buf",3))
    {
	if(unlink("test-read.file")>=0)
	    printf("%s ..[FAIL]\n",argv[0]);
	else
	    printf("%s ..[SUCCESS]\n",argv[0]);
    }
    
  return 0;
}
