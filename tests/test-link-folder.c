#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if(mkdir(test_dir"/test-link",0777) >= 0)
    {  
    	int result= link(test_dir"/test-link",test_dir"/link_to_folder"); 
   
	if(result==0)
	    printf("test-link(folder)...FAILD1 error number :%d\n",result); 
	else
	    printf("test-link(folder)...[SUCCESS]\n"); 	
    }
    else
	printf("test-link(folder)...[FAILD]\n"); 		
    return 0;
}
