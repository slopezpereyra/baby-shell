#include <stdio.h> 
#include <stdlib.h>
#include"parser.h"
#include"cmds.h"
#include<string.h>


void shell(){
  char* cmd;
  struct execcmd * tokens;
  printf("MyBash : ");
  while (1){
    cmd = read_stdin();

    if ( cmd  == NULL ) {
      printf("Error reading cmd");
      break;
    }

    tokens = parse_exec_cmd(cmd);
    
    if (( tokens -> argv )[0] != NULL){

      char* program = (tokens -> argv)[0];

      if ( strcmp(program, "exit") == 0 ) break;

      printf("Executing %s\n", program);
      
      execute_cmd(tokens);

//      free(cmd);
//      free(tokens);
      printf("MyBash : ");

    }
  }
  free(cmd);
  free(tokens);
}

int main(){
  shell();
}
