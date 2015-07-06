#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char *argv[]){
    int dir=mkdir(test_dir"/test-readdir",0777);
    DIR *fd_proc;
    struct dirent *dirp;
    if(dir>=0){

	int dir1=mkdir(test_dir"/test-readdir/test-readdir",0777);
  	int fd=open(test_dir"/test-readdir/test-read.file", O_RDWR|O_CREAT|O_LARGEFILE, 0777);
	int dir_fd=opendir(test_dir"/test-readdir/test-read.file");
	if(dir_fd !=NULL){
         
	    if(readdir(dir_fd)!= NULL){
		printf("%s ...[FAILD]\n",argv[0]);
	
	    }
	    else
		printf("%s ...[SUCCESS]\n",argv[0]);
	
	}
	else
	{
	    printf("%s ...[SUCCESS]\n",argv[0]);
	}

    }else{
	printf("%s ...[FAILED3]\n",argv[0]);
    }
    return 0;
}
