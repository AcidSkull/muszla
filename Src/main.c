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

void parse_input(char *input, char **args){
    int i = 0;
    args[i] = strtok(input, " ");

    while(args[i] != NULL && i < MAX_ARGS - 1){
        i++;
        args[i] = strtok(NULL, " ");
    }

    args[i] = NULL;
}

void execute_command(char **args){
    pid_t pid = fork();
    
    switch (pid)
    {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            if (execvp(args[0], args) < 0) {
                perror(args[0]);
            }
            exit(EXIT_SUCCESS);
        default:
            int status;
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
}

int main(void){
    char user_input[MAX_LINE];
    char *args[MAX_ARGS];

    while(1) {
        print_shell_prompt();
        get_input(user_input);
        parse_input(user_input, args);
        execute_command(args);
    }

    return 0;
}