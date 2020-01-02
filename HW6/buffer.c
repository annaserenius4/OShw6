//I certify that no unauthorized assistance has been received or given in the completion of this work. Signature: Anna Serenius;

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

#define MAX_TMP 100
#define ERROR -1
#define FILENAME "tmpfileformapping"
//#define FILESIZE = getpagesize()

void deposit(struct buffer *buffer, char buffer_char)
{
    sem_wait(&(buffer->emptyBuffers));
    
    buffer->shared_buffer[buffer->next_in] = buffer_char;
    if(buffer->next_in + 1 < 80){
        buffer->next_in++;
    }else{
        buffer->next_in = 0;
    }
    
    sem_post(&(buffer->fullBuffers));
}



char remoove(struct buffer *buffer)
{
    sem_wait(&(buffer->fullBuffers));
    char produced_char;
    produced_char = buffer->shared_buffer[buffer->next_out];
    if ((buffer->next_out+1)>79){
            buffer->next_out = 0;
        }else{
            buffer->next_out++;
        }
    sem_post(&(buffer->emptyBuffers));
    return produced_char;
}

void initialize(struct buffer *buffer)
{
    buffer->next_in = 0;
    buffer->next_out = 0;
    buffer->emptyBuffers; 
    buffer->fullBuffers; 
    
}

void destroy(struct buffer *buffer)
{
    sem_destroy(&(buffer->fullBuffers));
    sem_destroy(&(buffer->emptyBuffers));
    munmap(buffer, getpagesize());
}