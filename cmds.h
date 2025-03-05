#ifndef CMDS_H
#define CMDS_H


#define ABSTRACT -1
#define EXEC 0
#define YUX 1 // Yuxtaposed commands: ";" operator.
#define PIPE 2
#define MAX_ARGS 10

typedef unsigned int cmd_type;

struct execcmd {
  cmd_type type;
  unsigned int n_args;
  char* argv[MAX_ARGS + 1];
  char* arge[MAX_ARGS + 1];
};


struct pipecmd {
  cmd_type type;
  struct execcmd *left;
  char *right;
};


struct execcmd *init_exec_cmd();
struct pipecmd *init_pipe_cmd();
cmd_type parse_abstract_cmd(char* buff);

#endif // CMDS_H
