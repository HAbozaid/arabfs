#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    creat(test_dir "/test-old-path",0777);
	
    if( open(test_dir"/test-newpath-exist", O_WRONLY|O_CREAT|O_LARGEFILE, 0777) >= 0 )
    {  
	int result= link(test_dir"/test-old-path",test_dir"/test-newpath-exist"); 
      
	if(result==0)
	    printf("%s ...[FAILD]\n", argv[0]);
	else
	    printf("%s ...[SUCCESS]\n", argv[0]);  	
    }
    else
	printf("test-link(NEW PATH EXIST CASE)...[FAILD]\n");

    return 0;
}
