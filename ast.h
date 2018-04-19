// SAMIP JASANI 2015A7PS0127P
#ifndef AST_H
#define AST_H
#include "astDef.h"
#include "_Tree.h"
extern const char keys1[][30];
extern const char keys2[][15];
// extern const char keys1[][45];
// extern const char keys2[][43];

Node makeAST(Node parseTreeNode, Node parentAST);
void rebalanceMatrixRows(Node rows);
void rebalanceMatrixRow(Node row);

#endif