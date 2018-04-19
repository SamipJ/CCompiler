// SAMIP JASANI 2015A7PS0127P
#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
typedef struct node node;
extern const char keys1[][30];
extern const char keys2[][15];
typedef node *Node;

struct node
{
    bool isterminal;
    void *data;
    Node child;
    Node parent;
    Node sibling;
};
Node new_node(void *ptr, bool isterminal);
Node add_sibling(Node n, void *ptr, bool isterminal);
Node add_child(Node n, void *ptr, bool isterminal);
Node nextNT(Node n);
void PrintInorderTree(Node n);
void FileInorderTree(Node n, FILE *fp);
#endif