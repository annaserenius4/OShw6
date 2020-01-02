/*I certify that no unauthorized assistance has been given or received in the completion of this assignment.
Anna Serenius
Collaborators: Chantal Shine, Ashley Alexander-Lee*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv){
pid_t child;
int status;
char chars[500000];
char *args[500000];

printf("%s", "% "); //printing command line prompt

while(fgets(chars, sizeof(chars), stdin)!= NULL){
  
    child = fork();
    if (child == 0){ //this is child process
    char* token; 
    int i =0;
    token =strtok(chars, " \n\t");
    args[i] = (char*)malloc(sizeof(token));
    args[i] = token;
    i++;
    while(token != NULL){
        token = strtok(NULL, " \n\t");
        args[i] = (char*)malloc(sizeof(token));
        args[i]= token;
        i++;
    }

    execvp(args[0], args);

    fprintf(stderr, "execvp failed.\n");
    
    return 0;

    } else if (child > 0){ //parent process
    wait(&child);
    int b =0;
    do{
    free(args[b]);
    b++;
    } while (args[b] != NULL);


    } else {
        fprintf(stderr, "Fork failed.\n"); 
    }
  printf("%s", "% "); //printing command line prompt
         
}
return 0;

}