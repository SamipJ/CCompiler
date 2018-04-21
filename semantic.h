// SAMIP JASANI 2015A7PS0127P

#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "symbolTable.h"

typedef struct errorhead errorhead;
typedef errorhead *errorHead;
typedef struct errornode errornode;
typedef errornode *errorNode;

struct errorhead
{
    errorNode first;
    errorNode last;
};

struct errornode
{
    int errorinfo;
    errorNode next;
};

sizeptr computeSize(Node stRoot, Node astRoot, int type);
sizeptr returnSize(Node stRoot, tokenPtr token);
int checkType(Node stRoot, Node astRoot);
int typeAE(Node stRoot, Node astRoot);
int checkDeclaration(Node stRoot, tokenPtr token);
int checkInitialization(Node stRoot);
void compareParameters(Node stRoot, parNode input, Node inputAST, Node till);
int checkOverloading(char *funcid, int lineno, Node stRoot);
int checkScopeDeclaration(Node stRoot, tokenPtr token);

#endif