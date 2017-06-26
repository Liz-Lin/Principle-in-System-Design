#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>


int make_socket (uint16_t PORT)
{
    int sockfd;
    struct sockaddr_in server;

    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror ("socket");
        exit (EXIT_FAILURE);
    } else
        printf("socket() is OK...\n");

    server.sin_family = AF_INET;
    server.sin_port = htons (PORT);
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    
    if (bind (sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1)
    {
        perror ("bind");
        exit (EXIT_FAILURE);
    }else
        printf("bind() is OK...\n");
    if (listen (sockfd, 20) < 0)
    {
        perror ("listen");
        exit (EXIT_FAILURE);
    }else
        printf("listen() is OK...\n");
    
    return sockfd;
}

void write_to_client (int sockfd, char* file_name)
{
    int iop;
    if( ( iop= open(file_name, O_RDONLY) ) < 0)
    {
        close(iop);
        fprintf(stderr,"failed open file %s", file_name);
        exit(EXIT_FAILURE);
    }
    
    int count =0;
    char buff[BUFSIZ];

    while(( count = read(iop, buff, BUFSIZ) ) > 0){
        if(write(sockfd, buff, count) < 0)
        {
            perror("write to socket\n");
            exit(EXIT_FAILURE);
        }
    }
    close(iop);
    return;
   
}


int read_from_client (int sockfd)
{
    char file_name[100];
    memset(file_name, '\0', 100);
    int nbytes;
    nbytes = read (sockfd, file_name, 100);
    if (nbytes < 0)/* Read error. */
    {
        perror ("read");
        exit (EXIT_FAILURE);
    }
    else if (nbytes == 0)/* End-of-file. */
        return -1;
    else/* Data read. */
    {
        fprintf (stderr, "Server: got file: `%s'\n", file_name);
        write_to_client(sockfd,file_name);
        return 1;
    }
}



void handler(int signal)
{
    while (waitpid(-1, 0, WNOHANG) > 0);
    return;
}

void fork_child_copying_file_to_socket(int sockfd, int clientfd)
{
    int pid = fork();
    switch(pid){
        case -1:
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        case 0:
            close(sockfd);
            while(read_from_client(clientfd)>0);
            exit(0);
        default:
            break;
    }
}

int main(int argc, char** argv)
{
    if(argc != 2)
        exit(EXIT_FAILURE);
    uint16_t PORT = atoi(argv[1]);

    int sockfd = make_socket(PORT);
    struct sockaddr_in client;
    
    printf("Server started...\n");
    int clientfd,sin_size = sizeof (struct sockaddr_in);
    signal(SIGCHLD, handler);
    
    while(1)
    {
        if((clientfd = (accept(sockfd, (struct sockaddr *)&client,(socklen_t *)&sin_size))) <0 )
        {
            perror("accept() error!");
            exit(EXIT_FAILURE);
        }else
            printf("accept() is OK...\n");
        fork_child_copying_file_to_socket(sockfd,clientfd);
        close(clientfd);
    }
    
    close(sockfd);
    return 0;
}


