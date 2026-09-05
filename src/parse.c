#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"


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
