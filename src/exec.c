#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include "main.h"

static int apply_redirections(Redirection *redir) {
    while (redir != NULL) {
        int fd = -1;

        if (redir->type == REDIR_IN) {
            fd = open(redir->filename, O_RDONLY);
            if (fd < 0) {
                return -1;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } 
        else if (redir->type == REDIR_OUT) {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } 
        else if (redir->type == REDIR_APPEND) {
            fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        redir = redir->next;
    }
    return 0;
}

static bool is_builtin(char **argv) {
    if (!argv || !argv[0]) return false;

    if (strcmp(argv[0], "cd") == 0) return true;
    if (strcmp(argv[0], "exit") == 0) return true;

    return false;
}

static int execute_builtin(char** argv){
    if (strcmp(argv[0], "cd") == 0) {
        const char *path = argv[1];
        if (!path) {
            path = getenv("HOME");
        }
        if (chdir(path) != 0) {
            return 1;
        }
        return 0;
    }

    if (strcmp(argv[0], "exit") == 0) {
        int exit_code = 0;
        if (argv[1]) {
            exit_code = atoi(argv[1]);
        }
        exit(exit_code);
    }

    return 1;
}

static int execute_cmd(AST* ast){
    if(!ast || ast->data.cmd.argc == 0 || ast->data.cmd.argv[0] == NULL){
        return 0;
    }

    if(is_builtin(ast->data.cmd.argv)){
        return execute_builtin(ast->data.cmd.argv);
    }

    pid_t pid = fork();
    if(pid < 0){
        return 1;
    }

    if(pid == 0){
        if(apply_redirections(ast->data.cmd.redirects) < 0){
            exit(1);
        }

        execvp(ast->data.cmd.argv[0], ast->data.cmd.argv);

        perror(ast->data.cmd.argv[0]);
        exit(127);
    }

    int status;
    waitpid(pid, &status, 0);

    if(WIFEXITED(status)){
        return WEXITSTATUS(status);
    }

    return 1;
}

static int execute_logic(AST* ast){
    int left_status = execute(ast->data.logic.left);

    if(ast->data.logic.op == 0){
        if(left_status == 0){
            return execute(ast->data.logic.right);
        }
        return left_status;
    } else {
        if(left_status != 0){
            return execute(ast->data.logic.right);
        }
        return left_status;
    }
}

static int execute_pipe(AST* ast){
    int pfd[2];
    if(pipe(pfd) < 0) return 1;

    pid_t pid1 = fork();
    if(pid1 == 0){
        dup2(pfd[1], STDOUT_FILENO);
        close(pfd[0]);
        close(pfd[1]);
        exit(execute(ast->data.pipe.left));
    }

    pid_t pid2 = fork();
    if(pid2 == 0){
        dup2(pfd[0], STDIN_FILENO);
        close(pfd[0]);
        close(pfd[1]);
        exit(execute(ast->data.pipe.right));
    }

    close(pfd[0]);
    close(pfd[1]);

    int status1, status2;
    waitpid(pid1, &status1, 0);
    waitpid(pid2, &status2, 0);

    if (WIFEXITED(status2)) {
        return WEXITSTATUS(status2);
    }
    return 1;
}

int execute(AST* ast){
    if(!ast) return 0;

    switch(ast->tag){
        case AST_CMD:
            return execute_cmd(ast);
        case AST_LOGIC_OP:
            return execute_logic(ast);
        case AST_PIPE:
            return execute_pipe(ast);
        default:
            return 1;
    }
}
