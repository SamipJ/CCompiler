// SAMIP JASANI 2015A7PS0127P
#ifndef LEXER_H
#define LEXER_H
#include "lexerDef.h"
#include <stdio.h>
FILE *getStream(FILE *fp, char *buf);
tokenPtr getNextToken();
void printToken(tokenPtr t);
struct TrieNode *FillTrie();
void CommentFree();
extern const char keys1[][30];
extern const char keys2[][15];
// void printToken(tokenPtr t);
#endif