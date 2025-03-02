#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Define the default buffer size used for reading commands
#define DEFAULT_BUF_SIZE 1

// Function to read a command from stdin and return it as a dynamically allocated buffer
char* read_stdin(void);

// Function to parse a command string into space-separated tokens
struct execcmd * parse_exec_cmd(char* cmd);

// Function to execute a command by forking a new process and running the command using execvp
int execute_cmd(struct execcmd *cmd);


#endif // PARSER_H
