#ifndef _BUFFER_CACHE_HPP_
#define _BUFFER_CACHE_HPP_

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <cstring>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

constexpr unsigned int WAITING_QUEUE_SIZ =  10;
constexpr int NO_OF_HASH_QUEUE = 4;
constexpr unsigned int TRUE =  1;
constexpr unsigned int MAX_LINE_SIZE =  1024;

typedef enum WAITING_TYPE {
    WAITING_FOR_THIS_BUFFER = -1,    // Negative : Waiting for THIS buffer
    WAITING_FOR_ANY_BUFFER = 1      // Waiting for any buffer

} WAITING_TYPE;


namespace state {
    constexpr unsigned int BUFFER_BUSY = 1;              // Buffer is in use
    constexpr unsigned int BUFFER_DATA_VALID = 2;        // Data in the buffer is valid
    constexpr unsigned int BUFFER_DELAYED_WRITE = 4;     // Is buffer marked as delayed write
    constexpr unsigned int KERNEL_IO = 8;                // Is kernel reading/writing this buffer
    constexpr unsigned int BUFFER_AWAITED = 16;          // Is other process waiting for this buffer
    constexpr unsigned int BUFFER_DATA_OLD = 32;         // Buffer's data is old

    // State of the buffer manipulation functions
    // void add_state(buffer_header *buffer, STATE state);
    // void remove_state(buffer_header* buffer, STATE state);
    // void set_state(buffer_header* buffer, STATE state);
    // int is_in_state(buffer_header* bufer, STATE state);

}

class buffer_header {
    private:
        int device_no;
        int blk_no;
        unsigned int status;
        char *cache_data;   // data area 

        struct buffer_header* hash_next;
        struct buffer_header* hash_prev;
        struct buffer_header* free_next;
        struct buffer_header* free_prev;

    public:
        // Default CC

         buffer_header()
            : device_no(0), blk_no(0), status(0), cache_data(nullptr),
              hash_next(nullptr), hash_prev(nullptr),
              free_next(nullptr), free_prev(nullptr) {}
        
        void show_buffer(int inde, buffer_header *p);
        void show_all_buffers();
        buffer_header* getblk(int blk_num);
        void brealse(buffer_header* buffer);


};

class free_list : public buffer_header {
    private:
        buffer_header* free_list_head;
    public: 
        // Free list functions 
        free_list() : free_list_head(nullptr) {}

        void show_free_list();
        int is_free_list_empty();
        void free_list_push_front(buffer_header* ref, buffer_header* buf);
        void free_list_push_back(buffer_header* ref, buffer_header* buf);
        buffer_header* get_buf_from_head_of_free_list();
        void remove_buf_from_free_list(buffer_header*);
        int search_buf_in_free_list(buffer_header* buffer);

};

class hash_q {
    private:
        // Pointer to an array of hash queue pointers
        buffer_header** hash_queue;
    public:
        hash_q(int size) 
            : hash_queue(new buffer_header*[size]) {
            for (int i = 0; i < size; ++i) {
                hash_queue[i] = nullptr;
            }
        }
     ~hash_q() { delete[] hash_queue; }
        void show_all_hashq();
        // Hash queue handlers
        int is_hashq_empty(int hashq_number);
        void hashq_push_front(buffer_header* ref, buffer_header* buf);
        void add_buf_to_hashq(buffer_header* buffer);
        void remove_buf_from_hashq(buffer_header* buf);
        buffer_header* search_blk_in_hashq(int blk_no);

};

class waiting_queue {
    private:
        std::vector<int> waiting_queue;

    public:
        void add_to_waitq(int blk_num, WAITING_TYPE WAITING_FOR_THIS_BUFFER);
        void find_in_waitq(int blk_no);
        int get_proc_from_waitq(int blk_no);
        int is_waitq_emoty();
        void show_waitq();

};

// Function to register commands
void populate_command_maps();
void parse_command(const std::string& input);


#endif 