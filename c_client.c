
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#define h_addr h_addr_list[0]

struct sockaddr_in their_addr;

void sigchld_handler(int sig)
{
    while (waitpid(-1, 0, WNOHANG) > 0);
    return;
}

int make_socket (char* hostname, uint16_t PORT)
{
    int sockfd;
    struct hostent *he;
    if((he=gethostbyname(hostname)) == NULL)
    {
        perror("gethostbyname()");
        exit(1);
    }
    else
        printf("Client-The remote host is: %s\n", hostname);
    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket()");
        exit(1);
    }
    else
        printf("Client-The socket() sockfd is OK...\n");
    
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);
    
    if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect()");
        exit(1);
    }
    else
        printf("Client-The connect() is OK...\n");
    return sockfd;
}

int read_from_server(int sockfd, char* buf)
{
    int numbytes =read(sockfd, buf, BUFSIZ);
    if (numbytes < 0)
    {
        perror ("read");
        exit (EXIT_FAILURE);
    }
    else if (numbytes == 0)
        return -1;
    else
        return numbytes;
}

void read_and_write_file_from_server(int serverfd, char* file)
{
    char buffer[BUFSIZ];
    int oop, count;
    if( (oop = open(file, O_CREAT|O_WRONLY| O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) <0){
        fprintf(stderr, "Cannot open to write\n");
        exit(1);
    }
    else
    {
        while((count = read_from_server(serverfd,buffer)) > 0)
        {
            if((write(oop, buffer, count)) <0)
            {
                perror("Write() to file");
                exit(EXIT_FAILURE);
            }
            count=0;
        }
        close(oop);
    }
}

char* base_name(char* path)
{
   return strrchr(path, '/');
}

void fork_child_copying_file_from_socket(int sockfd,char* file_path,char* file_name)
{
    int pid = fork();
    switch(pid){
        case -1:
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        case 0:
            if(write(sockfd, file_path, strlen(file_path)+1) <0)
           {
               perror("Write to server");
               exit(EXIT_FAILURE);
           }
            read_and_write_file_from_server(sockfd, file_name);
            printf("End of copying for file %s and close socket \n\n", file_name);
            close(sockfd);
            exit(0);
            break;
        default:
            break;
    }
    return;
}
// client `hostname` $P `find /usr/share/java -size +500`
int main(int argc, char** argv)
{
    if(argc < 4 )
    {
        fprintf(stderr, "Client-Usage: %s the_client_hostname\n", argv[0]);
        exit(1);
    }
    signal(SIGCHLD, sigchld_handler);
    for(int i = 3; i < argc; ++i )
    {
        int sockfd = make_socket(argv[1],atoi(argv[2]));
        char* name = base_name(argv[i]);
        if(++name != NULL)
            fork_child_copying_file_from_socket(sockfd, argv[i], name);
        close(sockfd);
    }
    
    return 0;
}

