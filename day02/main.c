#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void problem1(FILE *input);
void problem2(FILE *input);

typedef enum Command {
    Forward,
    Up,
    Down,
} Command;
int get_command(FILE *input, Command *cmd, int *operand);
int getint(FILE *input, int *out);
FILE *open_file(int argc, char **argv);

int main(int argc, char **argv) {
    FILE *input = open_file(argc, argv);
    //problem1(input);
    problem2(input);
    fclose(input);
}

void problem1(FILE *input) {
    int operand;
    Command command;
    int depth = 0;
    int horizontal = 0;

    while(get_command(input, &command, &operand) > 0) {
        switch(command) {
            case Forward: 
                horizontal += operand;
                break;
            case Up: 
                depth -= operand;
                break;
            case Down: 
                depth += operand;
                break;
        }
    }

    printf("depth: %d, horizontal: %d, product: %d", depth, horizontal, depth * horizontal);
}

void problem2(FILE *input) {
    int operand;
    Command command;
    int depth = 0;
    int horizontal = 0;
    int aim = 0;

    while(get_command(input, &command, &operand) > 0) {
        switch(command) {
            case Forward: 
                horizontal += operand;
                depth += aim * operand;
                break;
            case Up: 
                aim -= operand;
                break;
            case Down: 
                aim += operand;
                break;
        }
    }

    printf("depth: %d, horizontal: %d, aim: %d, product: %d", depth, horizontal, aim, depth * horizontal);
}

int get_command(FILE *input, Command *cmd, int *operand) {
    char buf[64];
    int i = 0;

    while(!feof(input)) {
        char c = fgetc(input);
        if(c == ' ') {
            break;
        }

        buf[i] = c;
        i += 1;
    }

    buf[i] = '\0';

    if(strcmp(buf, "forward") == 0) {
        *cmd = Forward;
    } else if(strcmp(buf, "down") == 0) {
        *cmd = Down;
    } else if(strcmp(buf, "up") == 0) {
        *cmd = Up;
    }

    return i + getint(input, operand);
}

int getint(FILE *input, int *out) {
    char buf[64];
    int i = 0;

    while(!feof(input)) {
        char c = fgetc(input);
        if(c == '\n') {
            break;
        }

        buf[i] = c;
        i += 1;
    }

    *out = atoi(buf);

    return i;
}

FILE *open_file(int argc, char **argv) {
    if(argc < 2) {
        return stdin;
    } else {
        FILE *f = fopen(argv[1], "r");
        if(f == NULL) {
            fprintf(stderr, "[ERROR] File not found: %s", argv[1]);
            exit(1);
        }

        return f;
    }
}

