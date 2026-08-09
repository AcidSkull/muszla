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

    bool in_quotes = false;
    char quote_char = '\0';

    Token* tokens = malloc(sizeof(Token) * (len + 1));
    int token_count = 0;
    
    while(right <= len && left <= right){
        
        if((input[right] == '"' || input[right] == '\'') && (right == 0 || input[right-1] != '\\')){
            if (!in_quotes) {
                in_quotes = true;
                quote_char = input[right];
            } else if (input[right] == quote_char) {
                in_quotes = false;
            }
        }

        if(in_quotes){
            right++;
            continue;
        }

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

            if(is_operator(input[right])){
                char op_buf[3] = {0};

                if(right + 1 < len && input[right] == input[right + 1] &&
                    (input[right] == '>' || input[right] == '<' || 
                    input[right] == '|' || input[right] == '&')){

                    op_buf[0] = input[right];
                    op_buf[1] = input[right + 1];

                    TokenType type;
                    switch (input[right]){
                        case '|':
                            type = TOKEN_LOGICAL_OR;
                            break;
                        case '&':
                            type = TOKEN_LOGICAL_AND;
                            break;
                        case '>':
                            type = TOKEN_REDIRECT_OUT_APPEND;
                            break;
                        case '<':
                            type = TOKEN_HERE_DOCUMENT;
                            break;
                        default:
                            continue;
                    }
                
                    add_token(tokens, &token_count, op_buf, type);
                    right++;
                } else {
                    op_buf[0] = input[right];
                
                    TokenType type;
                    switch (input[right]){
                        case '|':
                            type = TOKEN_PIPE;
                            break;
                        case '&':
                            type = TOKEN_SEND_TO_BACKGROUND;
                            break;
                        case '>':
                            type = TOKEN_REDIRECT_OUT;
                            break;
                        case '<':
                            type = TOKEN_REDIRECT_IN;
                            break;
                        default:
                            continue;
                    }
                    
                    add_token(tokens, &token_count, op_buf, type);
                }
            }

            right++;
            left = right;
        }
    }

    *token_count_ptr = token_count;
    return tokens;
}

