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
    head->input = NULL;
    head->output = NULL;
    head->lineno = 0;
    return head;
}

void addInput(htHead head, enum tokenType type, char *id)
{
    parNode parameter = (parNode)malloc(sizeof(parnode));
    parameter->type = type;
    parameter->id = id;
    parameter->next = NULL;
    parNode temp = head->input;
    if (temp == NULL)
        head->input = parameter;
    else
    {
        while (temp->next)
        {
            if (strcmp(temp->id, parameter->id) == 0)
            {
                printf("ERROR(D) in %s\n", temp->id);
                return;
            }
            temp = temp->next;
        }
        if (strcmp(temp->id, parameter->id) == 0)
        {
            printf("ERROR(D) in %s\n", temp->id);
            return;
        }
        else
        {
            temp->next = parameter;
        }
    }
    return;
}

void addOutput(htHead head, enum tokenType type, char *id)
{
    parNode parameter = (parNode)malloc(sizeof(parnode));
    parameter->type = type;
    parameter->id = id;
    parameter->next = NULL;
    parNode temp = head->output;
    if (temp == NULL)
        head->output = parameter;
    else
    {
        while (temp->next)
        {
            if (strcmp(temp->id, parameter->id) == 0)
            {
                printf("ERROR(E) in %s\n", temp->id);
                return;
            }
            temp = temp->next;
        }
        if (strcmp(temp->id, parameter->id) == 0)
        {
            printf("ERROR(E) in %s\n", temp->id);
            return;
        }
        else
        {
            temp->next = parameter;
        }
    }
    return;
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
    temp->x = x;
    temp->y = y;
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

Node findFunc(char *funid, Node stRoot)
{
    Node child = stRoot->child;
    while (child)
    {
        if (strcmp(funid, ((htHead)child->data)->scope) == 0)
            return child;
        else
            child = child->sibling;
    }
    return NULL;
}

sizeptr returnSize(Node stRoot, tokenPtr token)
{
    char *id = token->string;
    int h = myhash(id);
    hashTable temp = ((htHead)stRoot->data)->ht[h];
    while (temp)
    {
        if (strcmp(temp->id, id) == 0)
        {
            sizeptr size1 = (sizeptr)malloc(sizeof(sizenode));
            size1->x = temp->x;
            size1->y = temp->y;
            return size1;
        }
        temp = temp->next;
    }
    if (stRoot->parent)
        return (returnSize(stRoot->parent, token));
    else
    {
        return NULL;
    }
}
void setInitialised(Node stRoot, tokenPtr token)
{
    char *id = token->string;
    int h = myhash(id);
    hashTable temp = ((htHead)stRoot->data)->ht[h];
    while (temp)
    {
        if (strcmp(temp->id, id) == 0)
        {
            temp->initialised = 1;
            return;
        }
        temp = temp->next;
    }
    if (stRoot->parent)
        setInitialised(stRoot->parent, token);
    else
    {
        return;
    }
}