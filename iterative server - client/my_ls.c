#include "my_ls.h"
#include <stdbool.h>
#include <stdint.h>
char indent[100];
void do_ls (char* file_path, char* file_name, int num_indent);
void process (char* file_path, char* file_name, int num_indent, struct stat s);
char get_filetype(int m)
{
    char c;
    if (S_ISREG(m))
        c = '-';
    else if (S_ISCHR(m))
        c = 'c';
    else if (S_ISDIR(m))
        c = 'd';
    else if (S_ISBLK(m))
        c = 'b';
    else
        c = '?';
    return c;
}

void user_permission (int mode, char* per)
{
    per[0] = mode&S_IRUSR? 'r':'-';
    per[1] = mode&S_IWUSR? 'w':'-';
    per[2] = mode&S_IXUSR? 'x':'-';
}

void group_permission (int mode, char* per)
{
    per[3] = mode&S_IRGRP? 'r':'-';
    per[4] = mode&S_IRGRP? 'w':'-';
    per[5] = mode&S_IXGRP? 'x':'-';
}

void other_permission (int mode, char* per)
{
    per[6] = mode&S_IROTH? 'r':'-';
    per[7] = mode&S_IWOTH? 'w':'-';
    per[8] = mode&S_IXOTH? 'x':'-';
    
}

char* permission (int mode, char* per)
{
    user_permission(mode,per);
    group_permission(mode,per);
    other_permission(mode,per);
    per[9] = '\0';
    return per;
}

char* get_UserName(uid_t uid)
{
    struct passwd* pw = getpwuid(uid);
    return pw->pw_name ? pw->pw_name : "";
}

char* get_GroupName(gid_t gid)
{
    struct group* grp = getgrgid(gid);
    return grp->gr_name ? grp->gr_name : "";
}

void last_modification_and_name(char* tmbuff, struct stat status,char* d_name)
{
    struct tm *tm;
    tm = localtime(&status.st_mtime);
    strftime(tmbuff, sizeof(tmbuff), "%b %d", localtime(&status.st_mtime));
    printf(" %s %d:%d %s\n",tmbuff, tm->tm_hour, tm->tm_min, d_name);
}

char* indentation(int num_indent)
{
    for(int i =0; i < 4*num_indent; ++i)
        indent[i] = ' ';
    indent[num_indent*4] = '\0';
    return indent;
}

void file_info (int num_indent ,char* d_name, struct stat status, char type)
{
    char per[11], tmbuff[100];
    type = get_filetype(status.st_mode);
    printf("%s%c%s",indentation(num_indent), type, permission(status.st_mode, per ));
    printf(" %d %s %s  %5jd",status.st_nlink, get_UserName(status.st_uid), get_GroupName(status.st_gid),(intmax_t)status.st_size);
    last_modification_and_name(tmbuff,status, d_name);
}

//line exception 1 -- line = 7
void is_file(char* file_name, int num_indent, struct stat s)
{
    char* new_name = strrchr(file_name, '/');
    if(new_name == NULL)
        new_name = file_name;
    else
        ++new_name;
    if(strcmp(file_name, "..") != 0 && strcmp(file_name, ".") != 0)
        file_info(num_indent, new_name, s, '-');
}

void read_dir (DIR* dirp, char* file_path, int num_indent)
{
    struct dirent* direntp;
    char new_path[100];
    while((direntp = readdir(dirp))){
        
        if(strcmp(direntp->d_name, "..") != 0 && strcmp(direntp->d_name, ".") != 0)
        {
            sprintf(new_path, "%s/%s", file_path, direntp->d_name);
            struct stat s;
            if(stat(new_path, &s) == -1 )
            {
                printf("stat wrong %s\n", file_path);
                return;
            }
            is_file(direntp->d_name,num_indent,s);
            //do_ls(new_path,direntp->d_name, num_indent+1);
        }
    }
}
//line exception 2 --- line = 6
void is_directory (char* file_path, char* file_name, int num_indent, struct stat s)
{
    DIR* dirp;
    file_info(num_indent, file_name, s, 'd');
    if(!(dirp= opendir(file_path)))
    {
        fprintf(stderr, "Error(%d) opening %s \n", errno, file_path);
        return;
    }
    
    struct dirent* direntp;
    char new_path[100];
    while((direntp = readdir(dirp))){
        
        if(strcmp(direntp->d_name, "..") != 0 && strcmp(direntp->d_name, ".") != 0)
        {
            sprintf(new_path, "%s/%s", file_path, direntp->d_name);
            struct stat s;
            if(stat(new_path, &s) == -1 )
            {
                //printf("stat wrong %s\n", file_path);
                continue;
            }
            if(S_ISREG(s.st_mode))
                file_info(num_indent+1, direntp->d_name, s, '-');
            if(S_ISDIR(s.st_mode))
                file_info(num_indent+1, direntp->d_name, s, 'd');
            else
                continue;
            
        }
    }

    closedir(dirp);
}

void process (char* file_path, char* file_name, int num_indent, struct stat s)
{
    if(S_ISREG(s.st_mode))
        is_file(file_name, num_indent, s);
    
    if(S_ISDIR(s.st_mode))
        is_directory(file_path, file_name, num_indent, s);
    
    return;
}

void do_ls (char* file_path, char* file_name, int num_indent)
{
    struct stat s;
    if(stat(file_path, &s) == -1 )
    {
        printf("stat wrong %s\n", file_path);
        return;
    }
    process(file_path, file_name, num_indent, s);
    return;
}

