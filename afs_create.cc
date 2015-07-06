#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <boost/algorithm/string.hpp>


int sfs_create (char const *path, mode_t mode, struct fuse_file_info *f)
{
    syslog (LOG_DEBUG, "create %s \n",path);
    auto p = ROOT.find(path);
    if(p>=0)
	return -EEXIST;
     
    std::list<std::string> strs;
    boost::split(strs, path, boost::is_any_of("/")); 
    strs.erase(strs.begin());
    auto e = strs.end();
   
    int parent;
    if(strs.size()==1)
	parent=0;
    else
    parent=ROOT.find(strs.begin(), --e);

    if(!hd.blocks[parent].file.isDirectory()){ 
	syslog (LOG_DEBUG, "create 2 \n");
	return -ENOTDIR;
    }

    std::list<unsigned int> free_index=hd.search(2*sizeof(block_t));
    if(free_index.size() !=2)
      return -EEXIST;
    
    hd.blocks[free_index.front()].change_type<file_t>(*strs.rbegin(),mode,file_type::F,free_index.back(),false);
    hd.blocks[free_index.back()].change_type<inode_t>();

    try { 
	hd.blocks[parent].file.add(free_index.front());
	f->fh = free_index.front();
	hd.blocks[free_index.back()].inode.usecount++;	
    } catch (FileExists) {
	return -EEXIST;
    } catch (...) {
      //log : unknown exception
	return -1; // TODO
    }
    return 0;
    
}
