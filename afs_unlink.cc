#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <libgen.h>

int afs_unlink (const char *path)
{
    syslog(LOG_DEBUG, "In Unlink\n");
    char *pathd = strdup (path);
    char *dir = dirname (pathd);

    	int parent_node_number = ROOT.find(dir);
	auto& parent = hd.blocks[parent_node_number].file;
 	auto& parent_inode = hd.blocks[parent.inode].inode;

	int current_node_number = ROOT.find(path);
	auto& current = hd.blocks[current_node_number].file;
	auto& current_inode = hd.blocks[current.inode].inode;

	
    if(current_node_number == -1)
	return -ENOENT;

    assert(current.type == file_type::F);

	for(unsigned int i = 0; i < current_inode.index_file.size() ; i++ )
	{
		if(hd.blocks[current.inode].inode.link == 1 ){
		  hd.blocks[current_inode.index_file[i]].tag = block_type::free;
			  hd.blocks[current_inode.index_file[i]].data.data.clear();
			(hd.blocks[current.inode].inode).index_file.erase((hd.blocks[current.inode].inode).index_file.begin() + i);
		}		
	} 

	hd.blocks[current_node_number].tag = block_type::free;
	for(unsigned int i = 0 ; i < hd.blocks[parent.inode].inode.index_file.size() ; i++)	
	{
		if(hd.blocks[parent.inode].inode.index_file[i] == static_cast<uint32_t>(current_node_number))
			hd.blocks[parent.inode].inode.index_file.erase(hd.blocks[parent.inode].inode.index_file.begin() + i);
	}
    
	
//parent->children->erase(current->name);
    //current.reset();
    syslog(LOG_DEBUG,"END OF UNLINK\n");
    return 0;

}
