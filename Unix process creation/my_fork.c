#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void printChar(char letter, int num)
{
    for (int i =0; i < num; ++i)
    {
        printf("%c", letter);
        fflush(stdout);
    }
}

void fork_again(char ch1, char ch2, int num)
{
    pid_t p = fork();
    int status;
    switch(p)
    {
        case -1:
            perror("Cannot fork");
            exit(1);
            break;
        case 0:
            //printf("Process ID for %c: %d\n", ch1, getpid());
            printChar(ch1, num);
            break;
        default:
            //printf("Process ID for %c: %d\n", ch2, getpid());
            printChar(ch2, num);
            wait(&status);
            break;
    }
}

void my_fork(int num)
{
    pid_t p = fork();
    int status;
    switch(p)
    {
        case -1:
            perror("Cannot fork.");
            exit(1);
            break;
        case 0:
            fork_again('A','C', num);
            break;
        default:
            fork_again('B','D', num);
            wait(&status);
            break;
    }
}


int main(int argc, char* argv[], char* envp[])
{
    int num;
    if(argc == 1)
        num = 10000;
    else
        num = atoi (argv[1]);
    my_fork(num);
    return 0;
}

