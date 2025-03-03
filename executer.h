#ifndef EXECUTER_H
#define EXECUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Function to execute a command by forking a new process and running the command using execvp
int execute_cmd(struct execcmd *cmd);
int execute_pipeline(struct pipecmd *pipe_cmd);

#endif // EXECUTER_H
