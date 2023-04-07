#ifndef PARSER_H
#define PARSER_H

#include "defines.h"

struct JobStruct* parse_input(struct TokensStruct* input_tokens);
int parse_single_process(char** start_token, char** end_token, struct ProcessStruct* process);

#endif // PARSER_H
