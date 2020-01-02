#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>



int main(){

int pipe1FDs[2];
int pipe2FDs[2];
int pipe3FDs[2];

pipe(pipe1FDs);
pipe(pipe2FDs);
pipe(pipe3FDs);

    pid_t c1pid = fork();
    if(c1pid==0){
        pid_t c2pid = fork();
        if(c2pid==0){
            pid_t c3pid = fork();
            if(c3pid==0){
                //third forked child process : outputs strings of 80 chars
                //reads from pipe 3, writes to sdout
                close(pipe1FDs[0]); 
                close(pipe2FDs[0]); 
                 
                close(pipe1FDs[1]);
                close(pipe2FDs[1]);
                close(pipe3FDs[1]);
                char outputString[81];
                int i = 0;
                int outputChar;
                while(read(pipe3FDs[0], &outputChar, 1) >0){
                    outputString[i] = outputChar;
                    i++;
                    if (i == 80){
                        int a = 0;
                        outputString[80] = '\n';
                        while(a<81){
                        putchar(outputString[a]);
                        a++;
                        }
                        i = 0;
                        a = 0;
                    }
                }
                close(pipe3FDs[0]);
                exit(0);
            }

            //second forked child process : replaces every pair of asterisks with a ^
            //reads from pipe 2, writes to pipe 3
            close(pipe1FDs[0]); 
             
            close(pipe3FDs[0]); 
            close(pipe1FDs[1]);
            close(pipe2FDs[1]);
            
            int buffer2;
            int buffer2Two;
            int carrot = '^';
            while (read(pipe2FDs[0], &buffer2, 1) > 0){
                if(buffer2 == '*'){
                    if(read(pipe2FDs[0], &buffer2Two, 1) > 0){
                        if(buffer2Two == '*'){
                            write(pipe3FDs[1], &carrot, 1);
                        } else {
                            write(pipe3FDs[1], &buffer2, 1);
                            write(pipe3FDs[1], &buffer2Two, 1);
                        }
                    } else {
                        write(pipe3FDs[1], &buffer2, 1);
                    }
                } else {
                    write(pipe3FDs[1], &buffer2, 1);
                }
            }
            close(pipe2FDs[0]);
            close(pipe3FDs[1]);
            wait(NULL);
            exit(0);
            //end of c2 process
        }

        //first forked child process: replaces every carriage return with a space
        //reads from pipe 1, writes to pipe 2
        char buffer1;
        
        close(pipe2FDs[0]); 
        close(pipe3FDs[0]); 
        close(pipe1FDs[1]);
        
        close(pipe3FDs[1]);
        while (read(pipe1FDs[0], &buffer1, 1) >0){
            if(buffer1 == '\n'){
                buffer1 = ' ';
                //write to pipe 2
                write(pipe2FDs[1], &buffer1, 1);
            }else{
                //write to pipe 2
                write(pipe2FDs[1], &buffer1, 1);
            }
        }
        //end of c1 process
        close(pipe1FDs[0]);
        close(pipe2FDs[1]);
        wait(NULL);
        exit(0);

    }else{
        //parent process : reads from stdin, writes to pipe 1
        close(pipe1FDs[0]); 
        close(pipe2FDs[0]); 
        close(pipe3FDs[0]); 
        
        close(pipe2FDs[1]);
        close(pipe3FDs[1]);
        int _char;
        while((_char=getchar())!=EOF){
            write(pipe1FDs[1], &_char, 1);
        }
       
        close(pipe1FDs[1]);
        wait(NULL);
        return 0;
    }

}