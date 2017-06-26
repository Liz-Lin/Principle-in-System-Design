
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
//exception 1  line =7
void parse(char* buff, char* args[])
{
    while(*buff != '\0')
    {
        while(*buff == ' ' || *buff == '\n' || *buff == '\t')
            *buff++ = '\0';
            
        *args++ = buff;
        while(*buff != '\0' && *buff != ' ' && *buff != '\t' && *buff != '\n')
            buff++;
    }
    *args = 0;
}

void execute_path (char** args, char* temp_path, char** envp,struct stat s)
{
    char path[255];
    for(char* bin = strtok(temp_path, ":"); bin != NULL; bin = strtok(NULL, ":"))
    {
        snprintf(path, 255, "%s/%s", bin, args[0]);
        if((stat(path,&s) == 0)){
            args[0] = path;
            execve(path, args, envp);
        }
    }
}

void execute(char** args, char* temp_path, char** envp)
{
    struct stat s;
    if(stat(args[0], &s) != -1)
        execve(args[0], args, envp);
    else
        execute_path(args, temp_path, envp, s);
    
}

int main(int argc, char* argv[], char* envp[])
{
    char* args[255];
    char buff[255];
    char temp[255];
    char* envp_path = getenv("PATH");
    
    fgets(buff, 255, stdin);
    parse(buff,args);
    strcpy(temp, envp_path);
    execute(args, temp, envp);
    return 0;
}

