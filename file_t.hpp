#define FUSE_USE_VERSION 26
#define UNUSED __attribute__((__unused__))

#include <boost/container/static_vector.hpp>
#include <boost/optional.hpp>
#include <condition_variable>
#include <boost/mpl/set.hpp>
#include <type_traits>
#include <functional>
#include <cstring>
#include <cassert>
#include <memory>
#include <vector>
#include <chrono>
#include <array>
#include <cmath>
#include <list>
#include <string>
#include <array>
#include <map>
#include <utility>

constexpr uint64_t num_blocks = NUM_BLOCKS;

enum class block_type : uint8_t { free, data, inode, superblock,file};

struct superblock_t
{
    boost::container::static_vector<uint32_t,124> files;
    static constexpr block_type tag = block_type::superblock;

};

struct inode_t
{
    inode_t(inode_t const  &i);
    inode_t();
    boost::container::static_vector<uint32_t,94> index_file;
    static constexpr block_type tag = block_type::inode;
    size_t size;
    int usecount;
    std::chrono::time_point<std::chrono::system_clock> ctime, atime, mtime;
    uid_t uid;
    gid_t gid;    
    int readcount, writecount;
    std::condition_variable cond;
    int link;
    bool check_permission(uid_t uid, gid_t gid ) const;
     //what about map some thing to think 
    // using map_l = std::map< std::string, int >;
    // union: map_l index_map; // must use fixed-size map!
};

struct data_t
{
    //boost::container::static_vector<uint8_t,496> data;   //char not int 
    boost::container::static_vector<char,496> data;
    //std::array<char,496> data;
    static constexpr block_type tag = block_type::data;
};

// Exceptions
class FileExists{};
enum class file_type: uint8_t {F,D};

struct file_t 
{
    static constexpr block_type tag = block_type::file;
    int inode;
    file_type type;
    std::string name;
    mode_t mode;  
    bool is_symlink;
    std::mutex mutex;
    std::condition_variable cond; 
    explicit file_t(std::string  name , mode_t mode,file_type type,int inode,bool is_symlink);
   
    bool isSymlink() const;
    bool isRoot() const;
    
  int find(std::string const path) const; // AAAAAH return by value !
    int find(std::list<std::string>::const_iterator first, std::list<std::string>::const_iterator last) const;
   
    void add(std::list<uint32_t> index);
    void add(int index);
    bool isDirectory() const;
    ~file_t(){} // AAAAH non-virtual destructor ! fine, if the class is final (keyword?)
};



template< typename Type, std::size_t ExpectedSize, std::size_t ActualSize = 0 >
struct validate_size : std::true_type
{
    static_assert( ActualSize == ExpectedSize,
                   "actual size does not match expected size" );
};

template< typename Type, std::size_t ExpectedSize >  //partial specialization
struct validate_size< Type, ExpectedSize, 0 >
  : validate_size< Type, ExpectedSize, sizeof( Type ) >  //what type is to get its size and 
{};


//make specialization cuz if true call one and if flse call 2 
template<typename T, size_t s = sizeof(T::tag)> // TODO check that tag is static, constexpr, with type block_type
struct has_tag : std::true_type {
  static_assert(T::tag==block_type::file, "invalid tag");
  static_assert(T::tag==block_type::free, "invalid tag");
  static_assert(T::tag==block_type::inode, "invalid tag");
  static_assert(T::tag==block_type::superblock, "invalid tag");
}; 

template<typename T>
struct has_tag<T, 0> : std::false_type {
};

template<typename T>
bool has_tag_v = has_tag<T>::value;  //conatain the variable of the tag template true or false 

template<typename T, std::enable_if_t<has_tag_v<T>,void>* = true>  //??  maybe to ge tag of the block ---why *  
block_type tag_v = T::tag;

//specializaton to if has tag callone and if no tag call 2

template<typename T>
block_type tag_v<T,0> = block_type::free;


//if types has t then return true 
template<typename T, typename ...Types>
constexpr bool is_one_of_v = typename boost::mpl::has_key<boost::mpl::set<Types...>,T>::type ();



// I do not encode block_type in block_t template parameter to
// enabe type-erasure (so we can store all blocks in the same
// array).  I also cannot use class polymorphism (type-erasure or
// not) since thisis struct will be reloaded from disk, and class
// polymorphism uses pointers, and there is no guarantee that
// pointers remain valid when the block is loaded from disk.
// We need to use (extended) unions.
struct block_t {    
    /* static data member */
    static constexpr uint16_t block_size = BLOCK_SIZE;
    
    /* non-static data member */
    block_type tag;

    /* static data member */
    static constexpr uint16_t obj_size = block_size - sizeof(tag) - 8 /* why 8? */;

    /* non-static data member */
    // this is what is called a ``discriminated union''
    union {
	superblock_t superblock;
	inode_t inode;
	data_t data;
        file_t file;
	std::array<uint8_t,obj_size> pad;
    };

    // copy constructor
    block_t(block_t const &o) : block_t() {
	tag = o.tag;
	// no need to call the dtor since this is a ctor! no previous
	// instances exist.
	switch(tag)
	{
	case block_type::data:
	    new (&data) data_t(o.data);
	case block_type::file:
	   new (&file) file_t(o.file.name,o.file.mode,o.file.type,o.file.inode,o.file.is_symlink);
	  // new (&file) file_t(file.name,file.mode,file.type,file.inode,file.is_symlink);
	 // new (&file) file_t(o.file); 
 	    break;
	case block_type::inode:
	    new (&inode) inode_t(o.inode);
	    break;
	case block_type::superblock:
	    new (&superblock) superblock_t(o.superblock);
	    break;
	case block_type::free:
	    // do nothing.
	    break;
	}
    }
    
    ~block_t() {
	// FIXME: We should not call the destructor when the object is
	// removed from memory, since it might still live on disk! 
	switch(tag)
	{
	case block_type::data:
	    data.~data_t();
	case block_type::file:
	    file.~file_t();    
	    break;
	case block_type::inode:
	    inode.~inode_t();
	    break;
	case block_type::superblock:
	    superblock.~superblock_t();
	    break;
	case block_type::free:
	    // do nothing.
	    break;
	}
    }

    // default ctor
    block_t() : tag{block_type::free} {
	static_assert(validate_size< block_t, block_size >::value, "sizeof(block_t) != block_size");
    }

    template <typename T, typename ...Args>
    std::enable_if_t<
      is_one_of_v<T,file_t, superblock_t,inode_t,data_t>,
		     T&> change_type(Args ...args) {
        this->~block_t(); //why ????
	tag = tag_v<T>;
	new (&pad) T(std::forward<Args>(args)...);   //Returns an rvalue reference to arg if arg is not an lvalue reference	
	return get<T>();
    }
  //get<>:Returns a reference to the Ith element of tuple tpl.
  
 #define getter(Type, Tag, Const)			\
    template <typename T>				\
    std::enable_if_t<std::is_same<T,Type>::value,	\
		     T Const &> get() Const {		\
	assert(tag == tag_v<T>);			\
	return Tag;					\
    }

    getter(superblock_t	, superblock, const)
    getter(superblock_t	, superblock,      )
    getter(inode_t	, inode     , const)
    getter(inode_t	, inode     ,      )
    getter(file_t	, file     , const)
    getter(file_t	, file      ,      )
    getter(data_t	, data      , const)
    getter(data_t	, data      ,      )    
};
//bt return el tag if is valid
// ~/opt/gcc/bin/g++  afs_create.cc -c -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse  -std=c++14 -DBLOCK_SIZE=512 -DNUM_BLOCKS=100 -o afs_create.o |& less
struct HDD
{
public:
    std::array<block_t,num_blocks> blocks;
    
    superblock_t& superblock;
 
     
    HDD(): blocks{block_t{}}, superblock{blocks[1].change_type<superblock_t>()}
	{
	    // blocks[1].change_type<data_t, block_t::block_type::data>();
	   blocks[2].change_type<inode_t>();
	   blocks[3].change_type<inode_t>(); //inode of the root
	}
  std::list<unsigned int> search(size_t total_size);
};

extern HDD hd;
extern file_t& ROOT;

