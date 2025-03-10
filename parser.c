#include "parser.h"
#include <stdio.h> 
#include <stdbool.h> 
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
#include <stdio.h>
#include <stdlib.h>

char* read_stdin(void) {
    fflush(stdout);

    char *buffer = NULL;  // `getline` will allocate memory
    size_t buf_size = 0;  // `getline` will update this
    ssize_t len;

    len = getline(&buffer, &buf_size, stdin); // Read a line from stdin

    if (len == -1) { // Handle EOF or error
        printf("Reached EOF or read error\n");
        free(buffer);
        return NULL;
    }

    return buffer;  // The caller is responsible for freeing `buffer`
}

cmd_type parse_abstract_cmd(char* buff){

  bool is_pipe = false;
  for (int i = 0; buff[i] != '\0'; i++){
    if ( buff[i] == ';' ){
      return YUX;
    }
    if ( buff[i] == '|' && !is_pipe){
      is_pipe = true;
    }
  }

  if (is_pipe)  return PIPE;
  return EXEC;

}

//
// Tokenize a buffer splitting it into space-separated tokens.
//
struct execcmd * parse_exec_cmd(char* buffer){

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

  //for (unsigned int i = 0; i < cmd -> n_args + 1; i++){
  //  printf("Argument %d : %s\n", i, (cmd->argv)[i]);
  //}

  return cmd;
}

void parse_yux_cmd(char *buff){
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
}

// Parse the command buffer of a piped command, initializing a 
// pipecmd struct pointer.
struct pipecmd *parse_pipe_cmd(char *buff){
  
  struct pipecmd *pcmd = init_pipe_cmd();

  for (int i = 0; buff[i] != '\0'; i++){
    if (buff[i] == '|'){
      buff[i] = '\0';
      pcmd -> left = parse_exec_cmd(buff);
      pcmd -> right = buff + i + 1;
      break;
    }
  }
  return pcmd;
}

// Recursive parsing of stdin into tree representation.
void tree_parse(char *buff){

  int l = strlen (buff);
  if (l > 0 && buff [l - 1] == '\n') buff [l - 1] = '\0';

  cmd_type t = parse_abstract_cmd(buff);

  if (t == YUX){
    // Calls tree_parse recursively on each node.
    parse_yux_cmd(buff);
    return;
  }

  if (t == PIPE){
    struct pipecmd *execution_cmd =  parse_pipe_cmd(buff);
    int original_stdin = dup(STDIN_FILENO);  // Save original stdin
    execute_pipeline(execution_cmd);
    dup2(original_stdin, STDIN_FILENO);  // Restore stdin to original
    close(original_stdin);  
    printf("My Bash: ");
    return;
  }

  struct execcmd *execution_cmd =  parse_exec_cmd(buff);
  execute_cmd(execution_cmd);
//  struct pipecmd *execution_cmd =  parse_pipe_cmd(buff);
  printf("My Bash: ");
      
}

