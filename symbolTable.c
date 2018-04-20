#include "symbolTable.h"
// #include "_HashTable.h"

Node makeST(Node astRoot, Node stRoot)
{
    // Node stRoot = NULL;
    if (!astRoot->isterminal)
    {
        if (((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == MAIN)
        {
            htHead ht = createEmptyHT(keys2[MAIN], 1, NULL);
            stRoot = new_node(ht, false);
            Node child = astRoot->child;
            while (child)
            {
                makeST(child, stRoot);
                child = child->sibling;
            }
        }
        else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == functionDef)
        {
            //check for recursion overloading
            htHead ht = createEmptyHT(keys2[FUNCTION], ((htHead)stRoot->data)->nesting_level + 1, ((htHead)stRoot->data)->scope);
            stRoot = add_child(stRoot, ht, false);
            int funcset = 0;
            Node child = astRoot->child;
            while (child)
            {
                if (funcset == 0)
                {
                    if (child->isterminal && ((tokenPtr)child->data)->type == FUNID)
                    {
                        funcset = 1;
                        ((htHead)stRoot->data)->scope = ((tokenPtr)child->data)->string;
                        child = child->sibling;
                    }
                    else
                    {
                        enum tokenType type = ((astNode)child->data)->type;
                        child = child->sibling;
                        char *id = ((tokenPtr)child->data)->string;
                        insertInHT(createHTNode(id, type, 0), stRoot->data);
                        child = child->sibling;
                    }
                }
                else
                {
                    if (((astNode)child->data)->isImp && (((astNode)child->data)->type == INT || ((astNode)child->data)->type == REAL || ((astNode)child->data)->type == STRING || ((astNode)child->data)->type == MATRIX))
                    {
                        enum tokenType type = ((astNode)child->data)->type;
                        child = child->sibling;
                        char *id = ((tokenPtr)child->data)->string;
                        insertInHT(createHTNode(id, type, 1), stRoot->data);
                        child = child->sibling;
                    }
                    else
                    {
                        makeST(child, stRoot);
                        child = child->sibling;
                    }
                }
            }
            //check that all variables are used
        }
        else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == declarationStmt)
        {
            Node child = astRoot->child;
            enum tokenType type = ((astNode)child->data)->type;
            child = child->sibling;
            while (child)
            {
                char *id = ((tokenPtr)child->data)->string;
                insertInHT(createHTNode(id, type, 1), stRoot->data);
                child = child->sibling;
            }
        }
        // else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == assignmentStmt)
        // {
        //     Node child = astRoot->child;
        //     char *id = ((tokenPtr)child->data)->string;
        //     int check = checkST(stRoot, id);
        //     if (check != -1)
        //     {
        //         if (check == MATRIX || check == STRING)
        //         {
        //         }
        //     }
        // }
        }

    return stRoot;
}

void printST(Node root)
{
    if (root)
    {
        printHT(root->data);
    }
    Node child = root->child;
    while (child)
    {
        printHT(child->data);
        child = child->sibling;
    }
}

int checkST(Node stRoot, char *id)
{
    int check = -1;
    if ((check = checkHT(stRoot->data, id)) != -1)
        return check;
    else if (stRoot->parent)
        return (checkST(stRoot->parent, id));
    else
        return check;
}