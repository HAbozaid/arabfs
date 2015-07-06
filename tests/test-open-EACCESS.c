#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	mkdir("test-dir",0111);
	int err=open("test-dir/test-open",O_WRONLY|O_CREAT,0777);
	if( err>= 0)
	    printf("%s .. %d[FAIL   ]\n",argv[0],err);
	else
	    printf("%s .. %d[SUCCESS]\n",argv[0],err);
	rmdir("test-dir");
	return 0;
}
