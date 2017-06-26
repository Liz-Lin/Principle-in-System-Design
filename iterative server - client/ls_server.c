#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "my_ls.h"
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/syscall.h>

#define PORT 26798
#define MAXDATASIZE 500

int make_socket ()
{
    int sockfd;
    struct sockaddr_in server;
    
    /* Create the socket. */
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
    if (bind (sockfd, (struct sockaddr *) &server, sizeof(struct sockaddr)) < 0)
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
void write_to_client (int filedes, char* msg)
{
    int nbytes;
    nbytes = write (filedes, msg, strlen (msg) + 1);
    if (nbytes < 0)
    {
        perror ("write");
        exit (EXIT_FAILURE);
    }
    return;
}

int read_from_client (int filedes, char* buffer)
{
    
    int nbytes;
    nbytes = read (filedes, buffer, MAXDATASIZE);
    if (nbytes < 0)/* Read error. */
    {
        perror ("read");
        exit (EXIT_FAILURE);
    }
    else if (nbytes == 0)/* End-of-file. */
        return -1;
    else/* Data read. */
    {
        fprintf (stderr, "Server: got message: `%s'\n", buffer);
        return 1;
    }
}
int send_ls(int sockfd, char* buffer)
{
    int nbytes;
    if( (nbytes = read (sockfd, buffer, MAXDATASIZE)) >0)
    {
        printf("%s\n",buffer);
        switch(fork())
        {
            case -1:
                perror("Cannot fork\n");
                exit(EXIT_FAILURE);
            case 0:
                dup2(sockfd, 1);
                do_ls(buffer, buffer, 0);
                exit(0);
            default:
                wait(0);
                close(sockfd);
                break;
        }
        return 1;
    }
    else if (nbytes == 0)/* End-of-file. */
        return -1;
    else/* Data read. */
    {
        perror ("read");
        exit (EXIT_FAILURE);
    }
    
}

int main(int argc, char** argv)
{
    int sockfd = make_socket();
    struct sockaddr_in client;
    
    printf("Server started...\n");
    int clientfd,sin_size = sizeof (struct sockaddr_in);
    
    
    while(1)
    {
        if((clientfd = (accept(sockfd, (struct sockaddr *)&client,(socklen_t *)&sin_size))) <0 )
        {
            perror("accept() error!");
            exit(EXIT_FAILURE);
        }else
            printf("accept() is OK...\n");
        char buffer[MAXDATASIZE];
        memset(buffer, '\0', MAXDATASIZE);
        send_ls(clientfd,buffer);
        close(clientfd);
    }
    
    close(sockfd);
    printf("Closing Socket now.....\n");
    //shutdown(sockfd, 2);
    return 0;
}


