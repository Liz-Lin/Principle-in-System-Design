
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int peekc()
{
    int c = getchar();
    ungetc(c, stdin);
    return c;
}
void parseNumber(char ch)
{
    printf("%c", ch);
    while (isdigit(peekc()) || peekc() == '.')
        printf("%c", getchar());
    printf("\n");
}

void parseWord(char ch)
{
    printf("%c", ch);
    while (isalnum(peekc()) || peekc() == '_' || peekc() == '-')
        printf("%c", getchar());
    printf("\n");
}


void parseOpeartor(char ch)
{
    switch(peekc())
    {
        case '&':
            if (ch == '&')
                printf("%c\n", getchar());
        case '|':
            if (ch == '|')
                printf("%c\n", getchar());
        case '<':
            if (ch == '<')
                printf("%c\n", getchar());
        case '>':
            if (ch == '>')
                printf("%c\n", getchar());
        case '+':
            if (ch == '+')
                printf("%c\n", getchar());
        case '-':
            if (ch == '-')
                printf("%c\n", getchar());
        case '=':
            if (ch == '=' || ch == '+' || ch == '-' || ch == '*' || ch == '/')
            printf("%c\n", getchar());
            break;
        default:
            printf("\n");
            break;
    }
}

bool end()
{
    if(getchar() == '*')
        if(getchar() == '/')
            return true;
    return false;
}

void processComment(char ch)
{
    switch(peekc())
    {
        case '/':
            while(getchar() != '\n') ;
            break;
        case '*':
            while(!end());
            break;
        default:
            break;
    }
}

void parseQuote(char ch)
{
    while((ch = getchar()) != '\'')
        printf("%c", ch);
}

void parseQuoteQuote(char ch)
{
    while((ch = getchar()) != '"'){
        switch(ch)
        {
            case '\\':
                printf("%c%c", ch, getchar());
                break;
            default:
                printf("%c", ch);
                break;
        }
    }
}

void parse(char ch)
{
    switch(ch)
    {
        case ' ':
        case '\t':
        case '\n':
            break;
        case '&':
        case '|':
        case '<':
        case '>':
        case '*':
        case '+':
        case '-':
        case '=':
            printf("%c", ch);
            parseOpeartor(ch);
            break;
        case '/':
            processComment(ch);
            break;
        case '\'':
            printf("\'%c\'\n", getchar());
            getchar();
	    break;
        case '\"':
            printf("%c", ch);
            parseQuoteQuote(ch);
            printf("%c\n", ch);
            break;
        
        default:
            if (isdigit(ch))
                parseNumber(ch);
            else if (isalpha(ch))
                parseWord(ch);
            else
                printf("%c\n", ch);
            break;
    }
}
int main()
{
    int ch;
    while((ch = getchar()) != EOF)
    {
        parse(ch);
    }
    printf("\n");
    return 0;
}

