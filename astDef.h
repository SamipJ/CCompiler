// SAMIP JASANI 2015A7PS0127P
#ifndef ASTDEF_H
#define ASTDEF_H
#include "parserDef.h"

extern const char keys1[][30];
extern const char keys2[][15];
typedef struct astnode
{
    int type;
    bool isImp; //something like MAIN,+ else declstmt
} astnode;
typedef astnode *astNode;

#endif