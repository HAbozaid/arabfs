#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <libgen.h>


int afs_link (const char *oldpath, const char *newpath)
{
    int file=ROOT.find(oldpath);
    if(file==-1)
	return -ENOENT;
    std::list<unsigned int> free_index=hd.search(sizeof(block_t));
    if(free_index.size() !=1)
      return -EEXIST;
    if(ROOT.find(newpath)>=0)
	return EEXIST;
    char *newname=basename(strdup(newpath));
    hd.blocks[free_index.front()].change_type<file_t>(newname,hd.blocks[file].file.mode,file_type::F,hd.blocks[file].file.inode,false);
    hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().link++;
    int parent =ROOT.find(dirname(strdup(newpath)));
    hd.blocks[parent].get<file_t>().add(free_index.front());
    return 0;

    }
