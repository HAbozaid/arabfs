#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
    if(mkdir(test_dir"/test-rename",0777)==0){
	if(rename(test_dir"/test-rename", test_dir"/test-rename2"))
	    printf("%s ... [SUCCESS]\n",argv[0]);
    }
    else
	printf("%s ... [FAIL   ]\n",argv[0]);
    rmdir(test_dir"/test-rename2");
    return 0;
}
