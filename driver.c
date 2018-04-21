// SAMIP JASANI 2015A7PS0127P
#include "lexer.h"
#include "_Trie.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"

int lineNo, bufSize, bufIndex;
const int noofnt = 44, nooft = 43;
struct TrieNode *root;
char *buf;
FILE *fp;
int strsize;
int flag = 0;
int memory = 0;
int countparse = 0;
int sizeparse = 0;
int countast = 0;
int sizeast = 0;

int main(int argc, char **argv)
{
    int i, j, option;
    tokenPtr t;
    if (argc != 3)
    {
        printf("[Usage]: ./compiler testcasefile codegenerationfile\n");
    }
    else
    {
        fp = fopen(argv[1], "r");
    }
    root = FillTrie();
    // printf(" (a) FIRST and FOLLOW set automated \n (b) Both lexical and syntax analysis modules implemented\n (c) Runs on all testcases\n\n");
    printf("LEVEL 4:Symbol Table/AST/Semantic Rules modules work\n\n");
    while (1)
    {
        flag = 0;
        bufSize = 1024;
        lineNo = 1;
        buf = (char *)calloc(bufSize, sizeof(char));
        bufIndex = 0;
        fp = fopen(argv[1], "r");
        printf("Press Option for the defined task:\n 0. Exit loop \n 1. Print token list \n 2. Print ParseTree if no syntax error else print errors \n 3. Print AST \n 4. Print memory allocation and compression ratio \n 5. Print Symbol Table and status of Syntax checking \n 6. Print errors \n 7. Produce Assembly Code (Not Implemented) \n>> ");
        scanf("%d", &option);
        switch (option)
        {
        case 0:
            return 0;
            break;
        case 1:
        {
            fp = getStream(fp, buf);
            while (fp != NULL || buf[bufIndex] != '\0')
            {
                t = getNextToken();
                printToken(t);
            }
            break;
        }
        case 2:
        {
            Grammer G = readGrammer("Grammer.txt");
            char **First = (char **)calloc(noofnt, sizeof(char *));
            for (i = 0; i < noofnt; i++)
            {
                First[i] = (char *)calloc(nooft, sizeof(char));
                for (j = 0; j < nooft; j++)
                    First[i][j] = '0';
            }
            MakeFirst(G, First);
            char **Follow = (char **)calloc(noofnt, sizeof(char *));
            for (i = 0; i < noofnt; i++)
            {
                Follow[i] = (char *)calloc(nooft, sizeof(char));
                for (j = 0; j < nooft; j++)
                    Follow[i][j] = '0';
            }
            MakeFollow(G, Follow, First);
            Rules **parseTable = MakeParseTable(G, Follow, First);
            Node root = makeParseTree(parseTable, First, Follow);
            if (flag == 0)
            {
                FileInorderTree(root, stdout);
                printf("SYNTAX CORRECT\n");
            }
            else
                printf("SYNTAX INCORRECT SO PARSE TREE NOT PRINTED\n");
            break;
        }
        case 3:
        {
            Grammer G = readGrammer("Grammer.txt");
            char **First = InitialiseFirst();
            MakeFirst(G, First);
            char **Follow = InitialiseFollow();
            MakeFollow(G, Follow, First);
            Rules **parseTable = MakeParseTable(G, Follow, First);
            Node root = makeParseTree(parseTable, First, Follow);
            if (flag == 0)
            {
                Node astRoot = makeAST(root, NULL);
                printf("Printing Inorder Traversal of AST (\\|/ indicates going to child ,-> is going to sibling and  /|\\ indicates going to parent)\n");
                PrintInorderASTree(astRoot);
                // Node stRoot = makeST(astRoot, NULL);
                // printST(stRoot);
            }
            else
                printf("SYNTACTIC FAILURE\n");
            break;
        }
        case 4:
            //calculate size
            countast = 0;
            countparse = 0;
            sizeast = 0;
            sizeparse = 0;
            Grammer G = readGrammer("Grammer.txt");
            char **First = InitialiseFirst();
            MakeFirst(G, First);
            char **Follow = InitialiseFollow();
            MakeFollow(G, Follow, First);
            Rules **parseTable = MakeParseTable(G, Follow, First);
            Node root = makeParseTree(parseTable, First, Follow);
            if (flag == 0)
            {
                Node astRoot = makeAST(root, NULL);
                printf(" Parse Tree nodes : %d \t\t Allocated Memory : %d Bytes\n AST nodes : %d\t\t\tAllocated Memory : %d Bytes\n Compression percentage : %.2f\n",
                       countparse, sizeparse, countast, sizeast, ((float)(sizeparse - sizeast) / sizeparse) * 100);
                // printf("Printing Inorder Traversal of AST\n");
                // PrintInorderASTree(astRoot);
                // Node stRoot = makeST(astRoot, NULL);
                // printST(stRoot);
            }
            else
                printf("SYNTACTIC FAILURE\n");
            break;
        case 5:
        {
            Grammer G = readGrammer("Grammer.txt");
            char **First = InitialiseFirst();
            MakeFirst(G, First);
            char **Follow = InitialiseFollow();
            MakeFollow(G, Follow, First);
            Rules **parseTable = MakeParseTable(G, Follow, First);
            Node root = makeParseTree(parseTable, First, Follow);
            if (flag == 0)
            {
                Node astRoot = makeAST(root, NULL);
                Node stRoot = makeST(astRoot, NULL);
                // if (flag == 0)
                printST(stRoot);
                // else
                if (flag == 1)
                    printf("SEMANTIC FAILURE\n");
            }
            else
                printf("SYNTACTIC FAILURE\n");
            break;
        }
        case 6:
        {
            Grammer G = readGrammer("Grammer.txt");
            char **First = InitialiseFirst();
            MakeFirst(G, First);
            char **Follow = InitialiseFollow();
            MakeFollow(G, Follow, First);
            Rules **parseTable = MakeParseTable(G, Follow, First);
            Node root = makeParseTree(parseTable, First, Follow);
            if (flag == 0)
            {
                Node astRoot = makeAST(root, NULL);
                Node stRoot = makeST(astRoot, NULL);
                if (flag == 0)
                    printf("SEMANTIC CORRECT\n");
                else
                    printf("SEMANTIC FAILURE\n");
            }
            else
                printf("SYNTACTIC FAILURE\n");
            break;
        }
        case 7:
        {
            Grammer G = readGrammer("Grammer.txt");
            char **First = InitialiseFirst();
            MakeFirst(G, First);
            char **Follow = InitialiseFollow();
            MakeFollow(G, Follow, First);
            Rules **parseTable = MakeParseTable(G, Follow, First);
            Node root = makeParseTree(parseTable, First, Follow);
            if (flag == 0)
            {
                Node astRoot = makeAST(root, NULL);
                Node stRoot = makeST(astRoot, NULL);
                if (flag == 0)
                {
                    printf("SEMANTIC CORRECT\n");
                    printf("Assembly Code not produced\n");
                }
                else
                    printf("SEMANTIC FAILURE\n");
            }
            else
                printf("SYNTACTIC FAILURE\n");
            break;
        }
        default:
            return 0;
        }
    }
    return 0;
}