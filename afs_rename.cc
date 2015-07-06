#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <libgen.h>
#include <iterator>
#include <boost/algorithm/string.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/detail/config_begin.hpp>
#include <boost/container/vector.hpp>
#include <boost/aligned_storage.hpp>

int sfs_rename (const char *path, const char *newpath)
{
    int i_file=ROOT.find(path);
    if(i_file==-1)
			return -ENOENT;
    int parent=ROOT.find(dirname(strdup(path)));

    int newfile=ROOT.find(newpath);
    if(newfile>=0)
			return EEXIST;
    int newparent=ROOT.find(dirname(strdup(newpath)));
    hd.blocks[i_file].get<file_t>().name=basename(strdup(newpath));
    hd.blocks[newparent].get<file_t>().add(i_file);
    //boost::container::static_vector<uint32_t,1> temp;
		auto tmp=std::find(hd.blocks[hd.blocks[parent].get<file_t>().inode].get<inode_t>().index_file.begin(),
											hd.blocks[hd.blocks[parent].get<file_t>().inode].get<inode_t>().index_file.end(),i_file);
    hd.blocks[hd.blocks[parent].get<file_t>().inode].get<inode_t>().index_file.erase(tmp,tmp+1);
  
    return 0;

}
