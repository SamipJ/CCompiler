// SAMIP JASANI 2015A7PS0127P
#ifndef LEXERDEF_H
#define LEXERDEF_H

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
    FINISH
};

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