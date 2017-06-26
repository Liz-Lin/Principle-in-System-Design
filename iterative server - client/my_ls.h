#ifndef my_ls_h
#define my_ls_h

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


void do_ls (char* file_path, char* file_name, int num_indent);

#endif

