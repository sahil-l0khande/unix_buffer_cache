#include <iostream>
#include <string>
#include "command_handle.hpp"
#include "buffer_cache.hpp"

int main(void)
{
   // init();
    // Register commands
    Command cmd;

    std::cout << "Welcome! Type 'help' to see available commands.\n";

    // Command loop
    std::string input;
    while (TRUE) {
        std::cout << "--> ";
        std::getline(std::cin, input); // Read input

        std::cout << input << std::endl;

        if (input == "exit") break; // Exit condition

        // Handle commands
        cmd.handle_command(input); // Parse and execute the command
    }

    //release_alloced();
    return 0;

}