### 数据结构
1. kernel-3.10/include/linux/pipe_fs_i.h
```
/**
 *	struct pipe_buffer - a linux kernel pipe buffer　
 *	@page: the page containing the data for the pipe buffer　//缓存数据
 *	@offset: offset of data inside the @page　　　　　　　　　　//偏移量
 *	@len: length of data inside the @page　　　　　　　　　　　　//数据长度
 *	@ops: operations associated with this buffer. See @pipe_buf_operations.
 *	@flags: pipe buffer flags. See above.
 *	@private: private data owned by the ops.
 **/
 //管道缓存
struct pipe_buffer {
	struct page *page;
	unsigned int offset, len;
	const struct pipe_buf_operations *ops;
	unsigned int flags;
	unsigned long private;
};
//相关flag
#define PIPE_BUF_FLAG_LRU	0x01	/* page is on the LRU */
#define PIPE_BUF_FLAG_ATOMIC	0x02	/* was atomically mapped */
#define PIPE_BUF_FLAG_GIFT	0x04	/* page is a gift */
#define PIPE_BUF_FLAG_PACKET	0x08	/* read() as a packet */
```
```
/**
 *	struct pipe_inode_info - a linux kernel pipe
 *	@mutex: mutex protecting the whole thing
 *	@wait: reader/writer wait point in case of empty/full pipe 空的pipe读等待，满的pipe写等待
 *	@nrbufs: the number of non-empty pipe buffers in this pipe
 *	@buffers: total number of buffers (should be a power of 2) 应该是2的幂
 *	@curbuf: the current pipe buffer entry
 *	@tmp_page: cached released page
 *	@readers: number of current readers of this pipe
 *	@writers: number of current writers of this pipe
 *	@files: number of struct file refering this pipe (protected by ->i_lock)
 *	@waiting_writers: number of writers blocked waiting for room
 *	@r_counter: reader counter
 *	@w_counter: writer counter
 *	@fasync_readers: reader side fasync
 *	@fasync_writers: writer side fasync
 *	@bufs: the circular array of pipe buffers
 **/
struct pipe_inode_info {
	struct mutex mutex;
	wait_queue_head_t wait;
	unsigned int nrbufs, curbuf, buffers;
	unsigned int readers;
	unsigned int writers;
	unsigned int files;
	unsigned int waiting_writers;
	unsigned int r_counter;
	unsigned int w_counter;
	struct page *tmp_page;
	struct fasync_struct *fasync_readers;
	struct fasync_struct *fasync_writers;
	struct pipe_buffer *bufs;
};

struct mutex {
	/* 1: unlocked, 0: locked, negative: locked, possible waiters */
	atomic_t		count;
	spinlock_t		wait_lock;
	struct list_head	wait_list;
```
```
/*
 * Note on the nesting of these functions:
 *
 * ->confirm()
 *	->steal()
 *	...
 *	->map()
 *	...
 *	->unmap()
 *
 * That is, ->map() must be called on a confirmed buffer,
 * same goes for ->steal(). 即->map()必须在确认缓冲区,也一样->steal()See below for the meaning of each
 * operation. Also see kerneldoc in fs/pipe.c for the pipe
 * and generic variants of these hooks.
 */
struct pipe_buf_operations {
	/*
	 * This is set to 1, if the generic pipe read/write may coalesce
	 * data into an existing buffer. If this is set to 0, a new pipe
	 * page segment is always used for new data.
   这是设置为1,如果通用管道读/写可能合并到现有数据缓冲区。如果设置为0,一个新的管道总是用于新数据。

	 */
	int can_merge;

	/*
	 * ->map() returns a virtual address mapping of the pipe buffer.
	 * The last integer flag reflects whether this should be an atomic
	 * mapping or not. The atomic map is faster, however you can't take
	 * page faults before calling ->unmap() again. So if you need to eg
	 * access user data through copy_to/from_user(), then you must get
	 * a non-atomic map. ->map() uses the kmap_atomic slot for
	 * atomic maps, you have to be careful if mapping another page as
	 * source or destination for a copy.
	 * 建立管道缓存的映射
	 */
	void * (*map)(struct pipe_inode_info *, struct pipe_buffer *, int);

	/*
	 * Undoes ->map(), finishes the virtual mapping of the pipe buffer.
	 */
	void (*unmap)(struct pipe_inode_info *, struct pipe_buffer *, void *);

	/*
	 * ->confirm() verifies that the data in the pipe buffer is there
	 * and that the contents are good. If the pages in the pipe belong
	 * to a file system, we may need to wait for IO completion in this
	 * hook. Returns 0 for good, or a negative error value in case of
	 * error.
	 */
	int (*confirm)(struct pipe_inode_info *, struct pipe_buffer *);

	/*
	 * When the contents of this pipe buffer has been completely
	 * consumed by a reader, ->release() is called.
	 */
	void (*release)(struct pipe_inode_info *, struct pipe_buffer *);

	/*
	 * Attempt to take ownership of the pipe buffer and its contents.
	 * ->steal() returns 0 for success, in which case the contents
	 * of the pipe (the buf->page) is locked and now completely owned
	 * by the caller. The page may then be transferred to a different
	 * mapping, the most often used case is insertion into different
	 * file address space cache.
	 */
	int (*steal)(struct pipe_inode_info *, struct pipe_buffer *);

	/*
	 * Get a reference to the pipe buffer.
	 */
	void (*get)(struct pipe_inode_info *, struct pipe_buffer *);
};
```
kernel-3.10/include/linux/fs.h
```
/*
 * inode->i_mutex nesting subclasses for the lock validator:
 *
 * 0: the object of the current VFS operation
 * 1: parent
 * 2: child/target
 * 3: quota file
 *
 * The locking order between these classes is
 * parent -> child -> normal -> xattr -> quota
 */
enum inode_i_mutex_lock_class
{
	I_MUTEX_NORMAL,
	I_MUTEX_PARENT,
	I_MUTEX_CHILD,
	I_MUTEX_XATTR,
	I_MUTEX_QUOTA
};

```
