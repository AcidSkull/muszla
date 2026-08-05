#ifndef SHELL_H
#define SHELL_H

#include <stdbool.h>

typedef enum {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT
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

#endif