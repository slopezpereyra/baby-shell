#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"
#include "cmds.h"


// 
// Execute commands from a tokenized buffer.
//
int execute_cmd(struct execcmd *cmd){


  char* program = (cmd -> argv)[0];

  if (program == NULL) exit(0);
  if ( strcmp(program, "exit") == 0 ) exit(1);

  int status;
  pid_t pid = fork();

  if ( pid == 0 ){
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
// UPDATE THIS DESCRIPTION
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
//  
//  ls | wc -l | awk '{print $1 + 1}'
int execute_pipeline(struct pipecmd *pipe_cmd){

  int pipefds[2];
  int pipe_code = pipe(pipefds);
  int status;

  char* left_pipe_program = ( pipe_cmd -> left ) -> argv[0];
  char* right_pipe_buffer = pipe_cmd -> right;
  if (left_pipe_program == NULL || right_pipe_buffer == NULL) exit (1);
  cmd_type right_pipe_type = parse_abstract_cmd(right_pipe_buffer);


  pid_t pid = fork();

  if (pid < 0) exit(0);

  if (pid == 0){// Child, execute left hand side and write output to pipe.

    close(pipefds[0]);
    dup2(pipefds[1], STDOUT_FILENO);
    close(pipefds[1]);
    if ( execvp(left_pipe_program, (pipe_cmd -> left)->argv) == -1 ){
      printf("Failed to execute...\n");
      exit(EXIT_FAILURE);
    }

  }else{
    close(pipefds[1]);
    dup2(pipefds[0], STDIN_FILENO);
    close(pipefds[0]);
    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status)) {
      printf("Child process exited with status %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
      printf("Child process terminated by signal %d\n", WTERMSIG(status));
    }

    // Base case of recursion
    if (right_pipe_type == EXEC){
      struct execcmd *right_cmd = parse_exec_cmd(right_pipe_buffer);
      execute_cmd(right_cmd);
    }
    // Recursive case
    else{
      struct pipecmd *right_pipe = parse_pipe_cmd(right_pipe_buffer);
      execute_pipeline(right_pipe);
    }

  }
  return 1;
}





