#include "tokenizer.h"

struct TokensStruct* tokenize_input(struct CommandStruct* input_command) {
    int in_quote = 0;
    char* character = input_command->body;
    while(*character != 0) {
        if(*character == '"') {
            if(in_quote)
                in_quote = 0;
            else 
                in_quote = 1;
        }
        else if(!in_quote) {
            for(int j = 0; j < TOKEN_DELIMITERS_NUMBER; j++) {
                if(*character == TOKEN_DELIMITERS[j]) {
                    *character = 0;
                    break;
                }
            }
        }

        character++;
    }

    char** tokens = malloc(input_command->length * sizeof(char*));
    if(!tokens) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    size_t index = 0, tokens_number = 0;
    while(index < input_command->length) {
        while(input_command->body[index] == 0) {
            index++;
        }

        if(index >= input_command->length) {
            break;
        }

        tokens[tokens_number] = input_command->body + index;

        tokens_number++;
        while(input_command->body[index] != 0) {
            index++;
        }
    }

    char** result_tokens = malloc((tokens_number + 1) * sizeof(char*));
    if(!result_tokens) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    for(size_t i = 0; i < tokens_number; i++) {
        result_tokens[i] = tokens[i];
    }
    result_tokens[tokens_number] = NULL;

    free(tokens);

    struct TokensStruct* input_as_tokens = malloc(sizeof(struct TokensStruct));
    input_as_tokens->tokens = result_tokens;
    input_as_tokens->tokens_number = tokens_number;

    return input_as_tokens;
}
