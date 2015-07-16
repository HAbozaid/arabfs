#include "file_t.hpp"
#include <fuse.h>
#include <syslog.h>
#include <boost/algorithm/string.hpp>
#include <boost/container/static_vector.hpp>

int afs_getattr (const char *path, struct stat *stbuf)
{
    syslog(LOG_DEBUG,"getattr path=%s\n",path);
    int file_inode= hd.blocks[0].get<file_t>().find(path);
    syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>getattr file_inode=%d\n",file_inode);
       syslog(LOG_DEBUG,"In getattr In IF1\n");
    if(file_inode==-1)
    return -ENOENT;
       syslog(LOG_DEBUG,"In getattr In IF2\n");
    
    std::list<std::string> strs;
    boost::split(strs, path, boost::is_any_of("/")); 
    //syslog(LOG_DEBUG,"getattr the name of the file is :%s\n",hd.blocks[file_inode].get<file_t>().name);
    memset(stbuf,0,sizeof(*stbuf));
    stbuf->st_mode = (!(hd.blocks[file_inode].get<file_t>().type==file_type::D)?S_IFREG:S_IFDIR) | hd.blocks[file_inode].get<file_t>().mode;
    
    if(hd.blocks[file_inode].get<file_t>().isSymlink())
    	stbuf->st_mode |= S_IFLNK;   
    stbuf->st_nlink = 1;
    stbuf->st_size = hd.blocks[hd.blocks[file_inode].get<file_t>().inode].get<inode_t>().size;
    stbuf->st_ctime = std::chrono::system_clock::to_time_t(hd.blocks[hd.blocks[file_inode].get<file_t>().inode].get<inode_t>().ctime);
    stbuf->st_atime = std::chrono::system_clock::to_time_t(hd.blocks[hd.blocks[file_inode].get<file_t>().inode].get<inode_t>().atime);
    stbuf->st_mtime = std::chrono::system_clock::to_time_t(hd.blocks[hd.blocks[file_inode].get<file_t>().inode].get<inode_t>().mtime);
    stbuf->st_uid =  hd.blocks[hd.blocks[file_inode].get<file_t>().inode].get<inode_t>().uid;
    stbuf->st_gid =  hd.blocks[hd.blocks[file_inode].get<file_t>().inode].get<inode_t>().gid;
    syslog(LOG_DEBUG,"In getattr In IF\n");
    return 0;
}





// int sfs_getattr (const char *path, struct stat *stbuf)
// {
//     syslog(LOG_DEBUG,"getattr path=%s\n",path);
    
//     auto p = ROOT->find(path);
//     if(!p)
// 	return -ENOENT;
	
//     memset(stbuf,0,sizeof(*stbuf));
//     stbuf->st_mode = ((!p->isDirectory())?S_IFREG:S_IFDIR) | p->mode;
  
//     if(p->isSymlink())
// 	stbuf->st_mode |= S_IFLNK;
//     stbuf->st_nlink = 1;
//     stbuf->st_size = p->size;
//     stbuf->st_ctime = std::chrono::system_clock::to_time_t( p->ctime);
//     stbuf->st_atime = std::chrono::system_clock::to_time_t( p->atime);
//     stbuf->st_mtime = std::chrono::system_clock::to_time_t( p->mtime);
//     stbuf->st_uid = p->uid;
//     stbuf->st_gid = p->gid;
//     syslog(LOG_DEBUG,"In getattr In IF\n");
//     return 0;	
// }
