#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

AST* parse_logic(Token* tokens, int* index);
AST* parse_pipe(Token* tokens, int* index);
AST* parse_command(Token* tokens, int* index);

AST* create_ast_node(int tag){
    AST *node = (AST*)malloc(sizeof(AST));
    if(!node){
        exit(1);
    }
    node->tag = tag;
    return node;
}

Redirection* create_redirection(RedirType type, const char* filename){
    Redirection* r = (Redirection*)malloc(sizeof(Redirection));
    if(!r){
        exit(1);
    }
    r->type = type;
    r->filename = strdup(filename);
    r->next = NULL;
    return r;
}

AST* create_ast_cmd(int argc, char** argv){
    AST* node = create_ast_node(AST_CMD);
    node->data.cmd.argv = argv;
    node->data.cmd.argc = argc;
    node->data.cmd.redirects = NULL;
    return node;
}

void add_redirection(AST* node, Redirection* redir){
    if(node->tag != AST_CMD) return;

    if(node->data.cmd.redirects == NULL){
        node->data.cmd.redirects = redir;
    } else {
        Redirection* current = node->data.cmd.redirects;
        while(current-> next != NULL){
            current = current->next;
        }
        current->next = redir;
    }
}

AST* create_ast_pipe(AST* l, AST* r){
    AST* node = create_ast_node(AST_PIPE);
    node->data.pipe.left = l;
    node->data.pipe.right = r;
    return node;
}

AST* create_ast_logic(int op, AST* l, AST* r){
    AST* node = create_ast_node(AST_LOGIC_OP);
    node->data.logic.op = op;
    node->data.logic.left = l;
    node->data.logic.right = r;
    return node;
}

void free_redirections(Redirection* redir){
    while(redir != NULL){
        Redirection* temp = redir;
        redir = redir->next;
        free(temp->filename);
        free(temp);
    }
}

void free_ast(AST* node){
    if(!node) return;

    switch(node->tag){
        case AST_CMD:
            if(node->data.cmd.argv){
                for(int i = 0; i < node->data.cmd.argc; i++){
                    free(node->data.cmd.argv[i]);
                }
                free(node->data.cmd.argv);
            }

            free_redirections(node->data.cmd.redirects);
            break;
        case AST_PIPE: 
            free_ast(node->data.pipe.left);
            free_ast(node->data.pipe.right);
            break;
        case AST_LOGIC_OP:
            free_ast(node->data.logic.left);
            free_ast(node->data.logic.right);
            break;
    }

    free(node);
}

AST* parse_logic(Token* tokens, int* index){
    AST* left = parse_pipe(tokens, index);
    if(!left) return NULL;

    Token token = tokens[*index];
    while(token.type == TOKEN_LOGICAL_AND || token.type == TOKEN_LOGICAL_OR){
        token = tokens[(*index)++];
        int op = (token.type == TOKEN_LOGICAL_AND) ? 0 : 1;

        AST* right = parse_pipe(tokens, index);
        if(!right){
            return left;
        }
        left = create_ast_logic(op, left, right);
        token = tokens[*index];
    }

    return left;
}

AST* parse_pipe(Token* tokens, int* index){
    AST* left = parse_command(tokens, index);
    if(!left) return NULL;

    Token token = tokens[*index];
    while(token.type == TOKEN_PIPE){
        token = tokens[(*index)++];

        AST* right = parse_command(tokens, index);
        if(!right){
            return left;
        }
        left = create_ast_pipe(left, right);
        token = tokens[*index];
    }

    return left;
}

AST* parse_command(Token* tokens, int* index){
    if (tokens[*index].type == TOKEN_PIPE || 
        tokens[*index].type == TOKEN_LOGICAL_AND || 
        tokens[*index].type == TOKEN_LOGICAL_OR) {
            return NULL; 
    }

    int capacity = 8;
    int argc = 0;
    char** argv = malloc(capacity * sizeof(char*));
    if (!argv) exit(1);

    AST* cmd_node = create_ast_cmd(0, NULL);

    while (tokens[*index].type != TOKEN_PIPE &&
           tokens[*index].type != TOKEN_LOGICAL_AND &&
           tokens[*index].type != TOKEN_LOGICAL_OR) {

        Token token = tokens[*index];

        if (token.type == TOKEN_WORD) {
            if (argc >= capacity - 1) {
                capacity *= 2;
                argv = realloc(argv, capacity * sizeof(char*));
                if (!argv) exit(1);
            }
            argv[argc++] = strdup(token.value);
            (*index)++;
        } 
        else if (token.type == TOKEN_REDIRECT_IN || 
                 token.type == TOKEN_REDIRECT_OUT || 
                 token.type == TOKEN_REDIRECT_OUT_APPEND) {
            
            RedirType rtype;
            if (token.type == TOKEN_REDIRECT_IN) rtype = REDIR_IN;
            else if (token.type == TOKEN_REDIRECT_OUT) rtype = REDIR_OUT;
            else rtype = REDIR_APPEND;

            (*index)++; 

            if (tokens[*index].type != TOKEN_WORD) {
                break;
            }

            add_redirection(cmd_node, create_redirection(rtype, tokens[*index].value));
            (*index)++;
        } 
        else {
            break;
        }
    }

    argv[argc] = NULL;
    cmd_node->data.cmd.argv = argv;
    cmd_node->data.cmd.argc = argc;

    if (argc == 0 && cmd_node->data.cmd.redirects == NULL) {
        free(argv);
        free(cmd_node);
        return NULL;
    }

    return cmd_node;
}

AST* parse(Token* tokens){
    int index = 0;
    return parse_logic(tokens, &index);
}
