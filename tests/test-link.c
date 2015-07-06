#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    if( open(test_dir"/original.c", O_WRONLY|O_CREAT|O_LARGEFILE, 0777) >= 0 )
    {  
	int result= link(test_dir"/original.c",test_dir"/link_to_orginal"); 
	if(result==0)
	    printf("%s ...[SUCCESS]\n", argv[0]);
	else
	    printf("%s ...FAILD error number :%d\n",argv[0], result);  
	
    }
    else
	printf("test-link(file)...FAILD\n");

    return 0;
}
