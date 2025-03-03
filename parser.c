#include "parser.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"
#include "executer.h"

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

// Recursive tree parsing.
// strtok is modifying buff
void parse_stdin(char* buff){
  // Ensure the buffer is null-terminated.
  int l = strlen (buff);
  if (l > 0 && buff [l - 1] == '\n') buff [l - 1] = '\0';

  char *cur_token = malloc(sizeof(char)*l);
  struct execcmd *execution_cmd = init_exec_cmd();
  char* program;
  int i = 0;
  int j = 0;

  while (1){
    if (buff[i] != '\0' && buff[i] != ';'){
      cur_token[i] = buff[j];
      i++;
      j++;
    }
    else {
      execution_cmd =  parse_exec_cmd(cur_token);
      program = (execution_cmd -> argv)[0];

      if (program == NULL) exit(0);
      if ( strcmp(program, "exit") == 0 ) exit(1);

      execute_cmd(execution_cmd);
      
      // Clear the array by setting all characters to '\0'
      for (int i = 0; i < l; i++) {
          cur_token[i] = '\0';
      }
      i = 0;
      j++;
      printf("MyBash : ");
      if (buff[j] == '\0') break;
    }

  }
  return;
}
  


//int main() {
//    char *cmd = read_stdin();
//    char** tokens = parse_cmd(cmd);
//    execute_cmd(tokens);
//    free(cmd);
//    free(tokens);
//    return 0;
//}
