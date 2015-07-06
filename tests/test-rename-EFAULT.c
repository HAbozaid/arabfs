#include <stdio.h>
int main(int argc, char *argv[])
{
    //can't access this path
    mkdir("test-dir",0777);
    if(rename("test-dir/test-rename","test-dir/test-rename2")<0)
	printf("%s SUCCESS  \n",argv[0]);
    else
	printf("%s FAIL \n",argv[0]);
    rmdir("test-dir");
    return 0;
}
