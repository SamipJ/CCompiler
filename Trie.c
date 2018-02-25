// SAMIP JASANI 2015A7PS0127P

// C implementation of search and insert operations
// on Trie
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Trie.h"
#include "lexerDef.h"
#include <ctype.h>
#include "parserDef.h"

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

// Alphabet size (# of symbols)
#define ALPHABET_SIZE (26)

// Converts key current character into index
// use only 'a' through 'z' and lower case
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// trie node

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
    struct TrieNode *pNode = NULL;

    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));

    if (pNode)
    {
        int i;

        pNode->isEndOfWord = false;
        pNode->type = 0;

        for (i = 0; i < ALPHABET_SIZE; i++)
            pNode->children[i] = NULL;
    }

    return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void insert(struct TrieNode *root, const char *key, int type)
{
    int level;
    int length = strlen(key);
    int index;

    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(tolower(key[level]));
        if (!pCrawl->children[index])
            pCrawl->children[index] = getNode();

        pCrawl = pCrawl->children[index];
    }

    // mark last node as leaf
    pCrawl->type = type;
    pCrawl->isEndOfWord = true;
}

// Returns true if key presents in trie, else false
int search(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        if (isupper(key[level]))
        {
            return 0;
        }
        index = CHAR_TO_INDEX(key[level]);

        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    if (pCrawl != NULL && pCrawl->isEndOfWord)
    {
        return pCrawl->type;
    }
    else
    {
        return 0;
    }
}

int searchforgrammer(struct TrieNode *root, const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *pCrawl = root;

    for (level = 0; level < length; level++)
    {
        // if (isupper(key[level]))
        // {
        //     return 0;
        // }
        // if (isalpha(key[level]) == 0)
        // {
        //     return 0;
        // }
        index = CHAR_TO_INDEX(tolower(key[level]));

        if (!pCrawl->children[index])
            return 0;

        pCrawl = pCrawl->children[index];
    }

    if (pCrawl != NULL && pCrawl->isEndOfWord)
    {
        return pCrawl->type;
    }
    else
    {
        return 0;
    }
}

// Driver
// int main()
// {
//     // Input keys (use only 'a' through 'z' and lower case)
//     char keys[][8] = {"the", "a", "there", "answer", "any",
//                       "by", "bye", "their"};
//     int type[8] = {1,
//                    2,
//                    3,
//                    4,
//                    5,
//                    6,
//                    7,
//                    8};
//     // char output[][32] = {"Not present in trie", "Present in trie"};

//     struct TrieNode *root = getNode();

//     // Construct trie
//     int i;
//     for (i = 0; i < ARRAY_SIZE(keys); i++)
//         insert(root, keys[i], type[i]);

//     // Search for different keys
//     printf("%d\n", search(root, "the"));
//     printf("%d\n", search(root, "these"));
//     printf("%d\n", search(root, "their"));
//     printf("%d\n", search(root, "thaw"));

//     return 0;
// }
struct TrieNode *FillTrieforGrammer()
{
    char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    // char keys[][11] = {"end", "int", "real", "string", "matrix",
    //    "if", "else", "endif", "read", "print", "function"};
    int type1[44] = {
        mainFunction,
        stmtsAndFunctionDefs,
        moreStmtAndFunctionDefs,
        stmtOrFunctionDef,
        stmt,
        functionDef,
        parameterList,
        typevar,
        remainingList,
        declarationStmt,
        varList,
        moreIds,
        assignFuncCallSizeStmt,
        funcCallSizeStmt,
        sizeStmt,
        conditionalStmt,
        otherStmts,
        elseStmt,
        ioStmt,
        funCallStmt,
        emptyOrInputParameterList,
        inputParameterList,
        listVar,
        assignmentStmt,
        arithmeticExpression,
        arithmeticExpression1,
        arithmeticExpression2,
        arithmeticExpression3,
        varExpression,
        operatorplusminus,
        operatormuldiv,
        booleanExpression,
        booleanExpression2,
        moreBooleanExpression,
        constrainedVars,
        matrixVar,
        matrixRows,
        matrixRows1,
        matrixRow,
        matrixRow1,
        var,
        matrixElement,
        logicalOp,
        relationalOp};
    int i;
    struct TrieNode *root = getNode();

    for (i = 0; i < ARRAY_SIZE(keys1); i++)
        insert(root, keys1[i], type1[i]);

    char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
    int type2[43] = {
        NONE,
        ERROR,
        ASSIGNOP,
        COMMENT,
        FUNID,
        ID,
        NUM,
        RNUM,
        STR,
        END,
        INT,
        REAL,
        STRING,
        MATRIX,
        MAIN,
        SQO,
        SQC,
        OP,
        CL,
        SEMICOLON,
        COMMA,
        IF,
        ELSE,
        ENDIF,
        READ,
        PRINT,
        FUNCTION,
        PLUS,
        MINUS,
        MUL,
        DIV,
        SIZE,
        AND,
        OR,
        NOT,
        LT,
        LE,
        EQ,
        GT,
        GE,
        NE,
        EPSILON,
        FINISH};
    for (i = 0; i < ARRAY_SIZE(keys2); i++)
        insert(root, keys2[i], type2[i]);
    return root;
}