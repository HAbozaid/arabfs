#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
//the unlink 2 
// test for remove files only not directory
    struct stat s;
    int fd=open(test_dir"/test-unlink-rm-file-only2", O_RDWR|O_CREAT|O_LARGEFILE, 0777);
    if( stat(test_dir"/test-unlink.file",&s) == 0 )
    {
	if( s.st_mode & S_IFDIR )
	{
	    printf("IS A DIRECTORY\n");
	    printf("%s ...[FAIL]\n",argv[0]);
	}
	else if( s.st_mode & S_IFREG )
	{
	    printf("IS A FILE\n");
	    printf("%s ...[SUCCESS]\n",argv[0]);
	}
    }
    else
    {
	printf("IS AN ERROR IN STAT\n");
    }

    return 0;
}
