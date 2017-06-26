#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "Timer.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdbool.h>

void print_time_1 (FILE* ifp, FILE* ofp)
{
    char ch;
    double wc_t=0, usr_t=0, sys_t=0;
    
    Timer_start();
    while( (ch = fgetc(ifp)) != EOF)
        fputc(ch, ofp);
    
    Timer_elapsedTime(&wc_t, &usr_t, &sys_t);
    printf("wallclock: %lf, user: %lf, system time: %lf\n", wc_t, usr_t, sys_t);

}

bool check_file(FILE* ifp, FILE* ofp, char* inputFilename, char* outputFilename)
{
    if(ifp == NULL || ofp ==NULL)
    {
        fprintf(stderr, "Cant open both file %s %s\n", inputFilename, outputFilename);
        fclose(ifp);
        fclose(ofp);
        return false;
    }
    return true;
}

void my_copy1 (char* inputFilename, char* outputFilename)
{
    FILE* ifp = fopen(inputFilename, "r");
    FILE* ofp = fopen(outputFilename, "w");
    
    if(check_file(ifp, ofp, inputFilename, outputFilename))
    {
        print_time_1(ifp, ofp);
        fclose(ifp);
        fclose(ofp);
    }
}

void print_time_2 (int iop, int oop)
{
    char c;
    double wc_t=0, usr_t=0, sys_t=0;
    
    Timer_start();
    while(read(iop, &c, 1) == 1)
        if(write(oop, &c, 1) == -1)
            exit(1);
    
    Timer_elapsedTime(&wc_t, &usr_t, &sys_t);
    printf("wallclock: %lf, user: %lf, system time: %lf\n", wc_t, usr_t, sys_t);
}

bool check_file_2 (int iop, int oop, char* inputFilename, char* outputFilename)
{
    if(iop == -1 || oop == -1)
    {
        fprintf(stderr, "Cant open both file %s %s\n", inputFilename, outputFilename);
        close(iop);
        close(oop);
        return false;
    }
    return true;

}

void my_copy2 (char* inputFilename, char* outputFilename)
{
    int iop  = open(inputFilename, O_RDONLY);
    int oop  = open(outputFilename, O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG |S_IRWXO);
    if(check_file_2(iop, oop, inputFilename, outputFilename))
    {
        print_time_2(iop, oop);
        close(iop);
        close(oop);
    }
}

void print_time_3 (int iop, int oop)
{
    double wc_t=0, usr_t=0, sys_t=0;
    int count =0;
    char buff[BUFSIZ];
    
    Timer_start();
    while(( count = read(iop, buff, BUFSIZ) ) > 0)
        if(write(oop, buff, count) ==-1)
            exit(1);
    
    Timer_elapsedTime(&wc_t, &usr_t, &sys_t);
    printf("wallclock: %lf, user: %lf, system time: %lf\n", wc_t, usr_t, sys_t);
}

void my_copy3 (char* inputFilename, char* outputFilename)
{
    int iop   = open(inputFilename, O_RDONLY);
    int oop  = open(outputFilename, O_CREAT | O_WRONLY,S_IRWXU | S_IRWXG | S_IRWXO );

    if(check_file_2(iop, oop, inputFilename, outputFilename))
    {
        print_time_3(iop, oop);
        close(iop);
        close(oop);
    }
}

int decide_repeat(int argc, char* argv[])
{
    int repeat=0;
    if(argc == 4)
        repeat = 1;
    else if(argc == 5)
        repeat = atoi(argv[4]);
    return repeat;
}

int main(int argc, char* argv[])
{
    for(int i =0; i< decide_repeat(argc, argv); ++i)
    {
        switch (atoi(argv[1]))
        {
            case 1:
                my_copy1(argv[2], argv[3]);
                break;
            case 2:
                my_copy2(argv[2], argv[3]);
                break;
            case 3:
                my_copy3(argv[2], argv[3]);
                break;
            default:
                printf("wrong inputs\n");
        }
    }
    
  
    return 0;
    
    
}
