#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<unistd.h>

#define SIZE 1024

int main()
{
    int client_fd;
    int len;
    struct sockaddr_in remote_addr;
    char buf[SIZE];
    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family=AF_INET;
    remote_addr.sin_addr.s_addr=inet_addr("192.168.16.113");
    remote_addr.sin_port=htons(8000);
    if((client_fd=socket(PF_INET,SOCK_STREAM,0))<0)
    {
        return -1;
    }
    if(connect(client_fd,(struct sockaddr *)&remote_addr,sizeof(struct sockaddr))<0)
    {
        return -1;
    }
    len = recv(client_fd,buf,SIZE,0);
    buf[len]='/0';
    printf("%s",buf);
    while(1){
        printf("Enter string to send");
        scanf("%s",buf);
        if(!strcmp(buf,"quit")){
            break;
        }
        len=send(client_fd,buf,strlen(buf),0);
        len=recv(client_fd,buf,SIZE,0);
        buf[len]='/0';
        printf("receive:%s/n",buf);
    }
    close(client_fd);
    return 0;
}
