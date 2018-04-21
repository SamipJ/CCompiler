// SAMIP JASANI 2015A7PS0127P

#include "semantic.h"

extern int flag;
//check if declared
int checkDeclaration(Node stRoot, tokenPtr token)
{
    char *id = token->string;
    int check = -1;
    if ((check = checkHT(stRoot->data, id)) != -1)
        return check;
    else if (stRoot->parent)
        return (checkDeclaration(stRoot->parent, token));
    else
    {
        // error generation
        // int lineno = token->lineno;
        // int type = token->type;
        return ERROR;
    }
}

int checkScopeDeclaration(Node stRoot, tokenPtr token)
{
    char *id = token->string;
    int check = -1;
    if ((check = checkHT(stRoot->data, id)) != -1)
        return check;
    else
    {
        // error generation
        // int lineno = token->lineno;
        // int type = token->type;
        return ERROR;
    }
}

int typeAE(Node stRoot, Node astRoot)
{
    if (!astRoot->isterminal)
    {
        astNode curnode = (astNode)astRoot->data;
        if (curnode->isImp)
        {
            switch (curnode->type)
            {
            case PLUS:
                if (typeAE(stRoot, astRoot->child) == INT && typeAE(stRoot, astRoot->child->sibling) == INT)
                    return INT;
                else if (typeAE(stRoot, astRoot->child) == REAL && typeAE(stRoot, astRoot->child->sibling) == REAL)
                    return REAL;
                else if (typeAE(stRoot, astRoot->child) == STRING && typeAE(stRoot, astRoot->child->sibling) == STRING)
                    return STRING;
                else if (typeAE(stRoot, astRoot->child) == MATRIX && typeAE(stRoot, astRoot->child->sibling) == MATRIX)
                    return MATRIX;
                else
                    return ERROR;
                break;
            case MINUS:
                if (typeAE(stRoot, astRoot->child) == INT && typeAE(stRoot, astRoot->child->sibling) == INT)
                    return INT;
                else if (typeAE(stRoot, astRoot->child) == REAL && typeAE(stRoot, astRoot->child->sibling) == REAL)
                    return REAL;
                else if (typeAE(stRoot, astRoot->child) == MATRIX && typeAE(stRoot, astRoot->child->sibling) == MATRIX)
                    return MATRIX;
                else
                    return ERROR;
                break;
            case MUL:
                if (typeAE(stRoot, astRoot->child) == INT && typeAE(stRoot, astRoot->child->sibling) == INT)
                    return INT;
                else if (typeAE(stRoot, astRoot->child) == REAL && typeAE(stRoot, astRoot->child->sibling) == REAL)
                    return REAL;
                else
                    return ERROR;
                break;
            case DIV:
                if (typeAE(stRoot, astRoot->child) == INT && typeAE(stRoot, astRoot->child->sibling) == INT)
                    return REAL;
                else if (typeAE(stRoot, astRoot->child) == INT && typeAE(stRoot, astRoot->child->sibling) == INT)
                    return REAL;
                else
                    return ERROR;
                break;
            case AND:
            case OR:
                if (typeAE(stRoot, astRoot->child) == BOOL && typeAE(stRoot, astRoot->child->sibling) == BOOL)
                    return BOOL;
                else
                    return ERROR;
                break;
            case NOT:
                if (typeAE(stRoot, astRoot->child) == BOOL)
                    return BOOL;
                else
                    return ERROR;
                break;
            case LT:
            case LE:
            case EQ:
            case GT:
            case GE:
            case NE:
                if (typeAE(stRoot, astRoot->child) == INT && typeAE(stRoot, astRoot->child->sibling) == INT)
                    return BOOL;
                else if (typeAE(stRoot, astRoot->child) == REAL && typeAE(stRoot, astRoot->child->sibling) == REAL)
                    return BOOL;
                else
                    return ERROR;
                break;
            }
        }
        else if (curnode->type == matrixElement)
        {
            //check if it is declared
            //check type
            int check = checkDeclaration(stRoot, astRoot->child->data);
            if (check == MATRIX)
            {
                return INT;
            }
            else
            {
                return ERROR;
            }
            //check size
        }
        else if (curnode->type == matrixVar)
        {
            return MATRIX;
        }
    }
    else
    {
        tokenPtr curnode = (tokenPtr)astRoot->data;
        if (curnode->type == ID)
        {
            int check = checkDeclaration(stRoot, curnode);
            if (check != ERROR)
                return check;
            else
                return ERROR;
        }
        else if (curnode->type == STR)
        {
            return STRING;
        }
        else if (curnode->type == NUM)
        {
            return INT;
        }
        else if (curnode->type == RNUM)
        {
            return REAL;
        }
        return ERROR;
    }
}

int checkType(Node stRoot, Node astRoot)
{
    int lhs = checkDeclaration(stRoot, ((tokenPtr)astRoot->child->data));
    int rhs = typeAE(stRoot, astRoot->child->sibling);

    // printf("%s LHS: %s RHS: %s\n", ((tokenPtr)astRoot->child->data)->string, keys2[lhs], keys2[rhs]);
    if (lhs == rhs && lhs != ERROR)
        return 1;
    else
    {
        if (lhs == ERROR)
        {
            flag = 1;
            printf("line no.: %d\t Semantic error: The ID \"%s\" is not declared \n", ((tokenPtr)astRoot->child->data)->lineno, ((tokenPtr)astRoot->child->data)->string);
        }
        else
        {
            flag = 1;
            if (rhs == ERROR)
                printf("line no.: %d\t Semantic error: Type mismatch LHS (\"%s\") and RHS (\"undefined\") \n", ((tokenPtr)astRoot->child->data)->lineno, keys2[lhs]);
            else
                printf("line no.: %d\t Semantic error: Type mismatch LHS (\"%s\") and RHS (\"%s\") \n", ((tokenPtr)astRoot->child->data)->lineno, keys2[lhs], keys2[rhs]);
        }
    }
    return 0;
}

int checkInitialization(Node stRoot)
{
    htHead head = stRoot->data;
    int i, check = 1;
    hashTable temp;
    for (i = 0; i < M; i++)
    {
        temp = head->ht[i];
        while (temp)
        {
            // printf("%s\t%s\t%d\t%d\n", temp->id, keys2[temp->type], temp->width, temp->offset);
            if (temp->initialised == 0)
            {
                check = 0;
                printf("line no.: %d\t Semantic error: output parameter \"%s\" is not assigned value in scope of function \"%s\" \n", ((htHead)stRoot->data)->lineno, temp->id, ((htHead)stRoot->data)->scope);
                flag = 1;
            }

            temp = temp->next;
        }
    }
    return check;
}

void compareParameters(Node stRoot, parNode input, Node inputAST, Node till)
{
    int lineno = 1;
    if (inputAST)
        lineno = ((tokenPtr)inputAST->data)->lineno;
    int type;
    while (input && (inputAST != till))
    {
        // printf("%s %s %s %s\n", input->id, keys2[input->type], ((tokenPtr)inputAST->data)->string, keys2[typeAE(stRoot, inputAST)]);
        type = typeAE(stRoot, inputAST);
        if (input->type == type)
        {
            input = input->next;
            inputAST = inputAST->sibling;
        }
        else
        {
            flag = 1;
            if (type != ERROR)
            {
                if (lineno != 1 && ((tokenPtr)inputAST->data)->type == ID)
                    printf("line no.: %d\t Semantic error: type of parameter \"%s\" (\"%s\") and ID \"%s\" (\"%s\") do not match \n", lineno, input->id, keys2[input->type], ((tokenPtr)inputAST->data)->string, keys2[type]);
                else if (lineno != 1 && ((tokenPtr)inputAST->data)->type == NUM)
                    printf("line no.: %d\t Semantic error: type of parameter \"%s\" (\"%s\") and NUM \"%d\" (\"%s\") do not match \n", lineno, input->id, keys2[input->type], (int)((tokenPtr)inputAST->data)->value, keys2[type]);
                else if (lineno != 1 && ((tokenPtr)inputAST->data)->type == RNUM)
                    printf("line no.: %d\t Semantic error: type of parameter \"%s\" (\"%s\") and RNUM \"%.2f\" (\"%s\") do not match \n", lineno, input->id, keys2[input->type], ((tokenPtr)inputAST->data)->value, keys2[type]);
                else if (lineno != 1 && ((tokenPtr)inputAST->data)->type == STR)
                    printf("line no.: %d\t Semantic error: type of parameter \"%s\" (\"%s\") and STR \"%s\" (\"%s\") do not match \n", lineno, input->id, keys2[input->type], ((tokenPtr)inputAST->data)->string, keys2[type]);
                else
                    printf("Semantic error: type of parameter \"%s\" (\"%s\") and ID \"%s\" (\"%s\") do not match \n", input->id, keys2[input->type], ((tokenPtr)inputAST->data)->string, keys2[type]);
            }
            else
            {
                printf("line no.: %d\t Semantic error: The ID \"%s\" is not declared \n", lineno, ((tokenPtr)inputAST->data)->string);
            }
            input = input->next;
            inputAST = inputAST->sibling;
        }
    }
    if (input || (inputAST != till))
        return;
    else
    {
        if (lineno != 0)
            printf("line no.: %d\t Semantic error: mismatch parameter length \n", lineno);
        else
            printf("Semantic error: mismatch parameter length \n");
    }
}

sizeptr computeSize(Node stRoot, Node astRoot, int type)
{
    if (type == MATRIX)
    {
        if (!astRoot->isterminal)
        {
            astNode curnode = (astNode)astRoot->data;
            if (curnode->isImp)
            {
                sizeptr a, b;
                a = computeSize(stRoot, astRoot->child, MATRIX);
                b = computeSize(stRoot, astRoot->child->sibling, MATRIX);
                if (a == NULL)
                {
                    if (b != NULL)
                        free(b);
                    return NULL;
                }
                else if (b == NULL)
                {
                    return NULL;
                }
                else if ((a->x == b->x) && (a->y == b->y))
                {
                    free(a);
                    return b;
                }
                else
                {
                    Node temp = astRoot;
                    while (1)
                    {
                        if (temp->isterminal)
                            break;
                        temp = temp->child;
                    }
                    int lineno = ((tokenPtr)temp->data)->lineno;
                    flag = 1;
                    printf("line no.: %d\t Semantic error: Matrix Size mismatch \n", lineno);
                    // printf("ERROR(X) matrix size mismatch in line %d\n", lineno);
                }
                return NULL;
            }
            else if (curnode->type == matrixVar)
            {
                int x = 0, y = 0, set = 0, temp = 0;
                Node temp1 = astRoot->child, temp2 = NULL;
                while (temp1)
                {
                    x++;
                    temp = 0;
                    temp2 = temp1->child;
                    while (temp2)
                    {
                        temp++;
                        temp2 = temp2->sibling;
                    }
                    if (set == 0)
                    {
                        set = 1;
                        y = temp;
                    }
                    else
                    {
                        if (y != temp)
                        {
                            flag = 1;
                            printf("line no.: %d\t Semantic error: Matrix Row Size variable \n", ((tokenPtr)astRoot->child->child->data)->lineno);
                            return NULL;
                        }
                    }
                    temp1 = temp1->sibling;
                }
                sizeptr size1 = (sizeptr)malloc(sizeof(sizenode));
                size1->x = x;
                size1->y = y;
                return size1;
            }
        }
        else
        {
            tokenPtr curnode = (tokenPtr)astRoot->data;
            if (curnode->type == ID)
            {
                return returnSize(stRoot, curnode);
            }
        }
    }
    else //STRING
    {
        if (!astRoot->isterminal)
        {
            astNode curnode = (astNode)astRoot->data;
            if (curnode->isImp)
            {
                sizeptr a, b;
                a = computeSize(stRoot, astRoot->child, STRING);
                b = computeSize(stRoot, astRoot->child->sibling, STRING);
                if (a == NULL)
                {
                    if (b != NULL)
                        free(b);
                    return NULL;
                }
                else if (b == NULL)
                {
                    return NULL;
                }
                else
                {
                    b->x += a->x;
                    free(a);
                    return b;
                }
            }
        }
        else
        {
            tokenPtr curnode = (tokenPtr)astRoot->data;
            if (curnode->type == ID)
            {
                return returnSize(stRoot, curnode);
            }
            else if (curnode->type == STR)
            {
                int x = strlen(curnode->string);
                sizeptr size1 = (sizeptr)malloc(sizeof(sizenode));
                size1->x = x;
                size1->y = 0;
                return size1;
            }
        }
    }
}

int checkOverloading(char *funcid, int lineno, Node stRoot)
{
    Node parentscope = stRoot->parent;
    // if (parentscope == NULL)
    // return 0;
    Node child = parentscope->child;
    while (child)
    {
        if (strcmp(((htHead)child->data)->scope, funcid) == 0)
        {
            flag = 1;
            printf("line no.: %d\t Semantic error: Function \"%s\" is being overloaded which is not allowed \n", lineno, funcid);
            return 1;
        }
        child = child->sibling;
    }
    if (parentscope->parent)
        return checkOverloading(funcid, lineno, parentscope);
    else
        return 0;
}