#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <unistd.h>

int sfs_open (const char *path, struct fuse_file_info *f)
{
    syslog(LOG_DEBUG,"In OPEN \n");
    int file= ROOT.find(path);
    if(file==-1 )
	return -ENOENT;
    
    if(!(hd.blocks[hd.blocks[file].file.inode].inode.check_permission(getuid(),getgid()))
       && hd.blocks[file].file.type==file_type::D ) {
	return -EACCES;
    }
  
    if(! ( ( hd.blocks[file].file.mode & S_IRUSR) || ( hd.blocks[file].file.mode && S_IRGRP ) || ( hd.blocks[file].file.mode & S_IROTH) ) ) {
	return -EACCES;
    }
    f->fh =file;
    hd.blocks[hd.blocks[file].file.inode].inode.usecount++;
    syslog(LOG_DEBUG,"In OPEN END\n");
    return 0;
}
