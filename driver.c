// SAMIP JASANI 2015A7PS0127P
#include "lexer.h"
#include "Trie.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int lineNo, bufSize, bufIndex;
const int noofnt = 44, nooft = 43;
struct TrieNode *root;
char *buf;
FILE *fp;
int strsize;

int main(int argc, char **argv)
{
    bufSize = 1024;
    lineNo = 1;
    bufIndex = 0;
    int i, j;

    // noofnt = 44;
    // nooft = 43;
    buf = (char *)calloc(bufSize, sizeof(char));
    if (argc == 2)
    {
        fp = fopen(argv[1], "r");
    }
    else
    {
        fp = fopen("./testcases/testcase1.txt", "r");
    }
    // tokenPtr t; // = calloc(1, sizeof(Token));
    root = FillTrie();
    // while (fp != NULL || buf[bufIndex] != '\0')
    // {
    //     t = getNextToken();
    //     // printToken(t);
    //     free(t);
    // }

    Grammer G = readGrammer("Grammer.txt");
    // printGrammer(G);
    char **First = (char **)calloc(noofnt, sizeof(char *));
    for (i = 0; i < noofnt; i++)
    {
        First[i] = (char *)calloc(nooft, sizeof(char));
        for (j = 0; j < nooft; j++)
            First[i][j] = '0';
    }
    MakeFirst(G, First);
    // printFirst(First);
    char **Follow = (char **)calloc(noofnt, sizeof(char *));
    for (i = 0; i < noofnt; i++)
    {
        Follow[i] = (char *)calloc(nooft, sizeof(char));
        for (j = 0; j < nooft; j++)
            Follow[i][j] = '0';
    }
    MakeFollow(G, Follow, First);
    // printFollow(Follow);
    Rules **parseTable = MakeParseTable(G, Follow, First);
    // printParseTable(parseTable);
    makeParseTree(parseTable);
    return 0;
}