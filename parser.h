#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Define the default buffer size used for reading commands
#define DEFAULT_BUF_SIZE 16

// Function to read a command from stdin and return it as a dynamically allocated buffer
char* read_stdin(void);

// Function to parse a command string into space-separated tokens
struct execcmd * parse_exec_cmd(char* cmd);

void parse_stdin(char* buff);

void tree_parse(char *buff);

#endif // PARSER_H
