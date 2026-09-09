#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include "main.h"

#define MAX_LINE 1024
#define MAX_ARGS 128
#define MAX_TOKENS 1024

void print_shell_prompt(){
    struct winsize w;
    int cols = 80; 

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0) {
        cols = w.ws_col;
    }
    for (int i = 0; i < cols - 1; i++) {
        putchar(' ');
    }

    putchar('\r');
    printf("msh> ");
    fflush(stdout);
}

void get_input(char *input){
    if(fgets(input, MAX_LINE, stdin) == NULL){
        exit(0);
    }

    input[strlen(input) - 1] = '\0';
}


int main(void){
    char user_input[MAX_LINE];
    char *args[MAX_ARGS];
    int token_count = 0;

    while(1) {
        print_shell_prompt();
        get_input(user_input);
        Token *tokens = analize(user_input, &token_count);
        AST *ast_tree = create_ast(tokens);

        free(tokens);
    }

    return 0;
}