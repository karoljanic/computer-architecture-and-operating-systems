#include "reader.h"
#include "built-in-functions.h"

struct CommandStruct* read_input() {
    int buffer_size = UNIT_BUFF_SIZE;
    int buffer_position = 0;
    char character;
    char* buffer = malloc(sizeof(char) * buffer_size);
    if(!buffer) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }

    while(1) {
        character = getchar();

        if(character == EOF) {
            run_exit();
        }
        else if(character == '\n') {
            buffer[buffer_position] = '\0';

            struct CommandStruct* command = malloc(sizeof(struct CommandStruct));
            command->body = buffer;
            command->length = strlen(buffer);
            return command;
        }
        else {
            buffer[buffer_position] = character;
        }

        buffer_position++;

        if(buffer_position >= buffer_size) {
            buffer_size += UNIT_BUFF_SIZE;
            buffer = realloc(buffer, buffer_size);
            if(!buffer) {
                fprintf(stderr, "Memory allocation error\n");
                exit(1);
            }
        }
    }
}
