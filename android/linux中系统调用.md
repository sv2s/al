### kernel-3.10/include/uapi/linux/unistd.h
```
/*
 * Include machine specific syscall numbers
 */
#include <asm/unistd.h>
```

### kernel-3.10/include/uapi/asm-generic/unistd.h 中增加系统调用的number
```
/*
 * This file contains the system call numbers, based on the
 * layout of the x86-64 architecture, which embeds the
 * pointer to the syscall in the table.
 *
 * As a basic principle, no duplication of functionality
 * should be added, e.g. we don't use lseek when llseek
 * is present. New architectures should use this file
 * and implement the less feature-full calls in user space.
 */

#ifndef __SYSCALL
#define __SYSCALL(x, y)
#endif
```
如pipe:
```
/* fs/pipe.c */
#define __NR_pipe2 59
__SYSCALL(__NR_pipe2, sys_pipe2)

#ifdef __ARCH_WANT_SYSCALL_NO_FLAGS
#define __NR_pipe 1040
__SYSCALL(__NR_pipe, sys_pipe)
```

### 在相关的文件中实现，使用宏SYSCALL_DEFINEx(ｘ为参数个数)
kernel-3.10/include/linux/syscalls.h　中SYSCALL_DEFINE定义：
```
#define SYSCALL_DEFINE0(sname)					\
	SYSCALL_METADATA(_##sname, 0);				\
	asmlinkage long sys_##sname(void)

#define SYSCALL_DEFINE1(name, ...) SYSCALL_DEFINEx(1, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE2(name, ...) SYSCALL_DEFINEx(2, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE3(name, ...) SYSCALL_DEFINEx(3, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE4(name, ...) SYSCALL_DEFINEx(4, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE5(name, ...) SYSCALL_DEFINEx(5, _##name, __VA_ARGS__)
#define SYSCALL_DEFINE6(name, ...) SYSCALL_DEFINEx(6, _##name, __VA_ARGS__)

```
### kernel-3.10/fs/pipe.c 中调用宏
```
SYSCALL_DEFINE2(pipe2, int __user *, fildes, int, flags)
{
	struct file *files[2];
	int fd[2];
	int error;

	error = __do_pipe_flags(fd, files, flags);
	if (!error) {
		if (unlikely(copy_to_user(fildes, fd, sizeof(fd)))) {
			fput(files[0]);
			fput(files[1]);
			put_unused_fd(fd[0]);
			put_unused_fd(fd[1]);
			error = -EFAULT;
		} else {
			fd_install(fd[0], files[0]);
			fd_install(fd[1], files[1]);
		}
	}
	return error;
}

SYSCALL_DEFINE1(pipe, int __user *, fildes)
{
	return sys_pipe2(fildes, 0);
}
```
### 所有的系统调用要在这里(/kernel-3.10/include/linux/syscalls.h)声明，比如pipe函数
```
asmlinkage long sys_pipe(int __user *fildes);
asmlinkage long sys_pipe2(int __user *fildes, int flags);
```
### kernel-3.10/arch/arm/kernel/calls.S 中定义：
```
CALL(sys_pipe)
CALL(sys_pipe2)
```

### 内核：
     1.在内核源码中实现系统调用函数
         可以参考fs/open.c中的open函数
          SYSCALL_DEFINE3(open, const char __user *, filename, int, flags, int, mode) ......

     2.定义函数调用号
     在arch/arm/include/asm/unistd.h中增加自己的函数调用编号

     3.声明新增的系统调用函数
     在include/linux/syscall.h中声明自己干刚定义的函数，如：
     asmlinkage long sys_open(const char __user *filename,
                    int flags, int mode);

     4.加入调用函数指针列表中
     在arch/arm/kernel/call.S最后面增加自己的函数
