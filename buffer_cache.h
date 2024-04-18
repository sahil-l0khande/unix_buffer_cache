#ifndef SRC_BUFFER_CACHE_H
#define SRC_BUFFER_CACHE_H

//standard headers
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

//Buffer header struct
typedef struct buffer_header {
    int device_no;
    int block_no;
    unsigned int status;
    char *cache_data;

    struct buffer_header *hash_next;
    struct buffer_header *hash_prev;
    struct buffer_header *free_next;
    struct buffer_header *free_prev;
} buffer_header;

// Possible states for buffer
typedef enum STATE {
    //Is buffer currently in use by a process
    BUFFER_BUSY = 1,
    
    // Is buffer data valid
    BUFFER_DATA_VALID = 2,

    // Is buffer marked delayed write
    BUFFER_MARKED_DELAYED_WRITE = 4,

    // Is kernel currently reading/writing this buffer to memory
    KERNEL_READING_WRITING = 8,

    // Is another process waiting for this buffer
    BUFFER_AWAITED = 16,

    // Is this buffer's data OLD
    BUFFER_DATA_OLD = 32
}STATE;

//STATE Manipulation

void add_state(buffer_header *buffer, STATE state);
void remove_state(buffer_header *buffer, STATE state);
void set_state(buffer_header *buffer, STATE state);
int is_in_state(buffer_header *buffer, STATE state);

// Waiting Queue : for sleeping processes waiting for a buffer  to get free
#define SIZE_OF_WAITING_QUEUE 10
int *waiting_queue;

#define NO_OF_HASH_QUEUES 4

// Pointer to an array of hash Queue pointers
buffer_header **hash_queue;

// Free list head
buffer_header *free_list_dummy_head;

// Hash queue functions