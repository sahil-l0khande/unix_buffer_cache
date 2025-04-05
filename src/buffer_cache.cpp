#include <buffer_cache.hpp>
#include <iostream>

/* */
void init() {

    std::cout << "Initializing..." << std::endl;
    alloc_mem_for_data_structures();
    setup_data_structures();
    std::cout << "Ready to go" << std::endl;
}

void alloc_mem_for_data_structures(void) 
{
    std::cout << "Creating hash queues ... ";

    hash_queue = new buffer_t*[NO_OF_HASH_QUEUE];

    for (int i = 0; i < NO_OF_HASH_QUEUE; i++) {
        // dummy head
        hash_queue[i] = new buffer_t;

        // Empty out the hash_queue , set to defaults

        hash_queue[i]->hash_next = hash_queue[i];
        hash_queue[i]->hash_prev = hash_queue[i];
        hash_queue[i]->status = 0;
        hash_queue[i]->free_next = free_list_head;
        hash_queue[i]->free_prev = free_list_head;
        hash_queue[i]->cache_data = NULL;
    }

    std::cout << "Done " << std::endl;

    std::cout << "Creating free list ... ";

    free_list_head = new buffer_t;
    free_list_head->free_next = free_list_head;
    free_list_head->free_prev = free_list_head;
    std::cout << "Done " << std::endl;

    std::cout << "Creating wait queue " << std::endl;

    wating_queue = new int[WAITING_QUEUE_SIZ];
    std::cout << "Done " << std::endl;
}

void setup_data_structures()
{
    buffer_t* trav;
    std::cout << "Setting up buffer cache as per figure 3.2 of Mourice J Bach" << std::endl;

    std::cout << "Setting up hash queues ... ";

    for (int i = 0; i < NO_OF_HASH_QUEUE; i++) {
        // Put 3 buffers in each has queue
         
         buffer_t *p = new buffer_t;
         hashq_push_front(hash_queue[i], p);

         buffer_t *q = new buffer_t;
         hashq_push_front(hash_queue[i]->hash_next, q);

         buffer_t *r = new buffer_t;
         hashq_push_front(hash_queue[i]->hash_next->hash_next, r);
    }

    // Setting up hash queue 0
    trav = hash_queue[0]->hash_next;
    trav->blk_no = 28;
    set_state(trav, BUFFER_DATA_VALID);

    trav = trav->hash_next;
    trav->blk_no = 4;
    set_state(trav, BUFFER_DATA_VALID);

    trav = trav->hash_next;
    trav->blk_no = 64;
    set_state(trav, static_cast<STATE>(BUFFER_DATA_VALID | BUFFER_BUSY));

    // Setting up hash queue 1

    trav = hash_queue[1]->hash_next;
    trav->blk_no = 17;
    set_state(trav, static_cast<STATE>(BUFFER_DATA_VALID | BUFFER_BUSY));

    trav = trav->hash_next;
    trav->blk_no = 5;
    set_state(trav, BUFFER_DATA_VALID);

    trav = trav->hash_next;
    trav->blk_no = 97;
    set_state(trav, BUFFER_DATA_VALID);

    // Setting up hash queue 2

    trav = hash_queue[2]->hash_next;
    trav->blk_no = 98;
    set_state(trav, static_cast<STATE>(BUFFER_DATA_VALID | BUFFER_BUSY));
    trav = trav->hash_next;
    
    trav->blk_no = 50;
    set_state(trav, static_cast<STATE>(BUFFER_DATA_VALID | BUFFER_BUSY));
    trav = trav->hash_next;

    trav->blk_no = 10;
    set_state(trav, BUFFER_DATA_VALID);

    // Setting up hash Queue 3

    trav = hash_queue[3]->hash_next;
    trav->blk_no = 3;
    set_state(trav, BUFFER_DATA_VALID);

    trav = trav->hash_next;
    trav->blk_no = 35;
    set_state(trav, static_cast<STATE>(BUFFER_DATA_VALID | BUFFER_BUSY));

    trav = trav->hash_next;
    trav->blk_no = 99;
    set_state(trav, static_cast<STATE>(BUFFER_DATA_VALID | BUFFER_BUSY));
    
    std::cout << "Done " << std::endl;

    std::cout << "Setting up Free list ... " << std::endl;

    free_list_push_back(free_list_head, hash_queue[3]->hash_next);
    free_list_push_back(free_list_head, hash_queue[1]->hash_next->hash_next);
    free_list_push_back(free_list_head, hash_queue[0]->hash_next->hash_next);
    free_list_push_back(free_list_head, hash_queue[0]->hash_next);
    free_list_push_back(free_list_head, hash_queue[1]->hash_next->hash_next->hash_next);
    free_list_push_back(free_list_head, hash_queue[2]->hash_next->hash_next->hash_next);

    std::cout << "Done " << std::endl;
}

buffer_header* getblk(int blk_num){

  int hash_key = blk_num % NO_OF_HASH_QUEUE;

  // @todo : can we make this infinite loop
  while (hash_queue[hash_key]->hash_next != NULL){

    // search Block in Hash Queue
    buffer_t *buffer = search_blk_in_hashq(blk_num);
    
    if (buffer != NULL){    // if buffer found in hash Queue
    
      if (is_in_state(buffer, BUFFER_BUSY)){    // buffer is busy : SCENARIO 5
        std::cout << " ********* Scenario 5 *********" << std::endl;
        std::cout << "The kernel finds the block on the hash queue, but its buffer is currently busy." << std::endl;
        std::cout << "Process goes to sleep." << std::endl;
        std::cout << "Process will wake up when this buffer becomes free" << std::endl;
        add_state(buffer, BUFFER_AWAITED);
        add_to_waitq(blk_num, WAITING_FOR_THIS_BUFFER);
        // sleep(); 
        // continue;
        return NULL;  // for now
      }
      else{                                   // buffer is free : SCENARIO 1
        std::cout << "********* Scenario 1 *********" << std::endl;
        std::cout << "The kernel finds the block on its hash queue, and its buffer is free." << std::endl;

        set_state(buffer, static_cast<STATE>((BUFFER_BUSY | BUFFER_DATA_VALID)));
        std::cout << "Buffer Marked Busy." << std::endl;

        remove_buf_from_free_list(buffer);
        std::cout << "Removed Buffer from Free List." << std::endl;
        return buffer;
      }
    }
    else{                  // Block is not on hash Queue
    
      if (is_free_list_empty()){   // if there are no buffers in free list : SCENARIO 4
        
        std::cout << "********* Scenario 4 *********" << std::endl;
        std::cout << "The kernel cannot find the block on the hash queue, and the free list of buffers is empty." << std::endl;
        std::cout << "Process goes to sleep." << std::endl;
        std::cout << "Process will wake up when any buffer becomes free." << std::endl;

        add_to_waitq(blk_num, WAITING_FOR_ANY_BUFFER);
        // sleep();
        // continue;
        return NULL;  // for now
      }

      buffer_t *ref = free_list_head->free_next;

      if (is_in_state(ref, BUFFER_DELAYED_WRITE)){   // Buffer marked delayed write :  SCENARIO 3
        std::cout << "********* Scenario 3 *********" << std::endl;
        std::cout << "The kernel cannot find the block on the hash queue. Found free block is marked delayed write." << std::endl;
        buffer_t *prev = ref->free_prev;
        buffer_t *next = ref->free_next;
        prev->free_next = next;
        next->free_prev = prev;
        set_state(ref, static_cast<STATE>(BUFFER_BUSY | BUFFER_DATA_VALID | BUFFER_DELAYED_WRITE | BUFFER_DATA_OLD));
        std::cout << "Marking Buffer OLD. Starting Asynchronous write to disk." << std::endl;
        continue;
      }

      // SCENARIO 2  :Found a free Buffer
        std::cout << "********* Scenario 2 *********" << std::endl;
        std::cout << "The kernel cannot find the block on the hash queue, so it allocates a buffer from the free list." << std::endl;

      buffer_t *freeBuffer = get_buf_from_head_of_free_list();
      remove_state(freeBuffer, BUFFER_DATA_VALID);
      std::cout << "REMOVING FROM OLD HASH QUEUE.PUTTING IN NEW HASH QUEUE" << std::endl;
      freeBuffer->blk_no = blk_num;
      add_buf_to_hashq(freeBuffer);
      std::cout << "Kernel DISK access occuring. Status of Block Updated." << std::endl;
      add_state(freeBuffer, KERNEL_IO);
      std::cout << "Kernel DISK access finished. Status of Block Updated." << std::endl;
      remove_state(freeBuffer, KERNEL_IO);
    std::cout << "BUFFER now contains new and Valid Data. Updating status." << std::endl;
      add_state(freeBuffer, BUFFER_DATA_VALID);
      return freeBuffer;
    }
  }
  std::cout << "BUFFER NOT FOUND" << std::endl;
  return NULL;
}

void release_alloced(void)
{
    for (int i = 0; i < NO_OF_HASH_QUEUE; i++) {
        buffer_t* current = hash_queue[i];
        buffer_t* next;
        do {
            next = current->hash_next;
            delete current;
            current = next;
        } while (current != hash_queue[i]); // Loop through the circular linked list
    }
    delete[] hash_queue;

    // Deallocate free list
    buffer_t* current = free_list_head;
    buffer_t* next;
    do {
        next = current->free_next;
        delete current;
        current = next;
    } while (current != free_list_head); 

    // Deallocate waiting queue
    delete[] wating_queue; 
}