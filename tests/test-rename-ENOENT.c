#include <stdio.h>
int main(int argc, char *argv[])
{
    //oldpath not found or empty string
	
    if(rename(test_dir"/test-rename",test_dir"/test-rename2")<0||rename("",test_dir"/test-rename2")<0||rename(test_dir"/test-rename","")<0)
	printf("%s SUCCESS  \n",argv[0]);
    else
	printf("%s FAIL \n",argv[0]);
    return 0;
}
