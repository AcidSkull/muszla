#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"

bool is_delimiter(char chr){
    return (chr == ' ' || chr == '\t' || chr == '\n' || 
            chr == '|' || chr == '<' || chr == '>' || 
            chr == ';' || chr == '&');
}

bool is_operator(char chr){
    return (chr == '|' || chr == '<' || chr == '>' || 
            chr == ';' || chr == '&');
}

void add_token(Token* tokens, int *index, char* value, TokenType type){
    tokens[*index].type = type;
    tokens[*index].value = strdup(value);
    (*index)++;
}

Token* analize(char *input, int *token_count_ptr){
    int left = 0, right = 0;
    int len = strlen(input);

    bool in_quote = false;
    char quote_char = '\0';

    Token* tokens = malloc(sizeof(Token) * (len + 1));
    int token_count = 0;
    
    while(right <= len && left <= right){
        if(!is_delimiter(input[right]) && input[right] != '\0'){
            right++;
        } else {
            if(left != right){
                int len_sub = right - left;
                char* sub_string = (char*)malloc(len_sub + 1);

                if(sub_string != NULL){
                    strncpy(sub_string, input + left, len_sub);
                    sub_string[len_sub] = '\0';

                    if(strlen(sub_string) > 0 && sub_string[0] != ' '){
                        add_token(tokens, &token_count, sub_string, TOKEN_WORD);
                    }

                    free(sub_string);
                }

            }

            right++;
            left = right;
        }
    }

    *token_count_ptr = token_count;
    return tokens;
}

