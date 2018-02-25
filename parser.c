// SAMIP JASANI 2015A7PS0127P
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parserDef.h"
#include "Trie.h"
#include "parser.h"
extern int noofnt;

Grammer readGrammer(char *fname)
{
    struct TrieNode *root = FillTrieforGrammer();
    FILE *fp = fopen(fname, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Error : Failed to open entry file\n");
        return NULL;
        // return 1;
    }
    char *line = NULL, *saveptr1, *str1, *token; //,*p;
    size_t len = 0;
    ssize_t read;
    // char *key;
    // int j, k;
    int lhs, rhs;
    Grammer G = (Grammer)calloc(noofnt, sizeof(Rules));
    Rules currentRule;
    Rhs currentRhs;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        str1 = line;
        token = strtok_r(str1, " ", &saveptr1);
        // if (strcmp(token, "") == 0)
        // continue;
        token[strlen(token) - 1] = '\0';
        lhs = searchforgrammer(root, &token[1]);
        // if (lhs == 0)
        // {
        //     printf("%s\n", token);
        // }
        if (G[lhs] == NULL)
        {
            G[lhs] = (Rules)calloc(1, sizeof(rules));
            currentRule = G[lhs];
        }
        else
        {
            currentRule = G[lhs];
            while (currentRule->next != NULL)
            {
                currentRule = currentRule->next;
            }
            currentRule->next = (Rules)calloc(1, sizeof(rules));
            currentRule = currentRule->next;
        }
        currentRhs = NULL;
        // currentRule->next = NULL;
        // currentRule->rule = NULL;
        while (true)
        {
            token = strtok_r(NULL, " \t\n\r", &saveptr1);
            if (token == NULL)
                break;
            else if (token[0] == '=')
                continue;
            else if (token[0] == '|')
            {
                currentRule->next = (Rules)calloc(1, sizeof(rules));
                currentRule = currentRule->next;
                currentRhs = NULL;
            }
            else if (token[0] == '<')
            {
                token[strlen(token) - 1] = '\0';
                rhs = searchforgrammer(root, &token[1]);
                if (currentRhs == NULL)
                {
                    currentRule->rule = (Rhs)calloc(1, sizeof(rhside));
                    currentRhs = currentRule->rule;
                    currentRhs->isTerminal = false;
                    currentRhs->type = rhs;
                }
                else
                {
                    currentRhs->next = (Rhs)calloc(1, sizeof(rhside));
                    currentRhs = currentRhs->next;
                    currentRhs->isTerminal = false;
                    currentRhs->type = rhs;
                }
                continue;
            }
            else
            {
                // token[strlen(token) - 1] = '\0';
                rhs = searchforgrammer(root, token);
                if (currentRhs == NULL)
                {
                    currentRule->rule = (Rhs)calloc(1, sizeof(rhside));
                    currentRhs = currentRule->rule;
                    currentRhs->isTerminal = true;
                    currentRhs->type = rhs;
                }
                else
                {
                    currentRhs->next = (Rhs)calloc(1, sizeof(rhside));
                    currentRhs = currentRhs->next;
                    currentRhs->isTerminal = true;
                    currentRhs->type = rhs;
                }
                continue;
            }
            // G[lhs] = (Rules)calloc(1, sizeof(rules));
            // currentRule = G[lhs];
            // currentRule->next = NULL;
            // currentRule->rule = NULL;
            // for ( p=token; *p; ++p) *p = tolower(*p);
            // k=hash(token,m);
            // key=malloc(sizeof(char)*(strlen(token)+1));
            // strcpy(key,token);
            // temp = (stopWord)malloc(sizeof(stopword));
            // temp->key=key;
            // temp->next=ht[k];
            // ht[k]=temp;
            // ht[k]=(token,ht[k]);
        }
    }

    fclose(fp);
    return G;
}

void printGrammer(Grammer G)
{
    int i = 0;
    char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};

    Rules currentRule;
    Rhs currentRhs;
    for (i; i < noofnt; i++)
    {
        currentRule = G[i];
        printf("%s :\n", keys1[i]);
        while (currentRule != NULL)
        {
            currentRhs = currentRule->rule;
            while (currentRhs != NULL)
            {
                if (currentRhs->isTerminal)
                    printf("\t%s", keys2[currentRhs->type]);
                else
                    printf("\t%s", keys1[currentRhs->type]);
                currentRhs = currentRhs->next;
            }
            printf("\n");
            currentRule = currentRule->next;
        }
    }
}
