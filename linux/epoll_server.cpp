#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<string>
#include<fcntl.h>
#define MAX_EVENTS 100
#define BUFFER_SIZE 1024
void setnonblocking(int sockfd)
{
    int f;
    if((f=fcntl(sockfd, F_GETFL, 0) == -1))
    {
        perror("fcntl");
        exit(-1);
    }
    if(fcntl(sockfd, F_SETFL,f | O_NONBLOCK)<0)
    {
        perror("fcntl:set");
        exit(-1);
    }
}
int main()
{
    struct epoll_event ev,events[MAX_EVENTS];
    struct sockaddr_in my_addr, remote_addr;
    int epollfd,listen_fd, conn_fd;
    int n,nfds, len;
    socklen_t len_t;
    char buf[BUFFER_SIZE];
    std::string Str;
    printf("wangyao");
    memset(&my_addr,0,sizeof(struct sockaddr_in));
    //printf("%s %s",argv[0],argv[1]);
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //my_addr.sin_port = htonl(atoi(argv[1]));
    my_addr.sin_port = htonl(8000);
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1)
    {
        perror("socket");
        exit(-1);
    }

    if( (bind(listen_fd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)))== -1 )
    {
        perror("bind");
        exit(-1);
    }
    if (listen(listen_fd, 100) == -1)
    {
        perror("listen");
        exit(-1);
    }
    epollfd = epoll_create(MAX_EVENTS);
    if (epollfd == -1)
    {
        perror("epoll_create1");
        exit(-1);
    }
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = listen_fd;
    if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_fd, &ev) == -1)
    {
        perror("epoll_crl");
        exit(-1);
    }
    for(;;)
    {
        if((nfds=epoll_wait(epollfd, events, MAX_EVENTS, -1)) == -1)
        {
            perror("epoll_wait");
            exit(-1);
        }
        for(n= 0;n < nfds; ++n)
        {
            if(events[n].data.fd == listen_fd)
            {
                len_t = sizeof(struct sockaddr );
                conn_fd = accept(listen_fd, (struct sockaddr *)&my_addr, &len_t);
                if(conn_fd == -1)
                {
                    perror("accept");
                    exit(-1);
                }
                setnonblocking(conn_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_fd;
                if(epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_fd, &ev) == -1)
                {
                    perror("epoll_ctl:conn_fd");
                    exit(-1);
                }
            }else if(events[n].events & EPOLLIN)
            {

               while((len = read(events[n].data.fd,buf,BUFFER_SIZE) > 0)){
                      Str += buf;
                       }
                printf("recieved: %s", Str.c_str());
            }else if(events[n].events & EPOLLOUT)
            {
                write(events[n].data.fd,"server",10);
            }
            }

    }
    return 0;
}
