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
        root->data = calloc(1, sizeof(astnode));
        ((astNode)root->data)->type = ROOT;
        ((astNode)root->data)->isImp = false;
        Node child = parseTreeNode->child;
        while (child)
        {
            makeAST(child, root);
            child = child->sibling;
        }
        // printf("%lu %d\n", root, ((Rhs)root->data)->type);
    }
    else if (!parseTreeNode->isterminal) //if node of parsetree is non terminal
    {
        Rhs data = (Rhs)parseTreeNode->data;
        int datatype = data->type;
        // if (datatype == booleanExpression)
        // {
        //     printf("Helo\n");
        // }

        //take care of singelton rules
        //take care of normal children
        if (datatype == var || datatype == stmtsAndFunctionDefs || datatype == otherStmts || datatype == inputParameterList || datatype == varList || datatype == moreIds || datatype == remainingList || datatype == moreStmtAndFunctionDefs || datatype == stmtOrFunctionDef || datatype == assignFuncCallSizeStmt || datatype == funcCallSizeStmt || datatype == emptyOrInputParameterList || datatype == listVar || datatype == operatorplusminus || datatype == operatormuldiv || datatype == constrainedVars || datatype == logicalOp || datatype == relationalOp || datatype == assignmentStmt || datatype == arithmeticExpression1 || datatype == declarationStmt || datatype == parameterList || datatype == arithmeticExpression3 || datatype == varExpression || datatype == funCallStmt || datatype == matrixRows1 || datatype == matrixRow1 || datatype == ioStmt || datatype == conditionalStmt || datatype == booleanExpression2 || datatype == sizeStmt || datatype == matrixElement)
        {
            //add node name declstmt

            Node child = parseTreeNode->child;
            while (child)
            {
                makeAST(child, parentAST);
                child = child->sibling;
            }
        }
        //add node for stmt or functiondef
        else if (datatype == functionDef || datatype == stmt || datatype == arithmeticExpression || datatype == arithmeticExpression2 || datatype == typevar || datatype == matrixVar || datatype == matrixRows || datatype == matrixRow || datatype == booleanExpression || datatype == elseStmt)
        {
            astNode astdata = (astNode)calloc(1, sizeof(astnode));
            astdata->isImp = false;
            astdata->type = datatype;
            Node curnode = add_child(parentAST, astdata, false);
            Node child = parseTreeNode->child;
            while (child)
            {
                makeAST(child, curnode);
                child = child->sibling;
            }
            if (datatype == arithmeticExpression || datatype == arithmeticExpression2)
            {
                if (curnode->child->sibling == NULL)
                {
                    Node before, temp = parentAST->child;
                    if (temp == curnode)
                        parentAST->child = curnode->child;
                    else
                    {
                        while (temp->sibling)
                        {
                            before = temp;
                            temp = temp->sibling;
                            if (temp == curnode)
                                before->sibling = curnode->child;
                        }
                    }
                    curnode->child->parent = parentAST;
                    free(astdata);
                    free(curnode);
                }
            }
            else if (datatype == matrixVar)
            {
                rebalanceMatrixRows(curnode->child); //do something
                Node child = curnode->child;
                while (child)
                {
                    if (child->child->sibling)
                        rebalanceMatrixRow(child->child->sibling);
                    child = child->sibling;
                }
            }
            else if (datatype == elseStmt || datatype == matrixElement)
            {
                if (curnode->child == NULL)
                {
                    Node child = parentAST->child;
                    while (child->sibling)
                    {
                        if (curnode == child->sibling)
                        {
                            child->sibling = NULL;
                            break;
                        }
                        child = child->sibling;
                    }
                    free(curnode);
                } //do something
            }
        }

        if (datatype == declarationStmt || datatype == sizeStmt || datatype == conditionalStmt || datatype == ioStmt || datatype == funCallStmt || datatype == assignmentStmt || datatype == matrixElement)
        {
            ((astNode)parentAST->data)->type = datatype;
        }
    }
    else
    {

        tokenPtr data = (tokenPtr)parseTreeNode->data;
        int datatype = data->type;
        // if (datatype == NOT)
        // {
        //     printf("Helo\n");
        // }
        if (datatype == MAIN || datatype == INT || datatype == REAL || datatype == STRING || datatype == MATRIX || datatype == PLUS || datatype == MINUS || datatype == MUL || datatype == DIV || datatype == LT || datatype == LE || datatype == EQ || datatype == GT || datatype == GE || datatype == NE || datatype == AND || datatype == OR || datatype == NOT)
        {
            // parentAST->data = (astNode)calloc(1, sizeof(astnode));
            ((astNode)parentAST->data)->isImp = true;
            ((astNode)parentAST->data)->type = datatype;
            // printf("%lu %d\n", parentAST, ((astNode)parentAST->data)->type);
        }
        else if (datatype == ID || datatype == NUM || datatype == RNUM || datatype == STR || datatype == FUNID || datatype == READ || datatype == PRINT || datatype == END)
        {
            // if (datatype == NUM) //delete later
            // return root;
            Node curnode = add_child(parentAST, data, true);
        }
        else if (datatype == SIZE)
        {
            astNode data = (astNode)calloc(1, sizeof(astnode));
            data->isImp = true;
            data->type = datatype;
            Node curnode = add_child(parentAST, data, false);
        } //take care of epsilon and other useless terminals
        // else if (datatype == EPSILON || datatype == SQO || datatype == SQC || datatype == END || datatype == SEMICOLON || datatype == COMMA || datatype == OP || datatype == CL || datatype == FUNCTION || datatype == ASSIGNOP)
        // {
        //     return root;
        // }
        // else if (datatype == INT || datatype == REAL || datatype == STRING || datatype == MATRIX)
        // {
        //     printf("Hello");
        // }
    }
    free(parseTreeNode);
    return root;
}

void rebalanceMatrixRows(Node rows)
{
    Node row = rows->child;
    if (row->sibling)
        rebalanceMatrixRows(row->sibling);
    rows->child = row->child;
    rows->data = row->data;
    rows->isterminal = row->isterminal;
    rows->sibling = row->sibling;
    free(row);
}

void rebalanceMatrixRow(Node row)
{
    Node num = row->child;
    if (num->sibling)
        rebalanceMatrixRow(num->sibling);
    row->child = num->child;
    row->data = num->data;
    row->isterminal = num->isterminal;
    row->sibling = num->sibling;
    free(num);
}