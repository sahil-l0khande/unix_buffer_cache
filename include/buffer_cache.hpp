#ifndef _BUFFER_CACHE_H_
#define _BUFFER_CACHE_H_

// Header files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <iostream>

// Symbolic constants 
// Writing queue : for sleeping processes which are waiting for perticular buffer becomes free
#define WAITING_QUEUE_SIZ 10

// DCLL for Hash queue and free list
#define NO_OF_HASH_QUEUE 4

#define TRUE 1
#define MAX_LINE_SIZE 1024


/* Types of waiting */
typedef enum WAITING_TYPE {
    WAITING_FOR_THIS_BUFFER = -1,    // Negative : Waiting for THIS buffer
    WAITING_FOR_ANY_BUFFER = 1      // Waiting for any buffer

} WAITING_TYPE;
// Buffer header structure

typedef struct buffer_header {
    int device_no;
    int blk_no;
    unsigned int status;
    char *cache_data;   // data area 

    struct buffer_header* hash_next;
    struct buffer_header* hash_prev;
    struct buffer_header* free_next;
    struct buffer_header* free_prev;
} buffer_t;

// int* wating_queue;

// // Pointer to an array of hash queue pointers
// buffer_t** hash_queue;

// // Head of free list
// buffer_t* free_list_head; // This will be dummy head node


// Input command mapping to handler functions

typedef struct cmd_func_map {
    char* cmd;
    void (*func) (int, char*[]);
} cmd_func_t;



// Possible states of buffer 

typedef enum STATE {

    // Buffer is in use
    BUFFER_BUSY = 1,

    // Data in the buffer is valid
    BUFFER_DATA_VALID = 2,

    // Is buffer is marked as delayed write
    BUFFER_DELAYED_WRITE = 4,

    // Is kernel reading/writing this buffer to disk (I/O)
    KERNEL_IO = 8,

    // Is other proccess waiting for this buffer
    BUFFER_AWAITED = 16,

    // Buffer's data is old
    BUFFER_DATA_OLD = 32
} STATE;


// Function declarations

// State of the buffer manipulation functions
void add_state(buffer_t *buffer, STATE state);
void remove_state(buffer_t* buffer, STATE state);
void set_state(buffer_t* buffer, STATE state);
int is_in_state(buffer_t* bufer, STATE state);

// Waiting queue functions

void add_to_waitq(int blk_num, WAITING_TYPE WAITING_FOR_THIS_BUFFER);
void find_in_waitq(int blk_no);
int get_proc_from_waitq(int blk_no);
int is_waitq_emoty();


// Hash queue handlers
int is_hashq_empty(int hashq_number);
void hashq_push_front(buffer_t* ref, buffer_t* buf);
void add_buf_to_hashq(buffer_t* buffer);
void remove_buf_from_hashq(buffer_t* buf);
buffer_t* search_blk_in_hashq(int blk_no);


// Free list functions 

int is_free_list_empty();

void free_list_push_front(buffer_t* ref, buffer_t* buf);
void free_list_push_back(buffer_t* ref, buffer_t* buf);
buffer_t* get_buf_from_head_of_free_list();
void remove_buf_from_free_list(buffer_t*);
int search_buf_in_free_list(buffer_t* buffer);


// Show functions

void show_buffer(int inde, buffer_t *p);
void show_all_buffers();
void show_all_hashq();
void show_free_list();
void show_waitq();

// Buffer cache management algorithms

buffer_t* getblk(int blk_num);
void brealse(buffer_t* buffer);

void populate_command_maps();
void parse_command(const std::string& input);

// Initializer funtion
void init(void);
void alloc_mem_for_data_structures();
void setup_data_structures();    // Set up data structures as per figure in bach
void release_alloced(void);
#endif /* _BUFFER_CACHE_H_ */