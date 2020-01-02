#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "Socket.h"

#define MAX_TMP 100
#define MAX_LINE 1000

ServerSocket welcome_socket;
Socket connect_socket;

int main(int argc, char **argv){
    welcome_socket = ServerSocket_new(atoi(argv[1]));
    connect_socket = ServerSocket_accept(welcome_socket);

    int id;
   

    unsigned char id_str[MAX_TMP];
    unsigned char tmp_name[MAX_TMP];

    id = (int)getpid();
    sprintf(id_str, "%d", id);
    strcpy(tmp_name, "tmp");
    strcat(tmp_name, id_str);

    FILE *tmpFP;
    FILE *fp;

    while(1){
        bool received_EOF = false;
        char chars[500000];
        int index = 0;
        char *args[50000];
    
        char c;
        char eof = EOF;
        c = Socket_getc(connect_socket);
        while(c != '\n'){
            if(c == EOF){
                received_EOF = true;
                break;
            }else{
                //Socket_putc(c, connect_socket);
                //putchar(c);
                chars[index] = c;
                index++;
                }
            c = Socket_getc(connect_socket);   
        } 
        if(received_EOF){
                break;
        }

        chars[index] = c;
        chars[index+1] = '\0';
 

        //resetting index 
        index = 0;


        //sending EOF
        //Socket_putc(eof, connect_socket);
        pid_t cpid;
        if(( cpid= fork()) == 0){
            /*int b = 0;
            while (args[b] != NULL){
                free(args[b]);
                b++;
            }
            b= 0;*/
            
            //parsing input line 
            char* token; 
            int i =0;
            token =strtok(chars, " \n\t");
            args[i] = token;
            i++;
            while(token != NULL){
                token = strtok(NULL, " \n\t");
                args[i] = (char*)malloc(sizeof(token));
                args[i]= token;
                i++;
            }
            fp = freopen(tmp_name, "w", stdout);
            execvp(args[0], args);
            printf("Command not found\n");
            exit(-1);

        }else{
            wait(&cpid);
    
            tmpFP = fopen(tmp_name, "r");
            int k;
            k = fgetc(tmpFP);
            while(!feof(tmpFP)){
                //printf("%c", k);
                Socket_putc(k, connect_socket);
                k = fgetc(tmpFP);
            }
            //sending EOF
            Socket_putc(k, connect_socket);

            close(tmpFP);
            /*if(received_EOF){
                break;
            }*/

        }
    }

    remove(tmp_name);
    
    Socket_close(welcome_socket);
    Socket_close(connect_socket);
    exit(0);
}