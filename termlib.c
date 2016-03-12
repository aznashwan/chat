#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "termlib.h"

// printm clears the current line and prints the given message, after
// which it progresses to a new line and sets the prompt.
void printm(char* msg) {
    if (strcmp(msg, "") == 0) {
        return;
    }

    printf(RETURN);
    printf(CLEARLINE);
    printf("%s\n", msg);
    printf(PROMPT);
    printf(RETURN);
}

// getl is a helper function which reads a line from standard
// input to the given buffer. It returns the number of bytes actually read.
int getl(char* line) {
    int rd;

    rd = getline(&line, &MAX_LINE_LEN, stdin);
    *(line + rd - 1) = '\0';
    printf("%s", strchr(line, '\0'));
    return rd;
}
