#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>

int sfs_release (const char *path, struct fuse_file_info *f)
{
    syslog (LOG_DEBUG, "release %s\n", path);
    int result=-ENOENT;
    int file=f->fh;
    if(file){
      hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().usecount--;
	syslog (LOG_DEBUG, "release end%s\n", path);
	return 0;
    }
    return result;
}
