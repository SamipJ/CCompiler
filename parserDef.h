// SAMIP JASANI 2015A7PS0127P
#ifndef PARSERDEF_H
#define PARSERDEF_H
#include "lexerDef.h"
#include <stdbool.h>
// typedef rules *Rules;

enum nont
{
    mainFunction,
    stmtsAndFunctionDefs,
    moreStmtAndFunctionDefs,
    stmtOrFunctionDef,
    stmt,
    functionDef,
    parameterList,
    typevar,
    remainingList,
    declarationStmt,
    varList,
    moreIds,
    assignFuncCallSizeStmt,
    funcCallSizeStmt,
    sizeStmt,
    conditionalStmt,
    otherStmts,
    elseStmt,
    ioStmt,
    funCallStmt,
    emptyOrInputParameterList,
    inputParameterList,
    listVar,
    assignmentStmt,
    arithmeticExpression,
    arithmeticExpression1,
    arithmeticExpression2,
    arithmeticExpression3,
    varExpression,
    operatorplusminus,
    operatormuldiv,
    booleanExpression,
    booleanExpression2,
    moreBooleanExpression,
    constrainedVars,
    matrixVar,
    matrixRows,
    matrixRows1,
    matrixRow,
    matrixRow1,
    var,
    matrixElement,
    logicalOp,
    relationalOp,
    ROOT
};

typedef struct rhs *Rhs;

typedef struct rhs
{
    int type;
    bool isTerminal;
    Rhs next;
} rhside;

typedef struct rules *Rules;

typedef struct rules
{
    Rhs rule;
    Rules next;
} rules;

typedef Rules *Grammer;

// typedef struct grammer
// {

// } grammer;

// typedef grammer *Grammer;

#endif