#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if(open(test_dir"/test-open", O_RDWR|O_CREAT) >= 0,0777)
		printf("%s  [SCCUESS]\n", argv[0] );
	else
		printf("%s [FAIL  ]\n", argv[0]);
	remove("test-open");
	return 0;
}
