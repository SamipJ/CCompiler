// SAMIP JASANI 2015A7PS0127P
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parserDef.h"
#include "_Trie.h"
#include "parser.h"
#include "_Stack.h"
#include "_Tree.h"
extern int lineNo, bufSize, bufIndex, flag;
extern char *buf;
extern FILE *fp;
// extern int noofnt;

extern const int noofnt, nooft;

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
    size_t read;
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
    printf("PRINTING GRAMMER\n");
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

void MakeFirst(Grammer G, char **First)
{
    int i;
    char firstDone[noofnt];
    // Rules currentRule;
    // Rhs currentRhs;
    for (i = 0; i < noofnt; i++)
    {
        firstDone[i] = '0';
    }
    for (i = 0; i < noofnt; i++)
    {
        if (firstDone[i] == '1')
            continue;
        else
        {
            FirstDAC(G, First, firstDone, i);
        }
        // currentRule = G[i];
        // while(currentRule!=NULL){
        // ??
        // currentRule = currentRule->next;
    }
    return;
}

void FirstDAC(Grammer G, char **First, char *firstDone, int key)//FIRST DIVIDE AND CONQUER
{
    int i;
    if (firstDone[key] == '1')
    {
        return;
    }
    else
    {
        Rules currentRule = G[key];
        while (currentRule != NULL)
        {
            Rhs currentRhs = currentRule->rule;

            while (currentRhs != NULL)
            {
                if (currentRhs->isTerminal)
                {
                    First[key][currentRhs->type] = '1';
                    break;
                }
                else
                {
                    if (firstDone[currentRhs->type] == '0')
                    {
                        FirstDAC(G, First, firstDone, currentRhs->type);
                    }
                    //copy and check epsilon thing
                    for (i = 0; i < nooft; i++)
                    {
                        if (First[currentRhs->type][i] == '1' && i != EPSILON)
                        {
                            // if (i == 14)
                            //     printf("hi");
                            First[key][i] = '1';
                        }
                    }
                    if (First[currentRhs->type][EPSILON] == '1')
                    {
                        currentRhs = currentRhs->next;
                        continue;
                        // First[key][EPSILON] = '0';
                    }
                    else
                        break;
                }
            }
            if (currentRhs == NULL)
            {
                First[key][EPSILON] = '1';
            }
            currentRule = currentRule->next;
        }
        firstDone[key] = '1';
        return;
    }
}

void printFirst(char **First)
{
    printf("PRINTING FIRST SET\n");
    char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
    int i, j;
    for (i = 0; i < noofnt; i++)
    {
        printf("%s :\n", keys1[i]);
        for (j = 0; j < nooft; j++)
        {
            if (First[i][j] == '1')
                printf("\t %s", keys2[j]);
        }
        printf("\n");
    }
    return;
}

void MakeFollow(Grammer G, char **Follow, char **First)
{
    Follow[mainFunction][FINISH] = '1';
    int prevFollowLength = 0, followLength = 1;
    int i, j, count = 0;
    Rules currentRule;
    Rhs currentRhs, followingRhs;
    while (prevFollowLength != followLength)
    {
        count++;
        prevFollowLength = followLength;
        followLength = 0;
        for (i = 0; i < noofnt; i++)
        {
            currentRule = G[i];
            while (currentRule != NULL)
            {
                currentRhs = currentRule->rule;
                while (currentRhs != NULL)
                {
                    if (currentRhs->isTerminal == false)
                    {
                        followingRhs = currentRhs->next;
                        while (1)
                        {
                            if (followingRhs == NULL)
                            {
                                for (j = 0; j < nooft; j++)
                                {
                                    if (Follow[i][j] == '1')
                                        Follow[currentRhs->type][j] = '1';
                                }
                                break;
                            }
                            else if (followingRhs->isTerminal && followingRhs->type != EPSILON)
                            {
                                Follow[currentRhs->type][followingRhs->type] = '1';
                                break;
                            }
                            else
                            {
                                for (j = 0; j < nooft; j++)
                                {
                                    if (First[followingRhs->type][j] == '1' && j != EPSILON)
                                        Follow[currentRhs->type][j] = '1';
                                }
                                if (First[followingRhs->type][EPSILON] == '0')
                                    break;
                                else
                                    followingRhs = followingRhs->next;
                            }
                        }
                    }
                    currentRhs = currentRhs->next;
                }
                currentRule = currentRule->next;
            }
            for (j = 0; j < nooft; j++)
            {
                if (Follow[i][j] == '1')
                    followLength++;
            }
        }
        //calculate followLength
    }
    // for (i = 0; i < noofnt; i++)
    // followDone[i] = '0';
    // followDone[mainFunction] = '1';
    // printf("%d\n", count);
}

void printFollow(char **Follow)
{
    printf("PRINTING FOLLOW SET\n");
    char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
    int i, j;
    for (i = 0; i < noofnt; i++)
    {
        printf("%s :\n", keys1[i]);
        for (j = 0; j < nooft; j++)
        {
            if (Follow[i][j] == '1')
                printf("\t %s", keys2[j]);
        }
        printf("\n");
    }
    return;
}

Rules **MakeParseTable(Grammer G, char **Follow, char **First)
{
    int i, j;
    Rules **parseTable = (Rules **)calloc(noofnt, sizeof(Rules *));
    Rules currentRule;
    Rhs currentRhs;
    for (i = 0; i < noofnt; i++)
    {
        parseTable[i] = (Rules *)calloc(nooft, sizeof(Rules));
        currentRule = G[i];
        while (currentRule != NULL)
        {
            currentRhs = currentRule->rule;
            while (1)
            {
                if (currentRhs == NULL)
                {
                    for (j = 0; j < nooft; j++)
                    {
                        if (Follow[i][j] == '1')
                            parseTable[i][j] = currentRule;
                    }
                    break;
                }
                else if (currentRhs->isTerminal && currentRhs->type != EPSILON)
                {
                    parseTable[i][currentRhs->type] = currentRule;
                    break;
                }
                else if (currentRhs->isTerminal && currentRhs->type == EPSILON)
                {
                }
                else
                {
                    for (j = 0; j < nooft; j++)
                    {
                        if (First[currentRhs->type][j] == '1')
                            parseTable[i][j] = currentRule;
                    }
                    if (First[currentRhs->type][EPSILON] == '0')
                        break;
                }
                currentRhs = currentRhs->next;
            }
            currentRule = currentRule->next;
        }
    }
    return parseTable;
}

void printParseTable(Rules **parseTable)
{
    printf("PRINTING PARSETABLE\n");
    char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
    int i, j;
    Rules currentRule;
    Rhs currentRhs;
    for (i = 0; i < noofnt; i++)
    {
        printf("%s :\n", keys1[i]);
        for (j = 0; j < nooft; j++)
        {
            currentRule = parseTable[i][j];
            if (currentRule != NULL)
            {
                printf("\t %s :", keys2[j]);
                currentRhs = currentRule->rule;
                printf("<%s> ==> ", keys1[i]);
                while (currentRhs != NULL)
                {
                    if (currentRhs->isTerminal)
                        printf("%s ", keys2[currentRhs->type]);
                    else
                        printf("<%s> ", keys1[currentRhs->type]);
                    currentRhs = currentRhs->next;
                }
                printf("\n");
            }
        }
    }
}

Node makeParseTree(Rules **parseTable, char **First, char **Follow)
{
    char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
    Stack stackH = createStack(100);
    Stack revStack = createStack(100);
    Rhs Start = (Rhs)calloc(1, sizeof(rhside));
    Rhs Finish = (Rhs)calloc(1, sizeof(rhside));
    Start->isTerminal = false;
    Start->type = mainFunction;
    Finish->isTerminal = true;
    Finish->type = FINISH;
    push(stackH, Finish);
    push(stackH, Start);
    Node root = new_node(Start, false);
    Node currentNode = root;
    Rhs stackPop, stackPush;
    Rules stackPushRule;
    tokenPtr t;
    // int flag;
    fp = getStream(fp, buf);
    while (fp != NULL || buf[bufIndex] != '\0' || isEmpty(stackH) != 0)
    {
        // printf("////////////////////////////\n");
        // PrintInorderTree(root);
        // printf("////////////////////////////\n");

        t = getNextToken();
        // printToken(t);
        //remove error and comments
        if (t->type == COMMENT || t->type == ERROR || t->type == NONE)
        {
            free(t);
            continue;
        }
        while (1)
        {
            if (!isEmpty(stackH))
            {
                stackPop = pop(stackH);
            }
            else
            {
                printf("EMPTY STACK\n");
                return NULL;
            }
            if (stackPop->isTerminal)
            {
                // printf("\nPOP %s\n", keys2[stackPop->type]);
                if (stackPop->type == FINISH)
                    return root;
                // printf("Token %s lineno %d\n", keys2[t->type], t->lineno);
                if (t->type != stackPop->type)
                {
                    // printf("ERROR!!1\n");
                    if (t->type == NUM)
                        printf("line no.: %d\t Syntax error: The token %s for lexeme <%d> does not match at line %d. The expected token here is %s\n", t->lineno, keys2[t->type], (int)t->value, t->lineno, keys2[stackPop->type]);
                    else if (t->type == RNUM)
                        printf("line no.: %d\t Syntax error: The token %s for lexeme <%.2f> does not match at line %d. The expected token here is %s\n", t->lineno, keys2[t->type], t->value, t->lineno, keys2[stackPop->type]);
                    else
                        printf("line no.: %d\t Syntax error: The token %s for lexeme <%s> does not match at line %d. The expected token here is %s\n", t->lineno, keys2[t->type], t->string, t->lineno, keys2[stackPop->type]);
                    flag = 1;
                    // // break;
                    continue;

                    // while (t->type != SEMICOLON)
                    // {
                    //     if (fp != NULL || buf[bufIndex] != '\0' || isEmpty(stackH) != 0)
                    //     {
                    //         t = getNextToken();
                    //         printToken(t);
                    //         if (t->type == COMMENT || t->type == ERROR || t->type == NONE)
                    //         {
                    //             free(t);
                    //             continue;
                    //         }
                    //     }
                    //     else
                    //     {
                    //         break;
                    //     }
                    // }
                    // if (!(stackPop->isTerminal) && stackPop->type == moreStmtAndFunctionDefs)
                    // {
                    //     push(stackH, stackPop);
                    // }
                    // else
                    // {
                    //     while (!(stackPop->isTerminal && stackPop->type == SEMICOLON))
                    //     {
                    //         if (!isEmpty(stackH))
                    //         {
                    //             printf("POP: ");
                    //             stackPop = pop(stackH);
                    //             if (stackPop->isTerminal)
                    //             {
                    //                 printf("%s\n", keys2[stackPop->type]);
                    //             }
                    //             else
                    //             {
                    //                 printf("%s\n", keys1[stackPop->type]);
                    //             }
                    //             currentNode = nextNT(currentNode);
                    //         }
                    //         else
                    //         {
                    //             break;
                    //         }
                    //     }
                    // }
                    // break;
                }
                else
                {
                    currentNode->data = t;
                    currentNode = nextNT(currentNode);
                    // free(t);
                    // free(stackPop);
                    break;
                }
            }
            else
            {
                // printf("\nPOP %s\n", keys1[stackPop->type]);
                // printf("Token %s lineno %d\n", keys2[t->type], t->lineno);
                stackPushRule = parseTable[stackPop->type][t->type];

                // free(stackPop);
                if (stackPushRule != NULL)
                {
                    {
                        // printf("\t %s :", keys1[stackPop->type]);
                        stackPush = stackPushRule->rule;
                        // printf("<%s> ==> ", keys1[stackPop->type]);
                        while (stackPush != NULL)
                        {
                            if (stackPush->isTerminal)
                            {
                                // printf("%s ", keys2[stackPush->type]);
                            }
                            else
                            {
                                // printf("<%s> ", keys1[stackPush->type]);
                            }
                            stackPush = stackPush->next;
                        }
                        // printf("\n");
                    }
                    stackPush = stackPushRule->rule;
                    while (stackPush != NULL && ((stackPush->type != EPSILON && stackPush->isTerminal) || (!stackPush->isTerminal)))
                    {
                        push(revStack, stackPush);
                        currentNode = add_child(currentNode, stackPush, stackPush->isTerminal);
                        stackPush = stackPush->next;
                    }
                    if (stackPush != NULL && (stackPush->type == EPSILON && stackPush->isTerminal))
                    {
                        currentNode = add_child(currentNode, stackPush, stackPush->isTerminal);
                        currentNode = currentNode->child;
                        // currentNode->data = stackPush;
                        // currentNode->isterminal = true;
                        currentNode = nextNT(currentNode);
                    }
                    else
                    {
                        currentNode = currentNode->child;
                        while (!isEmpty(revStack))
                        {
                            push(stackH, pop(revStack));
                        }
                    }
                }
                else
                {
                    // printf("ERROR!!2\n");
                    // flag = 0;
                    printf("line no.: %d\t Syntax error: Skiping Parsing Till SEMICOLON on line ", t->lineno);
                    flag = 1;
                    while (t->type != SEMICOLON)
                    {
                        if (fp != NULL || buf[bufIndex] != '\0' || isEmpty(stackH) != 0)
                        {
                            t = getNextToken();
                            // printToken(t);
                            if (t->type == COMMENT || t->type == ERROR || t->type == NONE)
                            {
                                free(t);
                                continue;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (!(stackPop->isTerminal) && stackPop->type == moreStmtAndFunctionDefs)
                    {
                        push(stackH, stackPop);
                    }
                    else
                    {
                        while (!(stackPop->isTerminal && stackPop->type == SEMICOLON))
                        {
                            if (!isEmpty(stackH))
                            {
                                // printf("POP: ");
                                stackPop = pop(stackH);
                                // if (stackPop->isTerminal)
                                // {
                                //     printf("%s\n", keys2[stackPop->type]);
                                // }
                                // else
                                // {
                                //     printf("%s\n", keys1[stackPop->type]);
                                // }
                                currentNode = nextNT(currentNode);
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    printf("%d\n", t->lineno);
                    break;
                    // while (1)
                    // {
                    //     t = getNextToken();
                    //     if (t->type == COMMENT || t->type == ERROR || t->type == NONE)
                    //     {
                    //         free(t);
                    //         continue;
                    //     }
                    //     if (t->type == FINISH)
                    //     {
                    //         break;
                    //     }
                    //     if (Follow[stackPop->type][t->type] == '1')
                    //     {
                    //         // flag = 1;
                    //         break;
                    //     }
                    //     else if (First[stackPop->type][t->type] == '1')
                    //     {
                    //         stackPushRule = parseTable[stackPop->type][t->type];

                    //         // free(stackPop);
                    //         if (stackPushRule != NULL)
                    //         {
                    //             stackPush = stackPushRule->rule;
                    //             while (stackPush != NULL && ((stackPush->type != EPSILON && stackPush->isTerminal) || (!stackPush->isTerminal)))
                    //             {
                    //                 push(revStack, stackPush);
                    //                 currentNode = add_child(currentNode, stackPush, stackPush->isTerminal);
                    //                 stackPush = stackPush->next;
                    //             }
                    //             if (stackPush != NULL && (stackPush->type == EPSILON && stackPush->isTerminal))
                    //             {
                    //                 currentNode->data = stackPush;
                    //                 currentNode->isterminal = true;
                    //                 currentNode = nextNT(currentNode);
                    //             }
                    //             else
                    //             {
                    //                 currentNode = currentNode->child;
                    //                 while (!isEmpty(revStack))
                    //                 {
                    //                     push(stackH, pop(revStack));
                    //                 }
                    //             }
                    //         }
                    //     }
                    //     else if (First[stackPop->type][EPSILON] = '1')
                    //     {
                    //         // if (!isEmpty(stackH))
                    //         // {
                    //         //     stackPop = pop(stackH);
                    //         // }
                    //         break;
                    //     }
                    // }
                    // if (flag == 1)
                    // continue;
                    // break;
                }
            }
        }
    }
    return root;
};