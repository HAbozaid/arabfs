#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <libgen.h>
#include <boost/algorithm/string.hpp>

int  sfs_mkdir(const char *path , mode_t mode)
{
    syslog(LOG_DEBUG, "In Mkdir\n");
    char *pathd = strdup (path);
    char *dir = dirname (pathd);
 syslog(LOG_DEBUG, "In Mkdir0\n");
    if(ROOT.find(path)>0)
    	return EEXIST;
    syslog(LOG_DEBUG, "In Mkdir1\n");	
    std::list<std::string> strs;
    boost::split(strs, path, boost::is_any_of("/")); 
    syslog(LOG_DEBUG, "In Mkdir2\n");
     syslog(LOG_DEBUG, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>In parent:%s:2\n",dir);
     int parent = ROOT.find(dir);
    syslog(LOG_DEBUG, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>In parent:%d:2\n",parent);
    if(parent<0)
   	return -ENOENT;

    std::list<unsigned int> free_index=hd.search(2*sizeof(block_t));
    if(free_index.size() !=2)
      return -EEXIST;
    syslog(LOG_DEBUG, "In Mkdir4  free_index:%d\n",free_index.front());
    syslog(LOG_DEBUG, "In Mkdir4  free_index back:%d\n",free_index.back());
    // std::string r="afs";
    //std::string s=(basename(strdup(path)))+r;
    syslog(LOG_DEBUG, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>in mkdir *strs.rbegin():%s:2\n",basename(strdup(path)));
    hd.blocks[free_index.front()].change_type<file_t>((basename(strdup(path))),mode,file_type::D,free_index.back(),false);
    //hd.blocks[free_index.front()].get<file_t>().name+"afs";
    syslog(LOG_DEBUG, "In Mkdir3 File Name int the block is %s\n",hd.blocks[free_index.front()].get<file_t>().name.c_str());
    syslog(LOG_DEBUG, "In Mkdir3 File Name is %s\n",(*strs.rbegin()).c_str());
    hd.blocks[free_index.front()].get<file_t>().inode=free_index.back();
    hd.blocks[free_index.back()].change_type<inode_t>();
    syslog(LOG_DEBUG, "In Mkdir4\n");
    hd.blocks[parent].get<file_t>().add(free_index.front());
    syslog(LOG_DEBUG, "In Mkdir5\n");
 
    return 0;
}






/*int  sfs_mkdir(const char *path , mode_t mode)
{
    syslog(LOG_DEBUG, "In Mkdir\n");

    if(ROOT.find(path))
	return -EEXIST;
	
    char *pathd = strdup (path);
    char *pathb = strdup (path);
    char *dir = dirname (pathd);
    char *base = basename (pathb);

    int parent = ROOT.find(dir);
	
    if(!parent)
	return -ENOENT;	
	
    // note to myself: use emplace_back with perfect forwarding here
    parent.add(std::make_shared<file_t>(boost::none, base, mode));

    // TODO: free pathd, pathb, dir, and base here.
	
    return 0;
    }*/
