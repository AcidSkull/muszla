#ifndef TEST_H
#define TEST_H

#include <stdbool.h>

#include "main.h"

bool assert_equals_char(char* question, char* answear);
bool assert_equals_tokens(Token* question, Token* answer, int length);

#endif