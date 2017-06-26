#ifndef string_h
#define string_h
#include <stdlib.h>

size_t strlen(const char *str)
{
    size_t len =0;
    while(str[len])
        ++len;
    return len;
}


char *strcpy(char *dest, const char *src)
{
    int i =0;
    for (; src[i] != '\0'; ++i)
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}

char *strcat(char *dest, const char *src)
{
    strcpy(dest + strlen(dest), src);
    return dest;
}


int strcmp(const char *str1, const char *str2)
{
    return ! *str1|| ! *str2 || *str1 != *str2 ? *str1 - *str2 : strcmp (str1+1, str2+1);
}


int strncmp(const char *str1, const char *str2, size_t n)
{
    
    return ! *str1|| ! *str2  || *str1 != *str2 ? *str1 - *str2 :(n-1) ==0? 0: strncmp (str1+1, str2+1, --n);
   
}


char *strchr(const char* str, int c)
{
    for(; *str ; str++)
	if(*str == c)
	  return (char*)str;
    return NULL;
}

char *strpbrk(const char *str1, const char *str2)
{
    while(*str1)	
	if(strchr(str2, (int)*str1++))
   	    return (char*)--str1;
    return NULL;
}


char *strstr(const char *haystack, const char *needle)
{
    size_t len = strlen (needle);
    for ( ; *haystack != '\0'; ++haystack)
   	if((strlen(haystack) >= strlen (needle) )&&strncmp (haystack, needle, len) == 0)
	    return (char*)haystack;
    return NULL; 
}


size_t strspn(const char *str1, const char *str2)
{
    size_t to_return = 0;
    while(*str1) 
      if(strchr(str2, (int)*str1++) != NULL)
          ++to_return;
      else return to_return;
}

//exception 1 line = 7
size_t strcspn(const char *s1, const char *s2)
{
    size_t to_return = 0;
    while(*s1)
        if(strchr(s2,*s1) == NULL)
            ++s1, ++to_return;
        else
           return to_return; 
    return to_return;
}

char* strdup(char s[])
{
    return strcpy (malloc (strlen(s)+1),s);
}

// exception 2 line = 9
char *strtok(char *str, const char *delim)
{
    static char* p = 0;
    if(str) 
	 p = str;
    str = p;
    p = strpbrk(str, delim);
    if(p)
    {
        *p = '\0';
        ++p;
	return str;
    } 
    return NULL;
}
#endif /* string_h */

