#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>

int sfs_truncate (const char *path, off_t size)
{   
    std::lock_guard<std::mutex> guard{ROOT.mutex};
    int file_index=ROOT.find(path);
       

    if(file_index==-1) {
	syslog (LOG_DEBUG, "truncate -ENOENT: \"!file\"\n");
	return -ENOENT;
    }

    assert(!hd.blocks[file_index].file.isDirectory());

    if(hd.blocks[file_index].get<file_t>().type==file_type::D) {
	syslog (LOG_DEBUG, "truncate -ENOENT: \"!file->contents\"\n");
	return -ENOENT;
    }
      

    {
	std::unique_lock<std::mutex> lk(hd.blocks[file_index].file.mutex);
	bool condition=(hd.blocks[hd.blocks[file_index].file.inode].inode.readcount > 0
			 &&hd.blocks[hd.blocks[file_index].file.inode].inode.writecount == 0);
	ROOT.cond.wait(lk,[&]{return !condition;});
        hd.blocks[hd.blocks[file_index].file.inode].inode.writecount++;
    }
    
    
    if(static_cast<unsigned int>(size) != hd.blocks[hd.blocks[file_index].file.inode].inode.size) {
      hd.blocks[hd.blocks[file_index].file.inode].inode.size=size; /// wallah a3lam :D
     }


      hd.blocks[hd.blocks[file_index].file.inode].inode.mtime=std::chrono::system_clock::now();
    {
	std::lock_guard<std::mutex> guard{hd.blocks[file_index].file.mutex};
	hd.blocks[hd.blocks[file_index].file.inode].inode.writecount--;
    }
    
    return 0;
}
