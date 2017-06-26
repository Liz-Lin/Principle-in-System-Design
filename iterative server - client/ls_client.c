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

#define PORT 26798
#define MAXDATASIZE 500
#define h_addr h_addr_list[0]
#define SERVERHOST      "localhost"

struct sockaddr_in their_addr;

void copy_file(int from, int to, char* buf) // from and to are the ints by open() or socket()
{
    int n=0;
    while ((n = read(from, buf, BUFSIZ)) > 0)
        if (write(to, buf, n) == -1)
            perror("Write failed");
}

int make_socket (char* hostname)
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
    int numbytes =read(sockfd, buf, MAXDATASIZE-1);
    if (numbytes < 0)
    {
        perror ("read");
        exit (EXIT_FAILURE);
    }
    else if (numbytes == 0)
    {
        close(sockfd);
        return -1;
    }
    else
        return 1;
    
}


int main(int argc, char** argv)
{
    
    if(argc <= 2)
    {
        fprintf(stderr, "Client-Usage: %s the_client_hostname\n", argv[0]);
        exit(1);
    }
    for(int i = 2; i < argc; ++i )
    {
        int sockfd = make_socket(argv[1]);
        write(sockfd, argv[i], strlen(argv[i])+1);
        char buf[MAXDATASIZE];
        memset(buf, '\0', MAXDATASIZE);
        while(( read_from_server(sockfd,buf) )>0)
        {
            printf("%s",buf);
            fflush(stdout);
            memset(buf, '\0', MAXDATASIZE);
        }
        printf("\nEnd of file %d %s for command ls\n", (i-1), argv[i]);
        close(sockfd);
    }
    
    return 0;
}
