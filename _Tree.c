// SAMIP JASANI 2015A7PS0127P
#include "_Tree.h"
#include <stdio.h>
#include <stdbool.h>
#include "parserDef.h"
#include "lexer.h"

Node new_node(void *ptr, bool isterminal)
{
    Node newNode = calloc(1, sizeof(node));
    newNode->data = ptr;
    newNode->isterminal = isterminal;
    return newNode;
}

Node add_sibling(Node n, void *ptr, bool isterminal)
{
    if (n == NULL)
        return NULL;
    while (n->sibling)
        n = n->sibling;
    n->sibling = new_node(ptr, isterminal);
    Node sibl = n->sibling;
    sibl->isterminal = isterminal;
    sibl->parent = n->parent;
    return n;
}

Node add_child(Node n, void *ptr, bool isterminal)
{
    Node child;
    if (n == NULL)
        return NULL;
    if (n->child)
    {
        add_sibling(n->child, ptr, isterminal);
        return n;
    }
    else
    {
        n->child = new_node(ptr, isterminal);
        child = n->child;
        child->parent = n;
    }
    return n;
}

Node nextNT(Node n)
{
    if (n == NULL)
    {
        return NULL;
    }
    if (n->sibling)
        return n->sibling;
    return (nextNT(n->parent));
}
// if (!n->isterminal)
// {
//     n = n->child;
//     while (n->sibling)
//     {
//         if (!n->isterminal && n->child == NULL)
//             return n;
//         n = n->sibling;
//     }
//     if (!n->isterminal && n->child == NULL)
//         return n;
//     n = n->parent;
// }
// return nextNT(n->parent);
// }

// Node addRule(Node n, Rules rulenode)
// {
//     Rhs rhsnode = rulenode->rule;
//     while (rhsnode)
//     {
//         add_child(n, rhsnode, rhsnode->isTerminal)
//     }
// }
// }
// else
// {
//     while (n->sibling)
//     {
//         n = n->sibling;
//         if (n->isterminal)
//             return n;
//     }
//     if (n->parent)
//     {
//         n = n->parent;
//     }
// }
// }
void PrintInorderTree(Node n)
{
    char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
    if (n->child)
    {
        printf("\\|/\n");
        PrintInorderTree(n->child);
        printf("/|\\\n");
    }
    if (n->isterminal)
        printToken(n->data);
    else
        printf("%s\n", keys1[((Rhs)(n->data))->type]);
    n = n->child;
    if (n)
    {
        printf("\\|/\n");

        while (n->sibling)
        {
            printf("->\n");
            n = n->sibling;
            PrintInorderTree(n);
        }
        printf("/|\\\n");

    } // while (n->child)
    // {
    //     n = n->child;
    // }
    // if (n->isterminal)
    //     printf("%d", (tokenPtr)(n->data)->type);
    // else
    //     printf("%d", (Rhs)(n->data)->type)
    // while(n->sibling){

    // }
}