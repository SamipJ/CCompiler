// SAMIP JASANI 2015A7PS0127P
#ifndef LEXERDEF_H
#define LEXERDEF_H
// const char keys1[][45] = {"mainFunction", "stmtsAndFunctionDefs", "moreStmtAndFunctionDefs", "stmtOrFunctionDef", "stmt", "functionDef", "parameterList", "typevar", "remainingList", "declarationStmt", "varList", "moreIds", "assignFuncCallSizeStmt", "funcCallSizeStmt", "sizeStmt", "conditionalStmt", "otherStmts", "elseStmt", "ioStmt", "funCallStmt", "emptyOrInputParameterList", "inputParameterList", "listVar", "assignmentStmt", "arithmeticExpression", "arithmeticExpression1", "arithmeticExpression2", "arithmeticExpression3", "varExpression", "operatorplusminus", "operatormuldiv", "booleanExpression", "booleanExpression2", "moreBooleanExpression", "constrainedVars", "matrixVar", "matrixRows", "matrixRows1", "matrixRow", "matrixRow1", "var", "matrixElement", "logicalOp", "relationalOp", "ROOT"};
// const char keys2[][43] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "EPSILON", "FINISH"};

enum tokenType
{
    NONE,
    ERROR,
    ASSIGNOP,
    COMMENT,
    FUNID,
    ID,
    NUM,
    RNUM,
    STR,
    END,
    INT,
    REAL,
    STRING,
    MATRIX,
    MAIN,
    SQO,
    SQC,
    OP,
    CL,
    SEMICOLON,
    COMMA,
    IF,
    ELSE,
    ENDIF,
    READ,
    PRINT,
    FUNCTION,
    PLUS,
    MINUS,
    MUL,
    DIV,
    SIZE,
    AND,
    OR,
    NOT,
    LT,
    LE,
    EQ,
    GT,
    GE,
    NE,
    EPSILON,
    FINISH,
    BOOL
};

extern const char keys1[][30];
typedef struct token Token;

typedef Token *tokenPtr;

struct token
{
    enum tokenType type;
    int lineno;
    float value;
    char *string;
    // tokenPtr next;
};

#endif