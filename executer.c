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

  char* program = (cmd -> argv)[0];

  if (program == NULL) exit(0);
  if ( strcmp(program, "exit") == 0 ) exit(1);

  int status;
  printf("Forking...\n");
  pid_t pid = fork();

  if ( pid == 0 ){
    printf("Child: attempting to execute\n");
    if ( execvp(program, cmd->argv) == -1 ){
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



// The idea is this: 
// Create a uni-directional channel of communication (a pipe) between the two
// process so process A can communicate its output to process B. The
// communication will be indirect: 
//
// - Process A will write its output on the write end of the pipe 
// - Process B will read its input from the read end of the pipe. 
//
// To do this:
//
// - Process A must duplicate STDOUT to the write end.
// - Process B must duplicate STDIN  to the read end.
int execute_pipeline(struct pipecmd *pipe_cmd){

  int pipefds[2];
  int pipe_code = pipe(pipefds);
  int status;

  pid_t pid = fork();

  if (pid < 0) exit(0);

  if (pid == 0){

    char* left_pipe_program = ( pipe_cmd -> left ) -> argv[0];
    dup2(pipefds[1], STDOUT_FILENO);
    if ( execvp(left_pipe_program, (pipe_cmd -> left)->argv) == -1 ){
      printf("Failed to execute...\n");
      exit(EXIT_FAILURE);
    }

  }else{
    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process terminated by signal %d\n", WTERMSIG(status));
        }
    close(pipefds[1]);
    dup2(pipefds[0], STDIN_FILENO);
    execute_cmd(pipe_cmd -> right);
    close(pipefds[0]);
  }

  return 1;

}








