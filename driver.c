// SAMIP JASANI 2015A7PS0127P
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>

int lineNo, bufSize, bufIndex;
char *buf;
FILE *fp;

int main(int argc, char **argv)
{
    bufSize = 1024;
    lineNo = 1;
    bufIndex = 0;
    buf = (char *)calloc(bufSize, sizeof(char));
    if (argc == 2)
    {
        fp = fopen(argv[1], "r");
    }
    else
    {
        fp = fopen("test", "r");
    }
    fp = getStream(fp, buf);
    tokenPtr t; // = calloc(1, sizeof(Token));
    while (fp != NULL || buf[bufIndex] != '\0')
    {
        t = getNextToken();
        printToken(t);
        free(t);
    }
    // while (fp != NULL)
    // {
    //     printf("%s", buf);
    //     fp = getStream(fp, buf);
    // }
    // if (*buf != '\0')
    // {
    //     printf("%s", buf);
    // }
}