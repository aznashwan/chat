#ifndef _TERMLIB_H
#define _TERMLIB_H

#define PROMPT " <"

static unsigned long MAX_LINE_LEN = 1024;

// some codes to use:
#define RETURN "\r"
#define UP "\033[1A"
#define CLEARLINE "\033[K"

// printm clears the current line and prints the given message, after
// which it progresses to a new line and sets the prompt.
void printm(char* msg);

// getl is a helper function which reads a line from standard
// input to the given buffer. It returns the number of bytes actually read.
int getl(char* line);

#endif
