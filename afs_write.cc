#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <iterator>
#include <cstring>
#include <boost/container/static_vector.hpp>
#include <algorithm>


int afs_write (const char UNUSED *path, const char  *buf, size_t  size, off_t  offset, struct fuse_file_info *f)
{
    std::string temp;
    if(!f)
			return -1;
 
    int file=f->fh;
    
    if(file==-1)
			return -ENOENT;
 
    {
	std::unique_lock<std::mutex> lk(hd.blocks[file].get<file_t>().mutex);
	hd.blocks[file].get<file_t>().cond.wait(lk,[&]{return ! (hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().readcount > 0 &&
								 hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().writecount > 0);}); 
    }
      hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().writecount++;
      hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().mtime = std::chrono::system_clock::now();
      
      std::list<unsigned int> free_index=hd.search(size);
    //the file is empty

      	//	hd.blocks[*(free_index.begin()++)].get<data_t>().data[0]='{';
      
      if (hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().index_file.empty())
      {
       	temp=buf;
	for(unsigned int i=0;i<free_index.size();i++)
	 {
	   hd.blocks[*free_index.begin()++].change_type<data_t>();
	   std::copy_n(temp.begin()+i*490,size,std::back_inserter(hd.blocks[*(free_index.begin()++)].get<data_t>().data));				}
      	hd.blocks[file].get<file_t>().add(free_index);
        hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().size=size;
      }
      
     else
      {
	std::vector<char>  data;

	hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().index_file.clear();
	data.insert(data.end(), buf, buf+size);
	std::list<unsigned int> free_index_2=hd.search(data.size());
	auto free_ind=free_index_2.begin();
	unsigned int free_index_size=free_index_2.size();
	for(unsigned int i=0;i<free_index_size;i++)
	  {
	    hd.blocks[*free_ind].change_type<data_t>();
	    std::copy(data.begin()+(i*496),data.begin()+((i+1)*496),std::back_inserter(hd.blocks[*(free_ind++)].get<data_t>().data));						}
	hd.blocks[file].get<file_t>().add(free_index_2);
	hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().size=size;
			}

      
    	{
	  std::lock_guard<std::mutex> guard{hd.blocks[file].get<file_t>().mutex};
	  hd.blocks[hd.blocks[file].get<file_t>().inode].get<inode_t>().writecount--;
        }




    return size;
}
