// SAMIP JASANI 2015A7PS0127P
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "symbolTableDef.h"
#include "ast.h"
#include "_HashTable.h"
#include "semantic.h"
Node makeST(Node astRoot, Node stRoot);
void printST(Node root);
int checkST(Node stRoot, char *id);

#endif