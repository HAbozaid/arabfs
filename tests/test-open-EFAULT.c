#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if(open(test_dir"/non-existant-directory/test-open", O_RDWR|O_CREAT) < 0)
	    printf("%s [SUCCESS]\n" ,argv[0]);
	else
	    printf("%s [FAIL  ]\n",argv[0]);
	return 0;
}
