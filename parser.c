#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"

#define DEFAULT_BUF_SIZE 1

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
  int l = strlen (buffer);
  if (l > 0 && buffer [l - 1] == '\n') buffer [l - 1] = '\0';

  struct execcmd *cmd = malloc(sizeof(struct execcmd *));
  (cmd->n_args) = 0;

  int buf_size = DEFAULT_BUF_SIZE;
  // Proceed  to tokenize.
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

  for (unsigned int i = 0; i < cmd -> n_args; i++){
    printf("Argument %d : %s\n", i, (cmd->argv)[i]);
  }

  return cmd;
}



//int main() {
//    char *cmd = read_stdin();
//    char** tokens = parse_cmd(cmd);
//    execute_cmd(tokens);
//    free(cmd);
//    free(tokens);
//    return 0;
//}
