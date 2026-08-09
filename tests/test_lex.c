#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "test.h"

#define TESTS_NUMBER 5

int main(){
    char *questions_strings[] = {
        "test string",
        "test | operator > in >> shell",
        "more<operator<<to&test&&|something||",
        "test 'word in apostrophes'",
        "test \"word in quotation marks\""
    };

    Token* answears[] = {
        (Token[]) { {.type = TOKEN_WORD, .value = "test"}, {.type = TOKEN_WORD, .value = "string"} }, 
        (Token[]) { {.type = TOKEN_WORD, .value = "test"}, {.type = TOKEN_PIPE, .value = "|"},
                    {.type = TOKEN_WORD, .value = "operator"}, {.type = TOKEN_REDIRECT_OUT, .value = ">"},
                    {.type = TOKEN_WORD, .value = "in"}, {.type = TOKEN_REDIRECT_OUT_APPEND, .value = ">>"},
                    {.type = TOKEN_WORD, .value = "shell"} }, 
        (Token[]) { {.type = TOKEN_WORD, .value = "more"}, {.type = TOKEN_REDIRECT_IN, .value = "<"},
                    {.type = TOKEN_WORD, .value = "operator"}, {.type = TOKEN_HERE_DOCUMENT, .value = "<<"},
                    {.type = TOKEN_WORD, .value = "to"}, {.type = TOKEN_SEND_TO_BACKGROUND, .value = "&"},
                    {.type = TOKEN_WORD, .value = "test"}, {.type = TOKEN_LOGICAL_AND, .value = "&&"},
                    {.type = TOKEN_PIPE, .value = "|"}, {.type = TOKEN_WORD, .value = "something"},
                    {.type = TOKEN_LOGICAL_OR, .value = "||"} },
        (Token[]) { {.type = TOKEN_WORD, .value = "test"}, {.type = TOKEN_WORD, .value = "'word in apostrophes'"} }, 
        (Token[]) { {.type = TOKEN_WORD, .value = "test"}, {.type = TOKEN_WORD, .value = "\"word in quotation marks\""} }
    };
    
    int answeas_length[TESTS_NUMBER] = {2, 7, 11, 2, 2}; 

    for(int i = 0; i < TESTS_NUMBER; i++){
        int counter = 0;
        Token *question = analize(questions_strings[i], &counter);
        assert_equals_tokens(question, answears[i], answeas_length[i]);
        free(question);
    }

    return 0;
}
