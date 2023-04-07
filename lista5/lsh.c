#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_BUFFER_SIZE 1024
#define DELIMITERS " \t\r\n\a"
#define BUILT_IN_FUNCTIONS_NUMBER 2
#define clear() printf("\033[H\033[J")

int terminate = 0;
int buffer_size = 0;
char* buffer;
char** tokens;
char current_directory[INIT_BUFFER_SIZE];

void set_current_directory() {
    getcwd(current_directory, sizeof(current_directory));
}

int custom_cd(char** args);
int custom_exit(char** args);

char* built_in_functions_names[BUILT_IN_FUNCTIONS_NUMBER] = {
    "cd", "exit"
};

int (*built_in_functions[]) (char **) = {
    &custom_cd, &custom_exit
};

int custom_cd(char** args) {
    if(args[1] == NULL) {
        fprintf(stderr, "The \"cd\" function requires 1 argument\n");
    }
    else if(chdir(args[1]) != 0) {
        fprintf(stderr, "Error executing \"cd\"\n");
    }

    set_current_directory();

    return 1;
}

int custom_exit(char** args) {
    terminate = 1;
    return 0;
}

void read_input() {
    buffer_size = INIT_BUFFER_SIZE;
    int buffer_position = 0;
    char c;
    buffer = malloc(sizeof(char) * buffer_size);
    if(!buffer) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    while(1) {
        c = getchar();

        if(c == EOF) {
            custom_exit(NULL);
            return;
        }
        else if(c == '\n') {
            buffer[buffer_position] = '\0';
            return;
        }
        else {
            buffer[buffer_position] = c;
        }

        buffer_position++;

        if(buffer_position >= buffer_size) {
            buffer_size += INIT_BUFFER_SIZE;
            buffer = realloc(buffer, buffer_size);
            if(!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                exit(1);
            }
        }
    }
}

void parse_input() {
    int token_position = 0;
    char* token;
    tokens = malloc(buffer_size * sizeof(char*));
    if(!tokens) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    token = strtok(buffer, DELIMITERS);
    while(token != NULL) {
        tokens[token_position] = token;
        token_position++;
        token = strtok(NULL, DELIMITERS);
    }
    tokens[token_position] = NULL;
}

int execute_input() {
    if(tokens[0] == NULL) {
        return 1;
    }

    for(int i = 0; i < BUILT_IN_FUNCTIONS_NUMBER; i++) {
        if(strcmp(tokens[0], built_in_functions_names[i]) == 0) {
            return (*built_in_functions[i])(tokens);
        }
    }

    pid_t process_id, wait_for_child_end;
    int status;

    process_id = fork();
    if(process_id < 0) { // fork error
        fprintf(stderr, "Fork error\n");
    }
    else if(process_id == 0) { // child
        if(execvp(tokens[0], tokens) == -1) {
            fprintf(stderr, "Error executing %s\n", tokens[0]);
        }
        
        return 1;
    }
    else { // parent
        do {
            wait_for_child_end = waitpid(process_id, &status, WUNTRACED);
        } 
        while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int main() { 
    clear();
    set_current_directory();

    while(terminate == 0) {
        printf("\033[38;5;3m%s\033[0m\033[38;5;11m|> \033[0m", current_directory);
        read_input();
        parse_input();
        execute_input();

        free(buffer);
        free(tokens);
    }

    clear();

    return 0;
}