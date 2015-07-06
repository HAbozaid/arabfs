#include <stdio.h>
#include <fcntl.h>
int main()
{
	//rename a file is read-only 
	open("test-file",O_WRONLY|O_CREAT,0444);
	chmod("test-file",0444);
	if(rename("test-file","test-file2")<0)
		printf("test-rename SUCCESS  \n");
	else
		printf("test-rename Fail \n");
	return 0;
}