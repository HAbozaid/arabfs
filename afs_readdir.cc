#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <libgen.h>
#include <boost/algorithm/string.hpp>

int afs_readdir (const char *path, void *buf, fuse_fill_dir_t filler, off_t UNUSED offset, struct fuse_file_info UNUSED *fi)
{

  syslog (LOG_DEBUG, "readdir :%s\n",path);
	
  std::list<std::string> strs;
    boost::split(strs, path, boost::is_any_of("/")); 
    strs.erase(strs.begin());
    auto e = strs.end();
    int parent;
    parent=ROOT.find(path);
         syslog (LOG_DEBUG, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>readdir :%d\n",parent);
    if(!(hd.blocks[parent].get<file_t>().isDirectory()))
	return -ENOENT;
     syslog (LOG_DEBUG, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>readdir :%d\n",parent);
    boost::container::static_vector<uint32_t,94> file_inode=hd.blocks[hd.blocks[parent].get<file_t>().inode].get<inode_t>().index_file;
    filler (buf, ".", nullptr, 0);
    filler (buf, "..", nullptr, 0);
    
    // for(auto kv :file_inode)
    //  filler(buf,reinterpret_cast<char*>(hd.blocks[(*file_inode.begin()++)].file.name), nullptr, 0);	
    
    for(unsigned int i=0;i<file_inode.size();i++){
      filler(buf,hd.blocks[file_inode.at(i)].get<file_t>().name.c_str(), nullptr, 0);
      syslog (LOG_DEBUG, "readdir in for loop file_inode[%d]=%d  \n",i,file_inode.at(i));
    }
    /*for(unsigned int i=0,j=*file_inode.begin();i<file_inode.size();i++,j++){
      filler(buf,hd.blocks[j].get<file_t>().name.c_str(), nullptr, 0);
      syslog (LOG_DEBUG, "readdir in for loop file_inode[%d]=%d  \n",i,j);
      }*/
      return 0;
}

//for(auto& kv : *dir->children)
//	filler(buf, kv.first.c_str(), nullptr, 0);





/*int sfs_readdir (const char *path, void *buf, fuse_fill_dir_t filler, off_t UNUSED offset, struct fuse_file_info UNUSED *fi)
{

    syslog (LOG_DEBUG, "readdir \n");
	
    auto dir= ROOT->find(path);

    if(!(dir && dir->isDirectory()))
	return -ENOENT;
	   
    filler (buf, ".", nullptr, 0);
    filler (buf, "..", nullptr, 0);
    for(auto& kv : *dir->children)
	filler(buf, kv.first.c_str(), nullptr, 0);	
	
    return 0;
    }*/
