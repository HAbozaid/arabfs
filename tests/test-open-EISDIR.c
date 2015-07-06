#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	mkdir("test-dir",0777);
	if(open("test-dir",O_WRONLY|O_CREAT,0777)<0)
	    printf("%s [SUCCESS]\n" ,argv[0]);
	else
	    printf("%s [FAIL  ]\n",argv[0]);
	rmdir("test-dir");
	return 0;
}
