#ifndef CMDS_H
#define CMDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define EXEC 0
#define REDIRECTOR 1
#define PIPE 2
#define MAX_ARGS 10

typedef unsigned int cmd_type;

struct cmd {
  cmd_type type;
};

struct execcmd {
  cmd_type type;
  unsigned int n_args;
  char* argv[MAX_ARGS + 1];
  char* arge[MAX_ARGS + 1];
};


struct pipecmd {
  cmd_type type;
  struct execcmd *left;
  struct execcmd *right;
};


struct execcmd *init_exec_cmd();

#endif // CMDS_H
