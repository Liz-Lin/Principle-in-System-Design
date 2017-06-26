#include "my_ls.h"
int main(int argc, char* argv[])
{
    int num_indent =0;
    
    if(argc == 1)
        do_ls(".",".", num_indent);
    else
        for(int i =1; i < argc; ++i)
            do_ls(argv[i], argv[i], num_indent);
    return 0;
}
