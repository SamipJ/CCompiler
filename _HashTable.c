// SAMIP JASANI 2015A7PS0127P
#include "_HashTable.h"

int myhash(char *str)
{
    unsigned long myhash = 5381;
    int c;
    while (c = *str++)
        myhash = ((myhash << 5) + myhash) + c; /* myhash * 33 + c */
    return (int)(myhash % M);
}

htHead createEmptyHT(const char *scope, int nesting_level, const char *parentscope)
{
    htHead head = (htHead)malloc(sizeof(hthead));
    head->scope = scope;
    head->nesting_level = nesting_level;
    head->parentscope = parentscope;
    head->currentoffset = 0;
    head->ht = (hashTable *)calloc(M, sizeof(hashTable));
    return head;
}

hashTable createHTNode(char *id, enum tokenType type, int initialised)
{
    hashTable htnode = (hashTable)calloc(1, sizeof(hashtable));
    htnode->id = id;
    htnode->type = type;
    htnode->initialised = initialised;
    // htnode->x = x;
    // htnode->y = y;
    switch (htnode->type)
    {
    case INT:
        htnode->width = 2;
        break;
    case REAL:
        htnode->width = 4;
        break;
        // case STRING:
        // htnode->width;
        // break;
        // case MATRIX:
        // htnode->width = 2 * x * y;
        // break;
    }
    return htnode;
}

int checkHT(htHead head, char *id)
{
    int found = -1, h = myhash(id);
    hashTable temp = head->ht[h];
    while (temp)
    {
        if (strcmp(temp->id, id) == 0)
        {
            found = temp->type;
            break;
        }
        temp = temp->next;
    }
    return found;
}

void insertInHT(hashTable htnode, htHead head)
{
    char *id = htnode->id;
    int h = myhash(id);
    hashTable prev, temp = head->ht[h];
    if (temp == NULL)
    {
        head->ht[h] = htnode;
        htnode->offset = head->currentoffset;
        head->currentoffset += htnode->width;
    }
    else
    {
        prev = temp;
        temp = temp->next;
        while (temp)
        {
            prev = temp;
            temp = temp->next;
        }
        prev->next = htnode;
        htnode->offset = head->currentoffset;
        head->currentoffset += htnode->width;
    }
}

void assignwidth(htHead head, char *id, int x, int y)
{
    int found = 0, h = myhash(id);
    hashTable temp = head->ht[h];
    while (temp)
    {
        if (strcmp(temp->id, id) == 0)
        {
            found = 1;
            if (temp->type == STRING)
                temp->width = x;
            else
                temp->width = x * y * 2;
            temp->offset = head->currentoffset;
            head->currentoffset += temp->width;
            break;
        }
        temp = temp->next;
    }
    if (found == 0)
        exit(-1);
    return;
}

void printHT(htHead head)
{
    printf("Scope: %s\tNesting Level: %d\tParent Scope:%s\n\n", head->scope, head->nesting_level, head->parentscope);
    int i;
    hashTable temp;
    for (i = 0; i < M; i++)
    {
        temp = head->ht[i];
        while (temp)
        {
            printf("%s\t%s\t%d\t%d\n", temp->id, keys2[temp->type], temp->width, temp->offset);
            temp = temp->next;
        }
    }
    return;
}