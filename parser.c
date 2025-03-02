#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define DEFAULT_BUF_SIZE 1024

//
// Allocate DEFAULT_BUF_SIZE bytes into a buffer and write that many bytes into
// it from stdin. Return the allocated buffer.
//
char* read_cmd(void){
  int buf_size = DEFAULT_BUF_SIZE;
  int pos = 0;
  char *buffer = malloc(sizeof(char)*buf_size);

  size_t bytes_read = fread(buffer, 1, buf_size, stdin);
  printf("Read %u bytes from stdin\n", bytes_read);
   //(stdin, )
  return buffer;

}

//
// Tokenize a buffer splitting it into space-separated tokens.
//
char** parse_cmd(char* cmd){
  // Remove "\n" from end of line in `cmd`, if present.
  int l = strlen (cmd);
  if (l > 0 && cmd [l - 1] == '\n') cmd [l - 1] = '\0';
  // Proceed  to tokenize.
  char **tokens = malloc(DEFAULT_BUF_SIZE * sizeof(char*));
  char *cur_token = strtok(cmd, " ");

  unsigned int i = 0;
  while (cur_token != NULL){
    tokens[i] = cur_token;
    i++;
    cur_token = strtok(NULL, " ");
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

// Main function.
void shell(void){
  char *cmd = read_cmd();
  char **tokens = parse_cmd(cmd);

}

int main() {
    char *cmd = read_cmd();
    char** tokens = parse_cmd(cmd);
    execute_cmd(tokens);
    free(cmd);
    free(tokens);
    return 0;
}
