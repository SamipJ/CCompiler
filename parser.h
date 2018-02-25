// SAMIP JASANI 2015A7PS0127P
#ifndef PARSER_H
#define PARSER_H
#include "parserDef.h"
Grammer readGrammer(char *fname);
void printGrammer(Grammer G);
struct TrieNode *FillTrieforGrammer();
#endif