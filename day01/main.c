#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void problem1(FILE *input);
void problem2(FILE *input);
FILE *open_file(int argc, char **argv);
int getint(FILE *input, int *out);

int main(int argc, char **argv) {
    FILE *input = open_file(argc, argv);
    //problem1(input);
    problem2(input);
    fclose(input);
}

void problem1(FILE *input) {
    int increments = 0;

    int cur;
    int prev;
    getint(input, &prev);

    while(getint(input, &cur)) {
        if(cur > prev) {
            increments += 1;
        }

        int tmp = prev;
        prev = cur;
        cur = tmp;
    }

    printf("%d", increments);
}

void problem2(FILE *input) {
    int increments = 0;

    int cur;
    int prev = 0;

    int window[3];
    getint(input, window);
    getint(input, window+1);
    
    while(getint(input, window+2)) {
        cur = window[0] + window[1] + window[2];
        if(cur > prev) {
            increments += 1;
        }

        window[0] = window[1];
        window[1] = window[2];

        int tmp = prev;
        prev = cur;
        cur = tmp;
    }

    printf("%d", increments - 1);
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
