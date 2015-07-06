#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <boost/algorithm/string.hpp>
#include <boost/container/static_vector.hpp>
#include <libgen.h>

int sfs_symlink(char const *target,char const *linkpath)
{
    syslog(LOG_DEBUG,"%s:%d target = %s, linkpath = %s\n",__FUNCTION__,__LINE__,target,linkpath);

    if(ROOT.find(linkpath)>0)
			return EEXIST;

 		std::list<unsigned int> symlink_index = hd.search(sizeof(block_t)*3);

  	hd.blocks[*(symlink_index.begin())].change_type<file_t>(basename(strdup(linkpath)), 0777,file_type::F, *(++symlink_index.begin()), true);
		hd.blocks[*(++symlink_index.begin())].change_type<inode_t>();
		hd.blocks[symlink_index.back()].change_type<data_t>();
		hd.blocks[symlink_index.front()].get<file_t>().add(symlink_index.back());

		std::copy(target, target+strlen(target), std::back_inserter(hd.blocks[symlink_index.back()].get<data_t>().data));
    
   	int parent = ROOT.find(dirname(strdup(linkpath)));

    if(parent==-1)
    	return -1;

    hd.blocks[parent].get<file_t>().add(symlink_index.front());
    
    return 0;
}
