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

Socket connect_socket;

int main(int argc, char **argv){

    Socket connect_socket;
    connect_socket = Socket_new(argv[1], atoi(argv[2]));

    while(1){
        printf("%s", "% "); //printing command line prompt
        char c;
        bool EOF_inputted = false;
        int chars_sent = 0;
        int counter = 0;
        int bufferClearChar;

        c = getchar();
        while(c != '\n'){
            if(c == EOF){
                EOF_inputted = true;
                break;
            }else{
                Socket_putc(c, connect_socket);
                chars_sent++;
                c = getchar();
            }   
        }
        Socket_putc(c, connect_socket);
        chars_sent++;

   

        if(EOF_inputted){
            break;
        }


        char received_chars[50000];
        char a;
        int i = 0;
        a = Socket_getc(connect_socket);
        while(a != EOF){
            received_chars[i] = a;
            i++;
            a = Socket_getc(connect_socket);
        }

        //printing array
        for(int loop = 0; loop < i; loop++){
            putchar(received_chars[loop]);
        }
        
    }
    
    Socket_close(connect_socket);
    exit(0);
    
}