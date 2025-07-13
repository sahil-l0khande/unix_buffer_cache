#ifndef _COMMAND_HANDLE_H
#define _COMMAND_HANDLE_H

#include <iostream>
#include <unordered_map>
#include <functional>
#include <sstream>
#include <string>

// Input command mapping to handler functions

class Command {
    // command maps
    std::unordered_map<std::string, std::function<void()>> cmd_map_without_arg;
    std::unordered_map<std::string, std::function<void(std::string)>> cmd_map_with_arg;

    public:
        Command();
        void handle_command(const std::string& input);
        void parse_command(const std::string& input);


        // Command functions
        void buf_handle();
        void set_handle(std::string argv);
        void getblk_handle(std::string argv);
        void brelease_handle(std::string argv);
        void hash_handle();
        void free_handle();
        void wait_handle();
        void reset_handle();
        void help_handle();
        void exit_handle();

};


#endif /* _COMMAND_HANDLE_H */