#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "test.h"

bool assert_equals_char(char* question, char* answear){
    if (question == NULL || answear == NULL) {
        return false;
    }
    
    if (strcmp(question, answear) == 0) {
        return true;
    } else {
        return false;
    }
}

bool assert_equals_tokens(Token* question, Token* answer, int length){
    for(int i = 0; i < length; i++){
        if(question[i].type != answer[i].type){
            printf("\033[31m[!] Test Error: Token type %d of value \"%s\" is not equal to %d in test number %d\033[0m\n", 
                    question[i].type,
                    question[i].value,
                    answer[i].type,
                    i);
            return false;
        }

        if(strcmp(question[i].value, answer[i].value) != 0){
            printf("\033[31m[!] Test Error: Token value %s is not equal to %s in test number %d\033[0m\n", 
                    question[i].value, 
                    answer[i].value,
                    i);
            return false;
        }
    }

    return true;
}

