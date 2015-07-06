#include "file_t.hpp"
#include <fuse.h>
#include <iostream>
#include <syslog.h>
#include <boost/algorithm/string.hpp>
using std::cout;
using std::endl;

HDD hd{}; // http://stackoverflow.com/a/23005015
file_t& ROOT =hd.blocks[0].change_type<file_t>("/",0777,file_type::D,3,false);


extern int sfs_getattr	(char const *path	, struct stat *stbuf);
extern int sfs_create	(char const *path	, mode_t mode		, struct fuse_file_info *f);
extern int sfs_readdir	(char const *path	, void *buf		, fuse_fill_dir_t filler	, off_t UNUSED offset	, struct fuse_file_info UNUSED *fi);
extern int sfs_mkdir	(char const *path	, mode_t mode);
extern int sfs_rmdir(const char *path);

extern int sfs_chown	(char const *path	, uid_t uid		, gid_t gid);
extern int sfs_chmod	(char const *path	, mode_t mode);
extern int sfs_open	(char const *path	, struct fuse_file_info *f);
extern int sfs_read	(char const *path	, char *buf		, size_t size			, off_t offset		, struct fuse_file_info *f);
extern int sfs_write	(char const *path	, char const *buf	, size_t size			, off_t offset		, struct fuse_file_info *f);
extern int sfs_release	(char const *path	, struct fuse_file_info *f);
extern int sfs_rename	(char const *path	, char const *newpath);
extern int sfs_link	(char const *oldpath	, char const *newpath);
extern int sfs_readlink (char const *path	, char *buf		, size_t size);
extern int sfs_symlink	(char const *target	, char const *linkpath);
extern int sfs_unlink	(char const *path);
extern int sfs_truncate (char const *path	, off_t size);


int main (int argc, char *argv[])
{
    struct fuse_operations functions;
    functions.getattr	= sfs_getattr;
    functions.readlink	= sfs_readlink;
    functions.getdir = 0;
    functions.mknod = 0;
    functions.mkdir	= sfs_mkdir;
    functions.unlink	= sfs_unlink;
    functions.rmdir =sfs_rmdir;
    functions.symlink	= sfs_symlink;
    functions.rename	= sfs_rename;
    functions.link	= sfs_link;
    functions.chmod	= sfs_chmod;
    functions.chown	= sfs_chown;
    functions.truncate	= sfs_truncate;
    functions.utime = 0;
    functions.open	= sfs_open;
    functions.read	= sfs_read;
    functions.write	= sfs_write;
    functions.statfs = 0;
    functions.flush = 0;
    functions.release	= sfs_release;
    functions.fsync = 0;
    functions.setxattr = 0;
    functions.getxattr = 0;
    functions.listxattr = 0;
    functions.removexattr = 0;
    functions.opendir = 0;
    functions.readdir	= sfs_readdir;
    functions.releasedir = 0;
    functions.fsyncdir = 0;
    functions.init = 0;
    functions.destroy = 0;
    functions.access = 0;
    functions.create	= sfs_create;
    functions.ftruncate = 0;
    functions.fgetattr = 0;
    functions.lock = 0;
    functions.utimens = 0;
    functions.bmap = 0;
    functions.flag_nullpath_ok = 0;
    functions.flag_nopath = 0;
    functions.flag_utime_omit_ok = 0;
    functions.flag_reserved = 0;
    functions.ioctl = 0;
    functions.poll = 0;
    functions.write_buf = 0;
    functions.read_buf = 0;
    functions.flock = 0;
    functions.fallocate = 0;
           
    return fuse_main (argc, argv, &functions, nullptr);
}
