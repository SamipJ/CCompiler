// SAMIP JASANI 2015A7PS0127P

#define ALPHABET_SIZE (26)
#include "lexer.h"
#include <stdbool.h>
extern const char keys1[][30];
extern const char keys2[][15];
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    enum tokenType type;
    // isEndOfWord is true if the node represents
    // end of a word
    bool isEndOfWord;
};

struct TrieNode *getNode(void);
void insert(struct TrieNode *root, const char *key, int type);
int search(struct TrieNode *root, const char *key);
int searchforgrammer(struct TrieNode *root, const char *key);