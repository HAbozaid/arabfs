#include <stdio.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    //newpath is an existing directory, but oldpath is not a directory
    open("test-file",O_WRONLY|O_CREAT|O_LARGEFILE,0777);
    mkdir("test-dir",0777);
    if(rename("test-file","test-dir")<0)
	printf("%s SUCCESS  \n",argv[0]);
    else
	printf("%s FAIL \n",argv[0]);
    remove("test-file");
    rmdir("test-dir");
    return 0;
}
