#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define DEFAULT_BUF_SIZE 1

//
// Allocate DEFAULT_BUF_SIZE bytes into a buffer and write that many bytes into
// it from stdin. Return the allocated buffer.
//
char* read_cmd(void){
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
char** parse_cmd(char* cmd){
  // Remove "\n" from end of line in `cmd`, if present.
  int l = strlen (cmd);
  if (l > 0 && cmd [l - 1] == '\n') cmd [l - 1] = '\0';

  int buf_size = DEFAULT_BUF_SIZE;
  // Proceed  to tokenize.
  char **tokens = malloc(buf_size * sizeof(char*));
  char *cur_token = strtok(cmd, " ");

  unsigned int i = 0;
  while (cur_token != NULL){
    tokens[i] = cur_token;

    if (i == buf_size - 1){
      buf_size += sizeof(char*) * (buf_size + DEFAULT_BUF_SIZE );
      void *tmp = realloc(tokens, buf_size);
      if (tmp == NULL){ 
        printf("Failed to extend input buffer\n");
        exit(EXIT_FAILURE);
      }
      tokens = tmp;
    }

    cur_token = strtok(NULL, " ");
    i++;
  } 

  return tokens;
}

// 
// Execute commands from a tokenized buffer.
//
int execute_cmd(char **tokens){

  int status;
  pid_t pid = fork();

  if ( pid == 0 ){
    if ( execvp(tokens[0], tokens) == -1 ){
      exit(EXIT_FAILURE);
    }
  }else if ( pid < 0 ){
      printf("Failed to fork...\n");
  }else{
    waitpid(pid, &status, WUNTRACED);
    while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;

}

//int main() {
//    char *cmd = read_cmd();
//    char** tokens = parse_cmd(cmd);
//    execute_cmd(tokens);
//    free(cmd);
//    free(tokens);
//    return 0;
//}
