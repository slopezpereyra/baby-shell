#include <stdio.h> 
#include <stdlib.h>
#include"parser.h"
#include<string.h>

void shell(){
  char* cmd;
  printf("MyBash : ");
  while (1){
    cmd = read_stdin();
    if ( cmd  == NULL ) {
      printf("Error reading cmd");
      break;
    }
    tree_parse(cmd);
  }
  free(cmd);
}

int main(){
//  shell();
//  char first_cmd[] = "ls -1\0";
//  char second_cmd[] = "wc -l\0";
//  struct execcmd *left = parse_exec_cmd(first_cmd);
//  struct execcmd *right = parse_exec_cmd(second_cmd);
//  struct pipecmd *pipe_cmd = malloc(sizeof(struct pipecmd));
//  pipe_cmd -> left = left;
//  pipe_cmd -> right = right;
//  execute_pipeline(pipe_cmd);
  shell();
}
