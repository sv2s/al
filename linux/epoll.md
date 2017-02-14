## epoll
### API
    * int epoll_create1(int flags);
    * int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
    * int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);

### bionic/libc/include/sys/epoll.h
  ```
  typedef union epoll_data {
  void* ptr;
  int fd;
  uint32_t u32;
  uint64_t u64;
} epoll_data_t;

struct epoll_event {
  uint32_t events;
  epoll_data_t data;
}
  ```
