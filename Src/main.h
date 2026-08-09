#ifndef SHELL_H
#define SHELL_H

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

typedef struct Command{
    char* argv;
    int argc;
    char *input_file;
    char *output_file;
    bool append_output;
    struct command *next;
} Command;

Token* analize(char *input, int *token_count_ptr);

#endif