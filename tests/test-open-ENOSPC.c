#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if(open("test-open",O_WRONLY|O_CREAT,0777)>= 0)
		printf("%s Created [SUCCESS]\n" ,argv[0]);
	else
	    printf("%s Create File [FAIL  ]\n",argv[0]);
	remove("test-open");
	return 0;
}
