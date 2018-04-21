// SAMIP JASANI 2015A7PS0127P
#include "symbolTable.h"
// #include "_HashTable.h"
extern int flag;
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
            int lineno = ((tokenPtr)astRoot->child->sibling->data)->lineno;
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
                        int check = checkScopeDeclaration(stRoot, (tokenPtr)child->data);
                        addOutput(stRoot->data, type, id, lineno);
                        if (check == ERROR)
                        {
                            insertInHT(createHTNode(id, type, 0), stRoot->data);
                        }
                        else
                        {
                            // printf("ERROR(A) in line %d\n", ((tokenPtr)child->data)->lineno);
                        }
                        // insertInHT(createHTNode(id, type, 0), stRoot->data);
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
                        addInput(stRoot->data, type, id, lineno);
                        int check = checkScopeDeclaration(stRoot, (tokenPtr)child->data);
                        if (check == ERROR)
                            insertInHT(createHTNode(id, type, 1), stRoot->data);
                        else
                        {
                            // printf("ERROR(A) in line %d\n", ((tokenPtr)child->data)->lineno);
                        }
                        // insertInHT(createHTNode(id, type, 1), stRoot->data);
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
                int check = checkScopeDeclaration(stRoot, (tokenPtr)child->data);
                if (check == ERROR)
                    insertInHT(createHTNode(id, type, 1), stRoot->data);
                else
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error: Redeclaration of ID \"%s\" \n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
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
                    // if (strcmp("u", ((tokenPtr)astRoot->child->data)->string) == 0)
                    // {
                    //     printf("hello\n");
                    // }
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
            Node temp = findFunc(((tokenPtr)child->data)->string, ((tokenPtr)child->data)->lineno, stRoot);
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
            {
                flag = 1;
                printf("line no.: %d\t Semantic error: Function \"%s\" not defined in current scope \n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
            }
        }
        else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == sizeStmt)
        {
            int count = 0;
            Node child = astRoot->child;
            while (1)
            {
                if (((astNode)child->data)->isImp && ((astNode)child->data)->type == SIZE)
                    break;
                count++;
                if (checkDeclaration(stRoot, child->data) != INT)
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error: \"%s\" should be of type INT \n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
                }
                child = child->sibling;
            }
            child = child->sibling;
            if (count == 2)
            {
                if (checkDeclaration(stRoot, child->data) != MATRIX)
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error:\"%s\" should be of type MATRIX or LHS should have only 1 parameter\n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
                }
            }
            else if (count == 1)
            {
                if (checkDeclaration(stRoot, child->data) != STRING)
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error:\"%s\" should be of type STRING or LHS should have 2 parameters\n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
                }
            }
            else
            {
                if (checkDeclaration(stRoot, child->data) == STRING)
                {
                    flag = 1;
                    printf("line no.: %d\tSemantic error: \"%s\" is of type STRING so LHS should have 1 parameters\n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
                }
                else if (checkDeclaration(stRoot, child->data) == MATRIX)
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error:\"%s\" is of type MATRIX so LHS should have 2 parameters\n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
                }
                else
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error:\"%s\" is of type \"%s\" so  RHS should be STRING(LHS: 1 parameter) or MATRIX(LHS:2 parameters)\n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string, keys2[((tokenPtr)child->data)->type]);
                }
            }
        }
        else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == ioStmt)
        {
            int type = ((tokenPtr)astRoot->child->data)->type;
            Node child = astRoot->child->sibling;
            if (type == READ)
            {
                int datatype = checkDeclaration(stRoot, child->data);
                if (datatype == ERROR)
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error: The ID \"%s\" is not declared \n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
                }
                else if (datatype == INT || datatype == REAL)
                {
                    return stRoot;
                }
                else
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error: %s is of type %s, only INT and REAL can be READ\n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string, keys2[checkDeclaration(stRoot, child->data)]);
                }
            }
            else
            {
                int datatype = checkDeclaration(stRoot, child->data);
                if (datatype == ERROR)
                {
                    flag = 1;
                    printf("line no.: %d\t Semantic error: The ID \"%s\" is not declared \n", ((tokenPtr)child->data)->lineno, ((tokenPtr)child->data)->string);
                }
                return stRoot;
            }
        }
        else if (!((astNode)astRoot->data)->isImp && ((astNode)astRoot->data)->type == conditionalStmt)
        {
            Node child = astRoot->child;
            if (typeAE(stRoot, child) != BOOL)
            {
                Node temp = child;
                while (!temp->isterminal)
                {
                    temp = temp->child;
                }
                flag = 1;
                printf("line no.: %d\t Semantic error: BOOLEAN Expression incorrect\n", ((tokenPtr)temp->data)->lineno);
            }
            child = child->sibling;
            while (child && !(!child->isterminal && !((astNode)child->data)->isImp && ((astNode)child->data)->type == elseStmt))
            {
                makeST(child, stRoot);
                child = child->sibling;
            }
            if (child == NULL)
            {
                return stRoot;
            }
            child = child->child;
            while (child)
            {
                makeST(child, stRoot);
                child = child->sibling;
            }
        }
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