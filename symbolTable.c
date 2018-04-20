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
                        //check if function is not overloaded;
                        int check = checkOverloading(((tokenPtr)child->data)->string, ((tokenPtr)child->data)->lineno, stRoot);
                        if (check == 1)
                        {
                            //free a components of hthead
                            Node temp = stRoot->parent->child;
                            while (temp->sibling)
                            {
                                if (temp->sibling == stRoot)
                                {
                                    temp->sibling = NULL;
                                    break;
                                }
                                temp = temp->sibling;
                            }
                            free(stRoot);
                            return NULL;
                        }
                        else
                        {
                            ((htHead)stRoot->data)->scope = ((tokenPtr)child->data)->string;
                            child = child->sibling;
                        }
                    }
                    else
                    {
                        enum tokenType type = ((astNode)child->data)->type;
                        child = child->sibling;
                        char *id = ((tokenPtr)child->data)->string;
                        addOutput(stRoot->data, type, id);
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
                        addInput(stRoot->data, type, id);
                        insertInHT(createHTNode(id, type, 1), stRoot->data);
                        child = child->sibling;
                    }
                    else if (child->isterminal && !((astNode)child->data)->isImp && (((astNode)child->data)->type == END))
                    {
                        ((htHead)stRoot->data)->lineno = ((tokenPtr)child->data)->lineno;
                        checkInitialization(stRoot);
                        return stRoot;
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
                int check = checkDeclaration(stRoot, (tokenPtr)child->data);
                if (check == ERROR)
                    insertInHT(createHTNode(id, type, 1), stRoot->data);
                else
                {
                    printf("ERROR(A) in line %d\n", ((tokenPtr)child->data)->lineno);
                }
                child = child->sibling;
            }
        }
        else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == assignmentStmt)
        {
            int check = checkType(stRoot, astRoot);
            if (check == 1)
            {
                int lhs = checkDeclaration(stRoot, ((tokenPtr)astRoot->child->data));

                if (lhs == MATRIX)
                {
                    sizeptr temp = computeSize(stRoot, astRoot->child->sibling, MATRIX);
                    if (temp)
                    {
                        assignwidth(stRoot->data, ((tokenPtr)astRoot->child->data)->string, temp->x, temp->y);
                        setInitialised(stRoot, (tokenPtr)astRoot->child->data);
                        free(temp);
                    }
                }
                else if (lhs == STRING)
                {
                    sizeptr temp = computeSize(stRoot, astRoot->child->sibling, STRING);
                    assignwidth(stRoot->data, ((tokenPtr)astRoot->child->data)->string, temp->x, temp->y);
                    setInitialised(stRoot, (tokenPtr)astRoot->child->data);
                    free(temp);
                }
                else
                {
                    if (strcmp("u", ((tokenPtr)astRoot->child->data)->string) == 0)
                    {
                        printf("hello\n");
                    }
                    setInitialised(stRoot, (tokenPtr)astRoot->child->data);
                }
            }
        }
        else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == funCallStmt)
        {
            Node child = astRoot->child;
            while (child)
            {
                if (child->isterminal && ((tokenPtr)child->data)->type == FUNID)
                {
                    break;
                }
                else
                {
                    child = child->sibling;
                }
            }
            Node temp = findFunc(((tokenPtr)child->data)->string, stRoot);
            if (temp)
            {
                parNode input = ((htHead)temp->data)->input;
                parNode output = ((htHead)temp->data)->output;
                Node outputAST = astRoot->child;
                Node inputAST = child->sibling;
                compareParameters(stRoot, output, outputAST, NULL);
                compareParameters(stRoot, input, inputAST, child);
                outputAST = astRoot->child;
                while (outputAST != child)
                {
                    setInitialised(stRoot, (tokenPtr)outputAST->data);
                    outputAST = outputAST->sibling;
                }
            }
            else
                (printf("ERROR(G) in line %d\n", ((tokenPtr)child->data)->lineno));
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
        printST(child);
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