#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<sys/time.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    int server_sockfd,client_socket;
    int server_len,client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds,testfds;

    server_sockfd = socket(AF_INET,SOCK_STREAM,0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = 9374;
    server_len = sizeof(server_address);

    bind(server_sockfd,(struct sockaddr *)&server_address,server_len);
    listen(server_sockfd,5);
    FD_ZERO(&readfds);
    FD_SET(server_sockfd,&readfds);
    while(1){
        char ch[50];
        int fd;
        int nread;
        testfds = readfds;
        printf("server waiting\n");
        result = select(FD_SETSIZE,&testfds,(fd_set *)0,(fd_set *)0,(struct timeval *)0);
        if (result<1) {
            perror("server5");
            exit(1);
        }
        for(fd = 0; fd < FD_SETSIZE; fd++)
        {
            if (FD_ISSET(fd,&testfds)) {
                if (fd == server_sockfd) {
                    client_len = sizeof(client_address);
                    client_socket = accept(server_sockfd,(struct sockaddr *)&client_address,(socklen_t *)&client_len);
                    FD_SET(client_socket,&readfds);
                    printf("添加进来一个客户\n");
                }
                else
                {
                    ioctl(fd,FIONREAD,&nread);
                    if (nread == 0) {
                        close(fd);
                        FD_CLR(fd,&readfds);
                        printf("一个客户离开\n");
                    }
                    else
                    {
                        read(fd,ch,sizeof(ch));
                        printf("有动静\n");
                        // dup2(fd,1);
                        write(fd,"ok",2);
                    }
                    
                }
                
            }
            
        }
        
        
    }
    
}