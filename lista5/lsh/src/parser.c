#include "parser.h"
#include "defines.h"

struct JobStruct* parse_input(struct TokensStruct* input_tokens) {
    size_t args_count = input_tokens->tokens_number;
    struct JobStruct* result_job = malloc(sizeof(struct JobStruct));

    if(strcmp(input_tokens->tokens[args_count - 1], "&") == 0) {
        input_tokens->tokens[args_count - 1] = NULL;
        args_count--;
        result_job->run_in_foreground = 0;
    }
    else {
        result_job->run_in_foreground = 1;
    }

    char** start_token = input_tokens->tokens;
    char** token = input_tokens->tokens;

    result_job->root = NULL;
    struct ProcessStruct* last_process = NULL;

    while(*token != NULL) {
        if(strcmp(*token, "|") == 0) {
            struct ProcessStruct* process = malloc(sizeof(struct ProcessStruct));
            if(result_job->root == NULL) {
                result_job->root = process;
                last_process = process;
            }
            else {
                last_process->next = process;
                last_process = process;
            }
            *token = NULL;

            parse_single_process(start_token, token, process);
            start_token = token + 1;
        }

        token++;
    }

    struct ProcessStruct* process = malloc(sizeof(struct ProcessStruct));
    if(result_job->root == NULL) {
        result_job->root = process;
        process->next = NULL;
    }
    else {
        last_process->next = process;
        process->next = NULL;
    }
    parse_single_process(start_token, token, process);

    result_job->id = -1;
    result_job->pgid = -1;

    return result_job;
}

int parse_single_process(char** start_token, char** end_token, struct ProcessStruct* process) {
    process->input_path = NULL;
    process->output_path = NULL;
    process->error_path = NULL;

    char** token = start_token;
    while(token != end_token) {
        int shift = 0;
        if(strcmp(*token, ">") == 0) {
            process->output_path = *(token+1);
            shift = 1;
        }
        else if(strcmp(*token, "<") == 0) {
            process->input_path = *(token+1);
            shift = 1;
        }
        else if(strcmp(*token, "2>") == 0) {
            process->error_path = *(token+1);
            shift = 1;
        }

        if(shift) {
            token += 2;
            *(token-1) = NULL;
            *(token-2) = NULL;
        }
        else {
            token++;
        }
    }

    token = start_token;
    while(*token == NULL) {
        token++;
    }
    process->command = token;

    size_t argc = 0;
    while(token != NULL && token != end_token) {
        if(*token != NULL)
            argc++;
        token++;
    }

    process->command_size = argc;
    process->pid = -1;

    process->built_in_command = 0;
    token = process->command;
    if(strcmp(*token, "exit") == 0 || strcmp(*token, "cd") == 0 || strcmp(*token, "fg") == 0 ||strcmp(*token, "bg") == 0 || strcmp(*token, "kill") == 0 || strcmp(*token, "jobs") == 0) {
        process->built_in_command = 1;
    }

    return 0;
}
