#include <stdio.h> 
#include <stdlib.h>
#include"parser.h"
#include"cmds.h"
#include<string.h>

void shell(){
  char* cmd;
  struct execcmd * execution_cmd;
  printf("MyBash : ");
  while (1){
    cmd = read_stdin();

    if ( cmd  == NULL ) {
      printf("Error reading cmd");
      break;
    }
    parse_stdin(cmd);
  }
  free(cmd);
  free(execution_cmd);
}

int main(){
  shell();
}
