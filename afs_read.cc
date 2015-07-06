#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <string.h>
#include <cstring>
#include <iterator>
#include <vector>
#include <boost/container/static_vector.hpp>


int sfs_read (const char UNUSED *path, char *buf, size_t size, off_t offset, struct fuse_file_info *f)
{
   int index=f->fh;
   // file_t file;
    //inode_t inode=hd.blocks[hd.blocks[index].file.inode].inode;
   syslog(LOG_DEBUG,"In READ the index file number1:%d \n",index);
   syslog(LOG_DEBUG,"In read the size of index_files: %d\n",hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file.size());

   // syslog(LOG_DEBUG,"IN READ the content of the block:%d",hd.blocks[hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file[0//]].get<data_t>().data.empty());

   /*syslog(LOG_DEBUG,"IN READ the content of the block:%s",hd.blocks[hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file[0]].get<data_t>().data);*/
   
   syslog(LOG_DEBUG,"In READ number of blocks are %d\n",hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file.size());
   std::vector<char>  data;
    for(unsigned int i =0;
	i < hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file.size();
       i++)
      {
      for(unsigned int j = 0;
	  j<hd.blocks[hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file[i]].get<data_t>().data.size();
	  j++){
	// syslog(LOG_DEBUG,"In READ 1 the content of buffer:%c\n",hd.blocks[hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file[i]].get<data_t>().data[j]);
	data.push_back( hd.blocks[hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().index_file[i]].get<data_t>().data[j]);
      }
    }
    syslog(LOG_DEBUG,"In read fh is %p\n",reinterpret_cast<void*>(f->fh));
    // syslog(LOG_DEBUG,"In READ FILE NAME IS %s\n",file->name.c_str());
    // syslog(LOG_DEBUG,"In READ 1 =%d\n",file->usecount);
    
    if(index==-1)
	return -ENOENT;
    
  
    syslog(LOG_DEBUG,"In READ 1 \n");
    
    
    {
      std::unique_lock<std::mutex> lk(hd.blocks[index].get<file_t>().mutex);

	syslog(LOG_DEBUG,"In READ  before wait\n");
	ROOT.cond.wait(lk,[&]{return !( ( hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().writecount )> 0 );});
    hd.blocks[hd.blocks[index].file.inode].inode.readcount++;
    }
    syslog(LOG_DEBUG,"In READ after wait\n");
     hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().atime=std::chrono::system_clock::now();
   syslog(LOG_DEBUG,"In READ after wait in copy fun\n");



   if (offset > data.size())
     size = 0;
   else if(offset+size > data.size()) 
     size=data.size()-offset;
   
   //std::strncpy(buf,(data),size);
   if(size>0)
   std::copy(data.begin()+offset,data.end(),buf);
   
    syslog(LOG_DEBUG,"In READ after wait in copy fun\n");
    syslog(LOG_DEBUG,"In READ the content of the buf*:%s\n",buf);
    
    std::lock_guard<std::mutex> guard{hd.blocks[index].file.mutex};
    hd.blocks[hd.blocks[index].get<file_t>().inode].get<inode_t>().readcount--;
    syslog(LOG_DEBUG,"In READ end:%d\n",size);
    syslog(LOG_DEBUG,"In READ end\n");
    
    return size;
}
