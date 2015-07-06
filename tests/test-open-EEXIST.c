#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	open("test-open",O_WRONLY|O_CREAT,0777); 
	int err=open("test-open",O_WRONLY|O_CREAT|O_EXCL);
	if(err < 0)
	    printf("%s .. %d[SUCCESS]\n",argv[0], err);
	else
	    printf("%s ,, %d [FAIL  ]\n" ,argv[0], err);
	remove("test-open");
	return 0;
}
