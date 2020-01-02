//I certify that no unauthorized assistance has been received or given in the completion of this work. Signature: Anna Serenius;
/*online resources used: 
https://beej.us/guide/bgipc/html/multi/mmap.html
and of course the linux man pages!
*/

#include "buffer.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define MAX_TMP 100
#define ERROR -1
#define FILENAME1 "tmp1fileformapping"
#define FILENAME2 "tmp2fileformapping"
#define FILENAME3 "tmp3fileformapping"

//pointers to the global buffers that are shared b/n processes
//buffers will be allocated a chunk of shared memory using mmap system call
buffer* buffer1;
buffer* buffer2;
buffer* buffer3;

int main(){
    //creating 3 temporary files that will be used as the shared mapped memory
   int pagesize = getpagesize();
   int fd1 = open(FILENAME1, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
   int fd2 = open(FILENAME2, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);
   int fd3 = open(FILENAME3, O_CREAT | O_RDWR, S_IREAD | S_IWRITE);

    //making sure that the files were created suceesfully
   if (lseek(fd1, pagesize-1, SEEK_SET) == ERROR){
        close(fd1);
        perror("error calling lseek");
        exit(EXIT_FAILURE);
    } else if (write(fd1, "", 1)==ERROR){
        close(fd1);
        perror("error writing to file");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd2, pagesize-1, SEEK_SET) == ERROR){
        close(fd2);
        perror("error calling lseek");
        exit(EXIT_FAILURE);
    } else if (write(fd2, "", 1)==ERROR){
        close(fd2);
        perror("error writing to file");
        exit(EXIT_FAILURE);
    }

    if (lseek(fd3, pagesize-1, SEEK_SET) == ERROR){
        close(fd3);
        perror("error calling lseek");
        exit(EXIT_FAILURE);
    } else if (write(fd3, "", 1)==ERROR){
        close(fd3);
        perror("error writing to file");
        exit(EXIT_FAILURE);
    }

    //parameters for mmap call
        //addr is where the shared memory should exist
            //NULL indicates to the OS that it should decide
        //offset is where within the shared memory we are accessing
    void* addr = NULL;
    off_t offset = 0;
    //allocating shared memory to the buffers
    buffer1 = mmap(addr, sizeof(buffer), PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);
    buffer2 = mmap(addr, sizeof(buffer), PROT_READ|PROT_WRITE, MAP_SHARED, fd2, 0);
    buffer3 = mmap(addr, sizeof(buffer), PROT_READ|PROT_WRITE, MAP_SHARED, fd3, 0);
    
    //initializing buffers sets their next in and next out integers to 0
    initialize(buffer1);
    initialize(buffer2);
    initialize(buffer3);

    //initializing the buffers' emptyBuffers and fullBuffers to 80 and 0, respectively
    //the second argument is 1 (nonzero), which indicates the semaphores
    //are to be shared b/n processes and not b/n threads
    sem_init(&buffer1->emptyBuffers, 1, 80);
    sem_init(&buffer1->fullBuffers, 1, 0);
    sem_init(&buffer2->emptyBuffers, 1, 80);
    sem_init(&buffer2->fullBuffers, 1, 0);
    sem_init(&buffer3->emptyBuffers, 1, 80);
    sem_init(&buffer3->fullBuffers, 1, 0);




//Process 1: reads chars from standard input
//child process 1: replaces every carriage return with a space
//child process 2: replaces every pair of asterisks with a ^
//child process 3: outputs chars to sdout
    pid_t c1pid = fork();
    pid_t c2pid; 
    pid_t c3pid; 
    if (c1pid == 0){
        if ((c2pid = fork()) == 0){
            if((c3pid==fork()) == 0){
                //c3 process
                bool done = false;
                char outputString[81];
                int i = 0;

                while(!done){
                    char outputChar;
                    outputChar = remoove(buffer3);
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
                exit(0);
            }else{
                //c2 process
                bool done = false;

                while(!done){
                    char removed_char;
                    removed_char = remoove(buffer2);

                    if(removed_char != EOF){
                        if (removed_char == '*'){
                        char removed_char2;
                        removed_char2 = remoove(buffer2);
                        if(removed_char2 != EOF){
                            if (removed_char2 == '*'){
                                char deposit_carrot;
                                deposit_carrot = '^';
                                deposit(buffer3, deposit_carrot);
                            }else{
                                deposit(buffer3, removed_char);
                                deposit(buffer3, removed_char2);
                            }
                        } else{
                            deposit(buffer3, removed_char);
                            deposit(buffer3, removed_char2);
                            done = true;
                        } 
                    } else {
                        deposit(buffer3, removed_char);
                    }
                    }else{
                        deposit(buffer3, removed_char);
                        done = true;
                    }
                }
                wait(NULL);
                exit(0);
            }
        } else {
            //c1 process
            bool done = false;

            while(!done){
                char removed_char;
                removed_char = remoove(buffer1);

                //printf("%s%c\n", "removed char: ", removed_char);
                if(removed_char != EOF){
                    if(removed_char == '\n'){
                        removed_char = ' ';
                        deposit(buffer2, removed_char);
                    }else{
                        deposit(buffer2, removed_char);
                    }
                }else{
                    deposit(buffer2, removed_char);
                    done = true;
                }
            }
            wait(NULL);
            exit(0);
        }
    }else{
        //process 1
        
        bool done = false;

        char c;
        
        while(!done){        
            c = getchar();
            if(c == EOF){
                deposit(buffer1, c);
                done = true;
                //putchar(c);
            }else{
            //  putchar(c);
                deposit(buffer1, c);
                //putchar(c);
                
            }
            
        }
        wait(NULL);
    }

    destroy(buffer1);
    destroy(buffer2);
    destroy(buffer3);

    remove(FILENAME1);
    remove(FILENAME2);
    remove(FILENAME3);

    return 0;
}



