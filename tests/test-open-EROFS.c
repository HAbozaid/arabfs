#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	open("test-open",O_RDONLY|O_CREAT,0444);
	if(open("test-open",O_WRONLY|O_LARGEFILE,0777)< 0)
	    printf("%s [SUCCESS]\n" ,argv[0]);
	else
	    printf("%s [FAIL  ]\n",argv[0]);
	return 0;
}
