#include "file_t.hpp"
#include <fuse.h>
#include <iostream>
#include <syslog.h>
#include <boost/algorithm/string.hpp>
using std::cout;
using std::endl;

HDD hd{}; // http://stackoverflow.com/a/23005015
file_t& ROOT =hd.blocks[0].change_type<file_t>("/",0777,file_type::D,3,false);


extern int afs_getattr	(char const *path	, struct stat *stbuf);
extern int afs_create	(char const *path	, mode_t mode		, struct fuse_file_info *f);
extern int afs_readdir	(char const *path	, void *buf		, fuse_fill_dir_t filler	, off_t UNUSED offset	, struct fuse_file_info UNUSED *fi);
extern int afs_mkdir	(char const *path	, mode_t mode);
extern int afs_rmdir(const char *path);

extern int afs_chown	(char const *path	, uid_t uid		, gid_t gid);
extern int afs_chmod	(char const *path	, mode_t mode);
extern int afs_open	(char const *path	, struct fuse_file_info *f);
extern int afs_read	(char const *path	, char *buf		, size_t size			, off_t offset		, struct fuse_file_info *f);
extern int afs_write	(char const *path	, char const *buf	, size_t size			, off_t offset		, struct fuse_file_info *f);
extern int afs_release	(char const *path	, struct fuse_file_info *f);
extern int afs_rename	(char const *path	, char const *newpath);
extern int afs_link	(char const *oldpath	, char const *newpath);
extern int afs_readlink (char const *path	, char *buf		, size_t size);
extern int afs_symlink	(char const *target	, char const *linkpath);
extern int afs_unlink	(char const *path);
extern int afs_truncate (char const *path	, off_t size);


int main (int argc, char *argv[])
{
    struct fuse_operations functions;
    functions.getattr	= afs_getattr;
    functions.readlink	= afs_readlink;
    functions.getdir = 0;
    functions.mknod = 0;
    functions.mkdir	= afs_mkdir;
    functions.unlink	= afs_unlink;
    functions.rmdir =afs_rmdir;
    functions.symlink	= afs_symlink;
    functions.rename	= afs_rename;
    functions.link	= afs_link;
    functions.chmod	= afs_chmod;
    functions.chown	= afs_chown;
    functions.truncate	= afs_truncate;
    functions.utime = 0;
    functions.open	= afs_open;
    functions.read	= afs_read;
    functions.write	= afs_write;
    functions.statfs = 0;
    functions.flush = 0;
    functions.release	= afs_release;
    functions.fsync = 0;
    functions.setxattr = 0;
    functions.getxattr = 0;
    functions.listxattr = 0;
    functions.removexattr = 0;
    functions.opendir = 0;
    functions.readdir	= afs_readdir;
    functions.releasedir = 0;
    functions.fsyncdir = 0;
    functions.init = 0;
    functions.destroy = 0;
    functions.access = 0;
    functions.create	= afs_create;
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
