#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <libgen.h>
#include <algorithm>

int afs_rmdir(const char *path)
{
    syslog(LOG_DEBUG, "In RMDIR\n");
    char *pathd = strdup (path);
    char *dir_path = dirname (pathd);

    int dir= ROOT.find(path);
    int parent=ROOT.find(dir_path);

    assert(hd.blocks[dir].get<file_t>().type == file_type::D);
    
    if(dir== -1)
      return -ENOENT;

    
    else
      {
      hd.blocks[hd.blocks[dir].get<file_t>().inode].get<inode_t>().index_file.erase(hd.blocks[hd.blocks[dir].get<file_t>().inode].get<inode_t>().index_file.begin(),hd.blocks[hd.blocks[dir].get<file_t>().inode].get<inode_t>().index_file.end());
      
      
      hd.blocks[hd.blocks[parent].get<file_t>().inode].get<inode_t>().index_file.erase(std::find( hd.blocks[hd.blocks[parent].get<file_t>().inode].get<inode_t>().index_file.begin(), hd.blocks[hd.blocks[parent].get<file_t>().inode].get<inode_t>().index_file.end(),dir));
      //hd.blocks[dir].tag=block_type::free;
      
      // hd.blocks[hd.blocks[dir].get<file_t>().inode].get<inode_t>].tag=block_type::free;
      syslog(LOG_DEBUG,"END OF UNLINK\n");
      }
    
    return 0;

}
