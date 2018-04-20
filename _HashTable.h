// SAMIP JASANI 2015A7PS0127P
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "ast.h"
#include "string.h"
#define M 100

typedef struct hashtable hashtable;
typedef hashtable *hashTable;

struct hashtable
{
    char *id;
    enum tokenType type;
    int width;
    int offset;
    int x; //for matrix
    int y; //for matrix
    int initialised;
    hashTable next;
};

typedef struct hthead
{
    const char *scope;
    int nesting_level;
    const char *parentscope;
    int currentoffset;
    hashTable *ht;
} hthead;

typedef hthead *htHead;

htHead createEmptyHT(const char *scope, int nesting_level, const char *parentscope);
hashTable createHTNode(char *id, enum tokenType type, int initialised);
int checkHT(htHead head, char *id);
void insertInHT(hashTable htnode, htHead head);
void assignwidth(htHead head, char *id, int x, int y);
void printHT(htHead head);

#endif