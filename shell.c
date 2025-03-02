#include <stdio.h> 
#include <stdlib.h>
#include"parser.h"
#include<string.h>


void shell(){
  char* cmd;
  char** tokens;
  printf("MyBash : ");
  while (1){
    cmd = read_cmd();

    if ( cmd  == NULL ) {
      printf("Error reading cmd");
      break;
    }

    tokens = parse_cmd(cmd);
    
    if (tokens[0] != NULL){

      if ( strcmp(tokens[0], "exit") == 0 ) break;

      printf("Executing %s\n", tokens[0]);
      
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
