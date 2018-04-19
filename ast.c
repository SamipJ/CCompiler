// SAMIP JASANI 2015A7PS0127P
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"
#include "_Tree.h"

Node makeAST(Node parseTreeNode, Node parentAST)
{
    Node root = NULL;                  // = (Node)calloc(1, sizeof(node));
    if (parseTreeNode->parent == NULL) //root of parsetree
    {
        root = (Node)calloc(1, sizeof(node));
        root->isterminal = false;
        root->data = calloc(1, sizeof(rhside));
        ((Rhs)root->data)->type = ROOT;
        ((Rhs)root->data)->isTerminal = false;
        Node child = parseTreeNode->child;
        while (child)
        {
            makeAST(child, root);
            child = child->sibling;
        }
    }

    return root;
}