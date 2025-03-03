#include "parser.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"
#include "executer.h"

//
// Allocate DEFAULT_BUF_SIZE bytes into a buffer and write that many bytes into
// it from stdin. Return the allocated buffer.
//
char* read_stdin(void){
  int buf_size = DEFAULT_BUF_SIZE;
  int pos = 0;
  char *buffer = malloc(sizeof(char)*buf_size);
  int ch;

  while (( ch = fgetc(stdin)) != EOF){
    if (ch == '\n') {
      break;
    }
    buffer[pos] = ch;

    if (pos == buf_size - 1){
      buf_size += sizeof(char) * (buf_size + DEFAULT_BUF_SIZE );
      void *tmp = realloc(buffer, buf_size);
      if (tmp == NULL){ 
        printf("Failed to extend input buffer\n");
        exit(EXIT_FAILURE);
      }
      buffer = tmp;
    }

    pos++;

  }
  return buffer;
}

//
// Tokenize a buffer splitting it into space-separated tokens.
//
struct execcmd * parse_exec_cmd(char* buffer){
  // Remove "\n" from end of line in `cmd`, if present.

  struct execcmd *cmd = init_exec_cmd();
  char *cur_token = strtok(buffer, " ");

  while (cur_token != NULL){
    if (( cmd -> n_args ) > MAX_ARGS){
      printf("Exceeded maximum number of tokens\n");
      exit(0);
    }
    (cmd -> argv)[cmd->n_args] = cur_token;
    (cmd -> n_args)++;
    cur_token = strtok(NULL, " ");
  }

  for (unsigned int i = 0; i < cmd -> n_args + 1; i++){
    printf("Argument %d : %s\n", i, (cmd->argv)[i]);
  }

  return cmd;
}

// Recursively parse a cmd buffer as a tree. Split each ";"-separated block into
// two leaves and parse them separately. Base case is a buffer without ";".
void tree_parse(char *buff){

  int l = strlen (buff);
  if (l > 0 && buff [l - 1] == '\n') buff [l - 1] = '\0';
  int i = 0;

  while (buff[i] != '\0'){

    if (buff[i] == ';'){
      // We use pointer arithmetic: ';' is separated by '\0' and two new
      // pointers to the same memory block `buff` are given: one pointing at the
      // beginning of the block, one pointing exactly after '\0'.
      buff[i] = '\0';
      char * node_left = buff;
      char * node_right = buff + i + 1;
      tree_parse(node_left);
      tree_parse(node_right);
      return;
    }
    i++;
  }
  // HERE we should NOT parse_exec_cmd but parse_abstract_cmd,
  // detect the cmd_type, cast to the corresponding struct (pipecmd or 
  // exec_cmd, for instance), and then resolve to execution. We still need a 
  // parse_pipe_cmd function that correctly creates an instance of pipecmd.
  struct execcmd *execution_cmd =  parse_exec_cmd(buff);
  execute_cmd(execution_cmd);
  printf("My Bash: ");
      
}

