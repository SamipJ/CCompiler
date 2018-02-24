// SAMIP JASANI 2015A7PS0127P
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexerDef.h"

extern int lineNo, bufSize, bufIndex;
extern char *buf;
extern FILE *fp;

FILE *getStream(FILE *fp, char *buf)
{
    if (fp != NULL)
    {
        size_t newLen = fread(buf, sizeof(char), bufSize - 1, fp);
        if (ferror(fp) != 0)
        {
            fputs("Error reading file", stderr);
            buf[0] = '\0';
            return NULL;
        }
        else
        {
            buf[newLen++] = '\0'; /* Just to be safe. */
        }
        if (newLen != bufSize)
        {
            fclose(fp);
            return NULL;
        }
        else
        {
            return fp;
        }
    }
    else
    {
        buf[0] = '\0';
        return NULL;
    }
}

// tokenPtr getNextToken(char *buf, enum tokenType *type, int *value, char *string, tokenPtr *prev, int *state)
// {
//     int i = 0;
//     while (buf[i] != '\0')
//     {
//     }
// };

int getNextState(tokenPtr tokenp, int currentstate, char c)
{
    int state;
    switch (currentstate)
    {
    case 0:
        switch (c)
        {
        case '=':
            state = 1;
            bufIndex++;
            break;
        case '#':
            state = 3;
            bufIndex++;
            break;

        case '_':
            state = 4;
            bufIndex++;
            break;
        case '"':
            state = 10;
            bufIndex++;
            break;
        case '\r':
            state = 0;
            bufIndex++;
            break;
        case '\n':
            state = 0;
            bufIndex++;
            lineNo++;
            break;
        case '\t':
            bufIndex++;
            state = 0;
            break;
        case ' ':
            bufIndex++;
            state = 0;
            break;
        case '[':
            state = -1;
            bufIndex++;
            tokenp->type = SQO;
            tokenp->lineno = lineNo;
            break;
        case ']':
            state = -1;
            bufIndex++;
            tokenp->type = SQC;
            tokenp->lineno = lineNo;
            break;
        case '(':
            state = -1;
            bufIndex++;
            tokenp->type = OP;
            tokenp->lineno = lineNo;
            break;
        case ')':
            state = -1;
            bufIndex++;
            tokenp->type = CL;
            tokenp->lineno = lineNo;
            break;
        case ';':
            state = -1;
            bufIndex++;
            tokenp->type = SEMICOLON;
            tokenp->lineno = lineNo;
            break;
        case ',':
            state = -1;
            bufIndex++;
            tokenp->type = COMMA;
            tokenp->lineno = lineNo;
            break;
        case '+':
            state = -1;
            bufIndex++;
            tokenp->type = PLUS;
            tokenp->lineno = lineNo;
            break;
        case '-':
            state = -1;
            bufIndex++;
            tokenp->type = MINUS;
            tokenp->lineno = lineNo;
            break;
        case '/':
            state = -1;
            bufIndex++;
            tokenp->type = DIV;
            tokenp->lineno = lineNo;
            break;
        case '@':
            state = -1;
            bufIndex++;
            tokenp->type = SIZE;
            tokenp->lineno = lineNo;
            break;
        case '*':
            state = -1;
            bufIndex++;
            tokenp->type = MUL;
            tokenp->lineno = lineNo;
            break;
        case '<':
            state = 12;
            bufIndex++;
            break;
        case '>':
            state = 13;
            bufIndex++;
            break;
        case '.':
            state = 14;
            bufIndex++;
            break;
        case 0:
            tokenp->type = FINISH;
            tokenp->lineno = lineNo;
            break;
        default:
            if (isalpha(c))
            {
                state = 6;
                bufIndex++;
                tokenp->string = calloc(2, sizeof(char));
                tokenp->string[1] = '\0';
                tokenp->string[0] = c;
            }
            else if (isdigit(c))
            {
                state = 7;
                bufIndex++;
                tokenp->value = c - '0';
            }
            else
            {
                state = -1;
                tokenp->type = ERROR;
                tokenp->lineno = lineNo;
                bufIndex++;
            }
        }

        break;
    case 1:
        switch (c)
        {
        case '/':
            state = 2;
            bufIndex++;
            break;
        case '=':
            state = -1;
            tokenp->type = EQ;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        default:
            state = -1;
            tokenp->type = ASSIGNOP;
            tokenp->lineno = lineNo;
            break;
        }
        break;
    case 2:
        if (c == '=')
        {
            state = -1;
            tokenp->type = NE;
            tokenp->lineno = lineNo;
            bufIndex++;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
        }
        break;

    case 3:
        if (c == '\n')
        {
            state = -1;
            tokenp->type = COMMENT;
            tokenp->lineno = lineNo;
            lineNo++;
            bufIndex++;
        }
        else
        {
            state = 3;
            bufIndex++;
        }
        break;
    case 4:
        if (isalnum(c))
        {
            state = 5;
            tokenp->string = calloc(3, sizeof(char));
            tokenp->string[0] = '_';
            tokenp->string[1] = c;
            tokenp->string[2] = '\0';
            bufIndex++;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
        }
        break;
    case 5:
        if (isalnum(c))
        {
            tokenp->string = realloc(tokenp->string, (strlen(tokenp->string) + 2) * sizeof(char));
            tokenp->string[strlen(tokenp->string) + 1] = '\0';
            tokenp->string[strlen(tokenp->string)] = c;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            if (strcmp(tokenp->string, "_main") == 0)
            {
                tokenp->lineno = lineNo;
                tokenp->type = MAIN;
                free(tokenp->string);
                tokenp->string = NULL;
            }
            else
            {
                tokenp->type = FUNID;
                tokenp->lineno = lineNo;
            }
            break;
        }
    case 6:
        if (isalpha(c))
        {
            tokenp->string = realloc(tokenp->string, (strlen(tokenp->string) + 2) * sizeof(char));
            tokenp->string[strlen(tokenp->string) + 1] = '\0';
            tokenp->string[strlen(tokenp->string)] = c;
            bufIndex++;
            break;
        }
        else if (isdigit(c))
        {
            state = -1;
            tokenp->string = realloc(tokenp->string, (strlen(tokenp->string) + 2) * sizeof(char));
            tokenp->string[strlen(tokenp->string) + 1] = '\0';
            tokenp->string[strlen(tokenp->string)] = c;
            bufIndex++;
            tokenp->type = ID;
            tokenp->lineno = lineNo;
            break;
        }
        else
        {
            tokenp->type = ID;
            // LOOKUPTABLE
            tokenp->lineno = lineNo;
            break;
        }
    case 7:
        if (isdigit(c))
        {
            tokenp->value = (tokenp->value) * 10 + c - '0';
            bufIndex++;
        }
        else if (c == '.')
        {
            bufIndex++;
            state = 8;
        }
        else
        {
            state = -1;
            tokenp->type = NUM;
            tokenp->lineno = lineNo;
        }
        break;
    case 8:
        if (isdigit(c))
        {
            tokenp->value += .1 * (c - '0');
            bufIndex++;
            state = 9;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
        }
        break;
    case 9:
        if (isdigit(c))
        {
            state = -1;
            tokenp->value += .01 * (c - '0');
            bufIndex++;
            tokenp->type = RNUM;
            tokenp->lineno = lineNo;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
        }
        break;
    case 10:
        if (isalpha(c) || c == ' ')
        {
            state = 11;
            tokenp->string = calloc(3, sizeof(char));
            tokenp->string[0] = '"';
            tokenp->string[1] = c;
            tokenp->string[2] = '\0';
            bufIndex++;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
        }
        break;
    case 11:
        if (isalpha(c) || c == ' ')
        {
            tokenp->string = realloc(tokenp->string, (strlen(tokenp->string) + 2) * sizeof(char));
            tokenp->string[strlen(tokenp->string) + 1] = '\0';
            tokenp->string[strlen(tokenp->string)] = c;
            bufIndex++;
        }
        else if (c == '"')
        {
            tokenp->string = realloc(tokenp->string, (strlen(tokenp->string) + 2) * sizeof(char));
            tokenp->string[strlen(tokenp->string) + 1] = '\0';
            tokenp->string[strlen(tokenp->string)] = c;
            bufIndex++;
            tokenp->type = STR;
            tokenp->lineno = lineNo;
            state = -1;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
        }
        break;
    case 12:
        if (c == '=')
        {
            bufIndex++;
            state = -1;
            tokenp->type = LE;
            tokenp->lineno = lineNo;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = LT;
            tokenp->lineno = lineNo;
            break;
        }
    case 13:
        if (c == '=')
        {
            bufIndex++;
            state = -1;
            tokenp->type = GE;
            tokenp->lineno = lineNo;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = GT;
            tokenp->lineno = lineNo;
            break;
        }

    case 14:
        if (c == 'a')
        {
            state = 15;
            bufIndex++;
            break;
        }
        else if (c == 'o')
        {
            state = 18;
            bufIndex++;
            break;
        }
        else if (c == 'n')
        {
            state = 20;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 15:
        if (c == 'n')
        {
            state = 16;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 16:
        if (c == 'd')
        {
            state = 17;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 17:
        if (c == '.')
        {
            state = -1;
            tokenp->type = AND;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 18:
        if (c == 'r')
        {
            state = 19;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 19:
        if (c == '.')
        {
            state = -1;
            tokenp->type = OR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 20:
        if (c == 'o')
        {
            state = 21;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 21:
        if (c == 'd')
        {
            state = 22;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    case 22:
        if (c == '.')
        {
            state = -1;
            tokenp->type = NOT;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
        else
        {
            state = -1;
            tokenp->type = ERROR;
            tokenp->lineno = lineNo;
            bufIndex++;
            break;
        }
    }

    return state;
}

tokenPtr getNextToken()
{
    int state = 0;
    tokenPtr tokenp = (tokenPtr)calloc(1, sizeof(Token));
    while (bufIndex < bufSize - 1 && buf[bufIndex] != '\0')
    {
        state = getNextState(tokenp, state, buf[bufIndex]);
        if (bufIndex == bufSize - 1)
        {
            bufIndex = 0;
            fp = getStream(fp, buf);
        }
        if (tokenp->type != NONE)
        {
            break;
        }
    }
    if (buf[bufIndex] == '\0')
    {
        state = getNextState(tokenp, state, buf[bufIndex]);
    }
    return tokenp;
    // if (fp != NULL)
    // {
    //     fp = getStream(fp, buf);
    //     while (bufIndex < bufSize && buf[bufIndex] != '\0')
    //     {
    //     }
    // }
}

const char *typeTok[] = {"NONE", "ERROR", "ASSIGNOP", "COMMENT", "FUNID", "ID", "NUM", "RNUM", "STR", "END", "INT", "REAL", "STRING", "MATRIX", "MAIN", "SQO", "SQC", "OP", "CL", "SEMICOLON", "COMMA", "IF", "ELSE", "ENDIF", "READ", "PRINT", "FUNCTION", "PLUS", "MINUS", "MUL", "DIV", "SIZE", "AND", "OR", "NOT", "LT", "LE", "EQ", "GT", "GE", "NE", "FINISH"};
void printToken(tokenPtr t)
{
    if (t->string == NULL)
    {
        printf("Token Type: %s  Token Value: %.2f Token Line: %d\n", typeTok[t->type], t->value, t->lineno);
    }
    else
    {
        printf("Token Type: %s Token Value: %.2f Token Line: %d Token String: %s\n", typeTok[t->type], t->value, t->lineno, t->string);
    }
}