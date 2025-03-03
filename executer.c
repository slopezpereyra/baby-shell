#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "cmds.h"


// 
// Execute commands from a tokenized buffer.
//
int execute_cmd(struct execcmd *cmd){

  int status;
  printf("Forking...\n");
  pid_t pid = fork();

  if ( pid == 0 ){
    printf("Child: attempting to execute\n");
    if ( execvp((cmd->argv)[0], cmd->argv) == -1 ){
      printf("Failed to execute...\n");
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
