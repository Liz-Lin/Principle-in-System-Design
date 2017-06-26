#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

struct sigaction act;

int sig_int = 0, sig_quit = 0, sig_stp = 0;

void handle_interrupt()
{
    printf("I");
    fflush(stdout);
}

void handle_quit()
{
    printf("Q");
    fflush(stdout);
}

void check_sig_stp ()
{
    if (sig_stp == 3)
    {
        printf("Interrupt: %d\nSTOP: %d\nQuit: %d\n", sig_int, sig_stp, sig_quit);
        exit(0);
    }
    else
        raise(SIGSTOP);
}

void handle_stop()
{
    printf("S");
    fflush(stdout);
    check_sig_stp();
}

void sighandler(int signum, siginfo_t *info, void *ptr)
{
    switch(signum)
    {
        case SIGINT:
            ++sig_int;
            handle_interrupt();
            break;
        case SIGQUIT:
            ++sig_quit;
            handle_quit();
            break;
        case SIGTSTP:
            ++sig_stp;
            handle_stop();
            break;
        default:
            printf("Wrong signal\n");
    }
    
}
void handler()
{
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
    sigaction(SIGTSTP, &act, NULL);
}

void init_signal_handlers()
{
    act.sa_sigaction = sighandler;
    act.sa_flags = SA_SIGINFO;
    handler();
}

int main(int argc, char* argv[])
{
    init_signal_handlers();
    while(1)
    {
        printf("X");
        fflush(stdout);
        sleep(1);
    }
    return 0;
}
