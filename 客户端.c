#include	<sys/types.h>	
#include	<sys/socket.h>	
#include <netinet/tcp.h>
#if TIME_WITH_SYS_TIME
#include	<sys/time.h>	
#include	<time.h>		
#else
#if HAVE_SYS_TIME_H
#include	<sys/time.h>	
#else
#include	<time.h>		
#endif
#endif
#include	<netinet/in.h>	
#include	<arpa/inet.h>	
#include	<errno.h>
#include	<fcntl.h>		
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>	
#include	<sys/uio.h>		
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>		
# include	<sys/select.h>

int main(int argc,char **argv){
    int sockfd,ned;
    int len;
    struct sockaddr_in address;
    int result;
    ssize_t n = 3;
    char ch[20],ck[4096];

    sockfd = socket(AF_INET,SOCK_STREAM,0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    // address.sin_addr.s_addr = htonl(argv[1]);
    address.sin_port = 9374;
    len = sizeof(address);
    

    result = connect(sockfd,(struct sockaddr *)&address,len);
    if (result == -1) {
        perror("oop -------");
        exit(1);
    }
    int on = 1;
    setsockopt( sockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&on, sizeof(on));

    while(1){
        int ns;
        bzero(ch,sizeof(ch));
        read(0,ch,sizeof(ch));
        n = strlen(ch);
        printf("*********%s\n",ch);
        if (n == 0 || n == 1) {
            printf("请输入正确的命令:\n");
        }
        else
        {
            write(sockfd,ch,n);
            while(1){
                int x = read(sockfd,ck,sizeof(ck));
                if (x==sizeof(ck)) {
                    printf("%s",ck);
                }
                else
                {
                    printf("%s",ck);
                    break;
                }  
            }
        }
        bzero(ck,4096);
    }
    close(sockfd);
    exit(0);
    
}
