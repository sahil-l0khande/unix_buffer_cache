#include "buffer_cache.h"

void alloc_mem_for_ds();  //allocate memory for data structures
void setup_ds();       // Set up data structures as per figure

/*
Initializing necessary structures
*/

void init() {
    printf("Initializing....\n");
    alloc_mem_for_ds();
    setup_ds();
    printf("Ready to go.\n");
}

void alloc_mem_for_ds()
{
    printf("Creating Hash Queues ... ");

    hash_queue = (buffer_header **)malloc(sizeof(buffer_header *) * NO_OF_HASH_QUEUES);
    for (int i = 0; i < NO_OF_HASH_QUEUES; i++) {

        // dummy head

        hash_queue[i] = (buffer_header *)malloc(sizeof(buffer_header));

        //empty out the hash_queue, set to defaults
        hash_queue[i]->hash_next = hash_queue[i];
        hash_queue[i]->hash_prev = hash_queue[i];
        hash_queue[i]->status = 0;
        hash_queue[i]->free_next = free_list_dummy_head;
        hash_queue[i]->free_prev = free_list_dummy_head;
        hash_queue[i]->cache_data = NULL;
    }

    printf("Done\n");

    printf("Creating free list ... ");
    free_list_dummy_head = (buffer_header*)malloc(sizeof(buffer_header));
    free_list_dummy_head->free_next = free_list_dummy_head;
    free_list_dummy_head->free_prev = free_list_dummy_head;
    printf("Done\n");

    printf("Creating wait queue ...");
    waiting_queue = (int*)malloc(sizeof(int)*SIZE_OF_WAITING_QUEUE);
    printf("Done\n");
}

/*
Initialise buffer cache to fig 3.3
*/
void setup_ds()
{
    buffer_header *trav;
    printf("Setting up buffer cache as per figure 3.2 of MAURICE J. BACH\n");
    printf("Settig up hash queues...");

    for (int i = 0; i < NO_OF_HASH_QUEUES; i++) {
        // put 3 buffers in each hash queue
        buffer_header *p = malloc(sizeof(buffer_header));
        hash_queue_push_front(hash_queue[i], p);

        buffer_header *q = malloc(sizeof(buffer_header));
        hash_queue_push_front(hash_queue[i]->hash_next, q);

        buffer_header *r = malloc(sizeof(buffer_header));
        hash_queue_push_front(hash_queue[i]->hash_next->hash_next, r);
    }

    //setting up hash queue 0
    trav = hash_queue[0]->hash_next;
    trav->block_no = 28;
    set_state(trav, BUFFER_DATA_VALID);

    trav = trav->hash_next;
    trav->block_no = 4;
    set_state(trav, BUFFER_DATA_VALID);

    trav = trav->hash_next;
    trav->block_no = 64;
    set_state(trav, BUFFER_DATA_VALID | BUFFER_BUSY);

    // Setting up hash queue 1
    trav = hash_queue[1]->hash_next;
    trav->block_no = 17;
    set_state(trav, BUFFER_DATA_VALID | BUFFER_BUSY);

    trav = trav->hash_next;
    trav->block_no = 5;
    set_state(trav, BUFFER_DATA_VALID);

    trav = trav->hash_next;
    trav->block_no = 97;
    set_state(trav, BUFFER_DATA_VALID);

    // Setting up hash queue 2
    trav = hash_queue[2]->hash_next;
    trav->block_no = 98;
    set_state(trav, BUFFER_DATA_VALID | BUFFER_BUSY);
    trav = trav->hash_next;

    trav->block_no = 50;
    set_state(trav, BUFFER_DATA_VALID | BUFFER_BUSY);
    trav = trav->hash_next;

    trav->block_no = 10;
    set_state(trav, BUFFER_DATA_VALID);

    
}