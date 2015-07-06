#include <stdio.h>
int main(int argc, char *argv[])
{
    //try to make a directory subdirectory of itself 
    mkdir("test-dir",0777);
    mkdir("test-dir/test-rename",0777);
    if(rename("test-dir/test-rename","test-dir")<0)
	printf("%s SUCCESS  \n",argv[0]);
    else
	printf("%s FAIL \n",argv[0]);
    rmdir("test-dir/test-rename");
    rmdir("test-dir");
    return 0;
}
