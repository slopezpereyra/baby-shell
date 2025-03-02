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
  char* argv[MAX_ARGS];
  char* arge[MAX_ARGS];
};


struct pipecmd {
  cmd_type type;
  struct cmd *left;
  struct cmd *right;
};

#endif // CMDS_H
