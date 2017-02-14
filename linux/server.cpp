#include<stdio.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/types.h>
#define SIZE 1024
int main(){
    int server_fd;
    int client_fd;
    int len;
    struct sockaddr_in my_addr;
    struct sockaddr_in remote_addr;
    socklen_t sin_size;
    char BUFFER[SIZE];
    memset(&my_addr,0,sizeof(my_addr));
    my_addr.sin_family=AF_INET;
    //my_addr.sin_addr.s_addr=INADDR_ANY;
    my_addr.sin_addr.s_addr=inet_addr("192.168.16.113");
    my_addr.sin_port=htons(8000);
    if((server_fd=socket(PF_INET,SOCK_STREAM,0))<0){
        perror("socket");
        return -1;
    }
    if(bind(server_fd,(struct sockaddr *)&my_addr,sizeof(struct sockaddr))<0)
    {
        perror("bind");
        return -1;
    }
    listen(server_fd,5);
    sin_size=sizeof(struct sockaddr_in);

    if((client_fd=accept(server_fd,(struct sockaddr *)&remote_addr,&sin_size))<0)
    {
        perror("accept");
        return -1;
    }
    len=send(client_fd,"Welcome to my server\n",21,0);
    while((len=recv(client_fd,BUFFER,SIZE,0))>0)
    {
        BUFFER[len] ='\0';
        printf("server :%s\n",BUFFER);
        if(send(client_fd,BUFFER,len,0)<0){
            return -1;
        }

    }
    close(client_fd);
    close(server_fd);
    return 0;
}
