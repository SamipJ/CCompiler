// SAMIP JASANI 2015A7PS0127P
#include "_Tree.h"
#include <stdio.h>
#include <stdbool.h>
#include "parserDef.h"
#include "lexer.h"
#include "astDef.h"
// const char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
// const char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};

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
    return sibl;
}

Node add_child(Node n, void *ptr, bool isterminal)
{
    Node child;
    if (n == NULL)
        return NULL;
    if (n->child)
    {
        child = add_sibling(n->child, ptr, isterminal);
        return child;
    }
    else
    {
        n->child = new_node(ptr, isterminal);
        child = n->child;
        child->parent = n;
    }
    return child;
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
    // char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    // char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
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
    }
}

void FileInorderTree(Node n, FILE *fp)
{
    if (n->child)
    {
        FileInorderTree(n->child, fp);
    }
    if (n->isterminal)
    {
        if (((tokenPtr)(n->data))->type == NUM)
        {
            fprintf(fp, "--------------------\t%-5d\t%-20s\t%-10d\t%-20s\tyes\t\t--------------------\n", ((tokenPtr)(n->data))->lineno, keys2[((tokenPtr)(n->data))->type], (int)((tokenPtr)(n->data))->value, keys1[((Rhs)(n->parent->data))->type]);
        }
        else if (((tokenPtr)(n->data))->type == RNUM)
        {
            fprintf(fp, "--------------------\t%-5d\t%-20s\t%-10.2f\t%-20s\tyes\t\t--------------------\n", ((tokenPtr)(n->data))->lineno, keys2[((tokenPtr)(n->data))->type], ((tokenPtr)(n->data))->value, keys1[((Rhs)(n->parent->data))->type]);
        }
        else if (((tokenPtr)(n->data))->type == EPSILON)
        {
            fprintf(fp, "%-20s\t-----\t%-20s\t----------\t%-20s\tyes\t\t--------------------\n", ((tokenPtr)(n->data))->string, keys2[((tokenPtr)(n->data))->type], keys1[((Rhs)(n->parent->data))->type]);
        }
        else
        {
            fprintf(fp, "%-20s\t%-5d\t%-20s\t----------\t%-20s\tyes\t\t--------------------\n", ((tokenPtr)(n->data))->string, ((tokenPtr)(n->data))->lineno, keys2[((tokenPtr)(n->data))->type], keys1[((Rhs)(n->parent->data))->type]);
        }
    }
    else if (n->parent == NULL)
    {
        fprintf(fp, "--------------------\t-----\t--------------------\t----------\tROOT                \tno\t\t%-20s\n", keys1[((Rhs)(n->data))->type]);
    }
    else
    {
        fprintf(fp, "--------------------\t-----\t--------------------\t----------\t%-20s\tno\t\t%-20s\n", keys1[((Rhs)(n->parent->data))->type], keys1[((Rhs)(n->data))->type]);
    }
    n = n->child;
    if (n)
    {
        while (n->sibling)
        {
            n = n->sibling;
            FileInorderTree(n, fp);
        }
    }
}

void PrintInorderASTree(Node n)
{
    // char keys1[][44] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp"};
    // char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};
    if (n->child)
    {
        printf("\\|/\n");
        PrintInorderASTree(n->child);
        printf("/|\\\n");
    }
    if (n->isterminal)
        printToken(n->data);
    else
    {
        if (((astNode)(n->data))->isImp)
            printf("%s\n", keys2[((astNode)(n->data))->type]);
        else
            printf("%s\n", keys1[((astNode)(n->data))->type]);
    }
    n = n->child;
    if (n)
    {
        printf("\\|/\n");

        while (n->sibling)
        {
            printf("->\n");
            n = n->sibling;
            PrintInorderASTree(n);
        }
        printf("/|\\\n");
    }
}