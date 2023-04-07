#include "lsh.h"

void lsh() {
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);

    struct JobStructsStoreStruct* jobs_store = (struct JobStructsStoreStruct*) malloc(sizeof(struct JobStructsStoreStruct));
    for(int i = 0; i < MAX_JOBS_NUMBER; i++) {
        jobs_store->jobs[i] = NULL;
    }

    pid_t pid = getpid();
    setpgid(pid, pid);
    tcsetpgrp(0, pid);

    signal(SIGINT, interrupt_handler);
    
    struct CommandStruct* input;
    struct TokensStruct* tokens;
    struct JobStruct* job;

    printf(CLEAR_SCREEN);

    while(1){
        char* current_directory = get_current_directory();
        printf(PROMPT_STYLE"%s|>"CLEAR_STYLE" ", current_directory);

        input = read_input();
        if(input->length != 0) {
            tokens = tokenize_input(input);
            job = parse_input(tokens);
            execute_job(jobs_store, job);
        }

        fight_with_zombies(jobs_store);
    }
}

void interrupt_handler() {
    
}

char* get_current_directory() {
    size_t buffer_size = UNIT_BUFF_SIZE;
    char* buffer = malloc(buffer_size * sizeof(char));
    if(!buffer) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    while(getcwd(buffer, buffer_size) == NULL) {
        buffer_size += UNIT_BUFF_SIZE;
        buffer = realloc(buffer, buffer_size * sizeof(char));
        if(!buffer) {
            fprintf(stderr, "Memory allocation error\n");
            exit(1);
        }
    }

    return buffer;
}
