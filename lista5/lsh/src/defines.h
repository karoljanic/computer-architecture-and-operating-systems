#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define UNIT_BUFF_SIZE 128
#define MAX_JOBS_NUMBER 64

#define TOKEN_DELIMITERS " \t\r\n\a"
#define TOKEN_DELIMITERS_NUMBER 5

#define STATUS_RUNNING 0
#define STATUS_DONE 1
#define STATUS_SUSPENDED 2
#define STATUS_CONTINUED 3
#define STATUS_TERMINATED 4

#define GET_ALL 0
#define GET_DONE 1
#define GET_NOT_DONE 2

#define PROMPT_STYLE "\033[32;47m"
#define CLEAR_STYLE  "\033[m"
#define CLEAR_SCREEN "\033[H\033[J"

struct CommandStruct {
    char* body;
    size_t length;
};

struct TokensStruct {
    char** tokens;
    size_t tokens_number;
};

struct ProcessStruct {
    pid_t pid;
    int built_in_command;
    int status;

    char **command;
    int command_size;

    char *input_path;
    char *output_path;
    char *error_path;

    struct ProcessStruct *next;
};

struct JobStruct {
    int id;
    pid_t pgid;
    int run_in_foreground;

    struct ProcessStruct *root;
};

struct JobStructsStoreStruct {
    struct JobStruct *jobs[MAX_JOBS_NUMBER + 1];
};

#endif // DEFINES_H
