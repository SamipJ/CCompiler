// SAMIP JASANI 2015A7PS0127P
#include "lexer.h"
#include "_Trie.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

int lineNo, bufSize, bufIndex;
const int noofnt = 44, nooft = 43;
struct TrieNode *root;
char *buf;
FILE *fp;
int strsize;
int flag;

int main(int argc, char **argv)
{
    bufSize = 1024;
    lineNo = 1;
    bufIndex = 0;
    int i, j, option;
    FILE *fileout;
    tokenPtr t;
    // noofnt = 44;
    // nooft = 43;
    printf(" (a) FIRST and FOLLOW set automated \n (b) Both lexical and syntax analysis modules implemented\n\n");
    printf("Press Option for the defined task: \n 1. for print comment free code \n 2. print token list by lexer \n 3. Parsing of Source Code \n 4. Parsing and Printing Parse Tree\n >> ");
    scanf("%d", &option);
    buf = (char *)calloc(bufSize, sizeof(char));
    if (argc >= 2)
    {
        fp = fopen(argv[1], "r");
    }
    else
    {
        fp = fopen("./testcases/testcase1.txt", "r");
    }
    root = FillTrie();
    if (option == 1)
    {
        CommentFree();
    }
    else if (option == 2)
    {
        fp = getStream(fp, buf);
        while (fp != NULL || buf[bufIndex] != '\0')
        {
            t = getNextToken();
            printToken(t);
        }
    }
    else if (option == 3)
    {
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
        Node root = makeParseTree(parseTable, First, Follow);
        // PrintInorderTree(root);
        // if (argc >= 3)
        // {
        //     fileout = fopen(argv[2], "w+");
        // }
        // else
        // {
        //     fileout = fopen("./parsetreeoutfile.txt", "w+");
        // }
        // FileInorderTree(root, fileout);
        printf("COMPILATION COMPLETE\n");
    }
    else if (option == 4)
    {
        root = FillTrie();
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
        Node root = makeParseTree(parseTable, First, Follow);
        // PrintInorderTree(root);
        if (argc >= 3)
        {
            fileout = fopen(argv[2], "w+");
        }
        else
        {
            fileout = fopen("./parsetreeoutfile.txt", "w+");
        }
        FileInorderTree(root, fileout);
        fclose(fileout);
    }
    else
    {
        printf("restart the code. Error Input\n");
    }

    return 0;
}