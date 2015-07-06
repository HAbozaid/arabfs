#include "file_t.hpp"
#include <fuse.h>
#include <unistd.h>
#include <syslog.h>

int sfs_chmod (const char *path, mode_t mode)
{
    int  index_file=ROOT.find(path);
    syslog(LOG_DEBUG,"In chmod \n");
    if(index_file==-1)
				return -EEXIST;

    if(!hd.blocks[hd.blocks[index_file].get<file_t>().inode].get<inode_t>().check_permission(getuid(), getgid()))
				return -EACCES;
    hd.blocks[index_file].get<file_t>().mode=mode;
    return 0;
}
