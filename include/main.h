#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_LOGICAL_OR,
    TOKEN_LOGICAL_AND,
    TOKEN_HERE_DOCUMENT,
    TOKEN_REDIRECT_OUT_APPEND,
    TOKEN_SEND_TO_BACKGROUND
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

typedef struct AST AST;
typedef struct Redirection Redirection;

struct Redirection{
    enum{
        REDIR_IN,
        REDIR_OUT,
        REDIR_APPEND
    } type;
    char* filename;
    Redirection* next;
};

struct AST{
    enum{
        AST_ARGS,
        AST_PIPE,
        AST_LOGIC_OP,
    } tag;
    
    union {
        struct{
            char** argv;
            int argc;
            Redirection* redirects;
        } cmd;
        
        struct{
            AST *left, *right;
        } pipe;
        
        struct{
            enum{
                LOG_AND,
                LOG_OR
            } op;
            AST *left, *right;
        } logic;
    };
};

Token* analize(char *input, int *token_count_ptr);
AST* create_AST(Token *tokens);

#endif