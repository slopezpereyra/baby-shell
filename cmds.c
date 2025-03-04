#include <stdio.h> 
#include <stdlib.h>
#include "cmds.h"
#include <unistd.h>
#include <sys/wait.h>

struct execcmd *init_exec_cmd(){
  struct execcmd * cmd = malloc(sizeof(struct execcmd));
  // Ensure last byte is null terminating string.
  ( cmd->argv )[MAX_ARGS] = NULL;
  (cmd -> n_args) = 0;
  (cmd -> type) = EXEC;
  return cmd;
}


struct pipecmd *init_pipe_cmd(){
  struct pipecmd * cmd = malloc(sizeof(struct pipecmd));
  // Ensure last byte is null terminating string.
  ( cmd->type ) = PIPE;
  (cmd -> left) = init_exec_cmd();
  (cmd -> right) = init_exec_cmd();
  return cmd;

}

//void execute_command(struct cmd *cmd){
//  
//  switch (cmd -> type){
//    // Go case by case doing the corresponding logic,
//    // casting cmd into the corresponding type.
//  }
//
//}
//
//// La idea es: ejecutra el lado izquierdo con STDIN y y duplicar su standard
//// output al write end del pipe. Luego ejecutar el lado derecho pasando como
//// input el
//
//void execute_pipecmd(struct cmd *cmd){
//
//  // After pipe(pipefds) is called, pipefds holds two file descriptors:
//  // pipe[0] for the read end of the pipe, pipe[1] for the write end of the
//  // pipe.
//  int pipefds[2];
//  
//  struct pipecmd *pcmd = (struct pipecmd*)cmd;
//
//  // Complete implementation here
//
//  int pipe_code = pipe(pipefds);
//
//  if ( pipe_code < 0 ) {
//    printf("Pipe has failed\n");
//    exit(0);
//  }
//
//  pid_t pid = fork();
//
//  if (pid < 0) exit(0);
//
//  if (pid != 0){
//    // Close read end for the child, leave only write end open.
//    close(pipefds[0]); 
//    // Let standard output and write end of the pipe refer to the  same file
//    // descriptor.
//    dup2(pipefds[1], STDOUT_FILENO);
//    // Execute command, result will be written on the write end of the file
//    // descriptor.
//    execute_cmd(pcmd->left);
//    close(pipefds[1]);
//    wait(pid);
//
//  }
//
//
//}
//
//// ( ls -1 ) | echo | echo
//
