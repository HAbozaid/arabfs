#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>

//this test for sure that the file is open for write only 

int main(int argc, char *argv[])
{

    struct stat s;
    int fd=open(test_dir"/test-truncate-1.file",O_WRONLY|O_CREAT|O_LARGEFILE, 0777);
    off_t size=100000;
    stat(test_dir"/test-truncate-1.file",&s);
    if(truncate(test_dir"/test-truncate-1.file",size)==0 && !(s.st_mode & O_RDONLY) && !(s.st_mode &O_RDWR))// !(read(fd,buf,4)))
	printf("%s ...[SUCCESS]\n",argv[0]);
    else
	printf("%s ...[FAIL]\n",argv[0]);
  
    return 0;
}


