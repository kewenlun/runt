#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<signal.h>

void do_same1(int cilent_socket);
void ding(int sig){
    printf("子进程结束\n");
}
int main(){
    int server_sockfd,client_sockfd;
    int server_len,client_len;
    pid_t pid;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    signal(SIGCHLD, SIG_IGN);
    server_sockfd = socket(AF_INET,SOCK_STREAM,0);
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = 9374;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_len = sizeof(server_address);
    int on = 1;
    if (setsockopt(server_sockfd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        perror("setsockopt error");
    bind(server_sockfd,(struct sockaddr *)&server_address,server_len);


    listen(server_sockfd,5);
    while(1){
        char ch[4096];

        printf("server waiting\n");

        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd,(struct sockaddr *)&client_address,(socklen_t *)&client_len);
        
        pid = fork();
        if (pid == (pid_t)-1) {
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            do_same1(client_sockfd);
            printf("*******123456****");
            kill(getppid(),SIGINT);
            exit(0);
        }
        else
        {
            (void)signal(SIGINT,ding);
            // pause();
            // exit(0);
             close(client_sockfd);
        }
        
        
    }
}
void do_same1(int client_socket){
    char ch[2048];
    while(1){
        memset(ch,0,sizeof(ch));
        int x = read(client_socket,ch,sizeof(ch));
        if (x == 0) {
            break;
        }else if (x == -1)
        {
            perror("错误信息：");
        }
        
        dup2(client_socket,1);
        dup2(client_socket,2);
        system(ch);
    }//*/
}