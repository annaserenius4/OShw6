//I certify that no unauthorized assistance has been received or given in the completion of this work. Signature: Anna Serenius;
#include <semaphore.h> 

typedef struct buffer{
    int next_in;
    int next_out;
    char shared_buffer[80];
    sem_t *emptyBuffers, *fullBuffers;
    
   
}buffer;

void deposit(struct buffer *buffer, char c);
char remoove(struct buffer *buffer);
void initialize(struct buffer *buffer);
void destroy(struct buffer *buffer);



