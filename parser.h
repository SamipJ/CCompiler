// SAMIP JASANI 2015A7PS0127P
#ifndef PARSER_H
#define PARSER_H
#include "parserDef.h"
#include <stdio.h>
#include "_Tree.h"
Grammer readGrammer(char *fname);
void printGrammer(Grammer G);
extern const char keys2[][15];
extern const char keys1[][30];

char **InitialiseFirst();
char **InitialiseFollow();
struct TrieNode *FillTrieforGrammer();
void MakeFirst(Grammer G, char **First);
void FirstDAC(Grammer G, char **First, char *firstDone, int key);
void printFirst(char **First);
void MakeFollow(Grammer G, char **Follow, char **First);
void printFollow(char **Follow);
Rules **MakeParseTable(Grammer G, char **Follow, char **First);
void printParseTable(Rules **parseTable);
Node makeParseTree(Rules **parseTable, char **First, char **Follow);
#endif