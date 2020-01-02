//I certify that no unauthorized assistance has been received or given in the completion of this work. Signature: Anna Serenius
#include "buffer.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>

buffer buffer1;
buffer buffer2;
buffer buffer3;


//Thread 1: reads chars from standard input
void *thread1(void *arg) {
    bool done = false;

    char c;
    
    while(!done){        
        c = getchar();
        if(c == EOF){
            deposit(&buffer1, c);
            done = true;
            //putchar(c);
        }else{
          //  putchar(c);
            deposit(&buffer1, c);
            //putchar(c);
            
        }
        
    }
    
}

//Thread 2: replaces every carriage return with a space
void *thread2(void *arg) {
//printf("thread 2 entered");
    bool done = false;

    while(!done){
        char removed_char;
        removed_char = remoove(&buffer1);

        //printf("%s%c\n", "removed char: ", removed_char);
        if(removed_char != EOF){
            if(removed_char == '\n'){
                removed_char = ' ';
                deposit(&buffer2, removed_char);
            }else{
                deposit(&buffer2, removed_char);
            }
        }else{
            deposit(&buffer2, removed_char);
            done = true;
        }
    }


}

//Thread 3: replaces every pair of asterisks with a ^
void *thread3(void *arg) {

    bool done = false;

    while(!done){
        char removed_char;
        removed_char = remoove(&buffer2);

        if(removed_char != EOF){
            if (removed_char == '*'){
            char removed_char2;
            removed_char2 = remoove(&buffer2);
            if(removed_char2 != EOF){
                if (removed_char2 == '*'){
                    char deposit_carrot;
                    deposit_carrot = '^';
                    deposit(&buffer3, deposit_carrot);
                }else{
                    deposit(&buffer3, removed_char);
                    deposit(&buffer3, removed_char2);
                }
            } else{
                deposit(&buffer3, removed_char);
                deposit(&buffer3, removed_char2);
                done = true;
            } 
        } else {
            deposit(&buffer3, removed_char);
        }
        }else{
            deposit(&buffer3, removed_char);
            done = true;
        }
    }
}

//Thread 4: output thread
void *thread4(void *arg) {

    bool done = false;
    char outputString[81];
    int i = 0;

    while(!done){
        char outputChar;
        outputChar = remoove(&buffer3);
        if (outputChar != EOF){
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
        } else{
            done = true;
        }
    }
}

int main(){

    initialize(&buffer1);
    initialize(&buffer2);
    initialize(&buffer3);

    sem_init(buffer1.emptyBuffers, 0, 80);
    sem_init(buffer1.fullBuffers, 0, 0);
    sem_init(buffer2.emptyBuffers, 0, 80);
    sem_init(buffer2.fullBuffers, 0, 0);
    sem_init(buffer3.emptyBuffers, 0, 80);
    sem_init(buffer3.fullBuffers, 0, 0);
    
    
    pthread_t t1, t2, t3, t4;
  

    pthread_create(&t1, NULL, thread1, NULL); 
    pthread_create(&t2, NULL, thread2, NULL); 
    pthread_create(&t3, NULL, thread3, NULL); 
    pthread_create(&t4, NULL, thread4, NULL); 

    pthread_join(t1, NULL); 
    pthread_join(t2, NULL); 
    pthread_join(t3, NULL); 
    pthread_join(t4, NULL); 

    destroy(&buffer1);
    destroy(&buffer2);
    destroy(&buffer3);

    return 0;
}