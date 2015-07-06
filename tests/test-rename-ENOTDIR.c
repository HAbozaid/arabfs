#include <stdio.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
	//rename dir to file is exist
    mkdir(test_dir"/test-rename",0777);
    open(test_dir"/test-file",O_WRONLY|O_CREAT|O_LARGEFILE,0777);
    if(rename(test_dir"/test-rename",test_dir"/test-file")<0)
	printf("%s SUCCESS  \n",argv[0]);
    else
	printf("%s FAIL \n",argv[0]);
    remove(test_dir"/test-file");
    rmdir(test_dir"/test-dir");
    return 0;
}
