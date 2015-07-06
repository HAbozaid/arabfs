#include "file_t.hpp"
#include <fuse.h>
#include <boost/algorithm/string.hpp>
#include <syslog.h>
#include <string>
#include <unistd.h>
#include <list>

// file_t::file_t(file_t const &f)
// {
//   name=f.name;
//   mode=f.mode;
//   type=f.type;
//   inode=f.inode;
//   is_symlink= f.is_symlink;
  
//   }

inode_t::inode_t(inode_t const &i)
  :size(i.size),
   usecount(i.usecount),
   ctime(i.ctime),
   atime(i.atime),
   mtime(i.mtime),
   gid(i.gid),
   readcount(i.readcount),
   writecount(i.writecount),link(i.link)
   {}
inode_t::inode_t()
  : size(0),
    usecount(0),
    ctime(std::chrono::system_clock::now()), atime(ctime),mtime(ctime),
    uid(getuid()), gid(getgid()),
    readcount(0), writecount(0), cond(),link(1)
{
}

file_t::file_t(std::string  name, mode_t mode,file_type type,int inode,bool is_symlink=false)
     :inode(inode),
      type(type),
      name(name),
      mode(mode), is_symlink(is_symlink),mutex()
{
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in CONSTRACTOR\n ");
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in find 2  before end %s:\n",name.c_str());
  // if(!isDirectory())
  //	contents = std::make_shared<std::vector<char>>();
	}

bool file_t::isSymlink() const {return is_symlink;}
/*bool file_t::isRoot() const { return !this->root; }
*/

// AAAAAAAAAAAAAAAAAAAAH return by value not reference !!!!!!!!!!!!!
int file_t::find(const std::string path)	
    const
{
  syslog(LOG_DEBUG,"IN FIND path=%s\n",path.c_str());
    assert(path.at(0) == '/');
    assert(!path.empty());
    if(path.size() == 1)
      {
       syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in if loop of find 1 ");
      return 0;//ROOT
      }
    //  else if(path.at(1)=='.')
    //  return -1;
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in loop of find 1 ");
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in  find 1 the original path is:%s ",path.c_str());
    std::list<std::string> strs;
    boost::split(strs, path, boost::is_any_of("/")); 
    strs.erase(strs.begin());
    syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for loop find 1 file name strs.begine():%s ",(*strs.begin()).c_str());
    return hd.blocks[0].get<file_t>().find(std::begin(strs), std::end(strs));////ROOT.find(std::begin(strs), std::end(strs));
 }

int  file_t::find(std::list<std::string>::const_iterator first, std::list<std::string>::const_iterator last)
    const
{
  assert(type==file_type::D);
  boost::container::static_vector<uint32_t,94> inode_files= hd.blocks[inode].get<inode_t>().index_file;
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for loop find 2 file inode :%d ",inode);
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for loop find 2 file name :%s ",name.c_str());
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for loop find 2 file name *frist:%s ",(*first).c_str());
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for THE SIZE OF INODE_FILES:%d",inode_files.size());

   if(inode_files.size()==0)
     return -1;
   
  unsigned int i;
  for(i=0;i<inode_files.size();i++){
     syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for loop of find 2 ");
    if(hd.blocks[inode_files[i]].file.name==*first)
      {
       syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for loop of inode_file:%d ",inode_files[i]);
       syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in for loop of find 2 ");
	if(std::distance(first,last)==1)
	  {
	     syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in find 2  if");
	  return inode_files.at(i);
	  break;
	   }
	return  hd.blocks[inode_files.at(i)].get<file_t>().find(++first, last);	
       }
    else if(i==inode_files.size()-1&&first==(--last))
      {
	 syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in find 2  else if");
	return -1;
	}
  }
  
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in find 2  before end");
   syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in *inode_files.begin()++:%d:",*inode_files.begin()++);
}


void file_t::add(std::list<uint32_t> index)
{
   for(unsigned int i=0;i<index.size();i++)
     hd.blocks[inode].get<inode_t>().index_file.push_back(*(index.begin()++));
}

void file_t::add(int index)
{
  hd.blocks[inode].get<inode_t>().index_file.push_back(index);
  syslog(LOG_DEBUG,">>>>>>>>>>>>>>>>>>>>>>>>>in ADD FILE :%d: ",*hd.blocks[inode].get<inode_t>().index_file.rbegin());
}

    /*void file_t::add(std::list<uint32_t> index)
   {
    for(unsigned int i=0;i<index.size();i++)
      hd.blocks[inode].inode.index_file.push_back(*(index.begin()++));
      }*/


bool file_t::isDirectory() const
{
  if(this->type==file_type::D)
     return true;
  return false;
}


bool inode_t::check_permission(uid_t uid, gid_t gid ) const {
    return uid==0 || ( (this->uid==uid ) && (this->gid==gid) );
}



  std::list<unsigned int> HDD::search(size_t total_size) //// AAAAAH signed size !!
{
  unsigned int blocks_num=std::ceil((float)total_size/BLOCK_SIZE); // AAAAAAAH integer division
  
    std::list<unsigned int> free_index;
    for(unsigned int i=0;i<blocks.size();i++)
    {
      if(blocks[i].tag==block_type::free)
	{
	    free_index.push_back(i);
	    if(free_index.size()==blocks_num)
	    {
		return free_index;
	    }
	}
    }
    return free_index;
 }






