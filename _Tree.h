// SAMIP JASANI 2015A7PS0127P
#ifndef TREE_H
#define TREE_H
#include <stdbool.h>
#include <stdlib.h>
typedef struct node node;

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
#endif