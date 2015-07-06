#include <stdio.h>
int main(int argc, char *argv[])
{
    //Write  permission  is  denied  for the directory containing oldpath or newpath
    mkdir("test-dir",0777);
    mkdir("test-dir/test-rename",0777);
    chmod("test-dir",0111);
    
    if(rename("test-dir/test-rename","test-dir/test-rename2")<0)
	printf("%s SUCCESS  \n",argv[0]);
    else
	printf("%s FAIL \n",argv[0]);
    chmod("test-dir",0777);
    rmdir("test-dir/test-rename");
    rmdir("test-dir");
    return 0;
}
