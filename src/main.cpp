#include <iostream>
#include <string>
#include <buffer_cache.hpp>

int main(void)
{
    init();
    // Register commands
    populate_command_maps();  

    std::cout << "Welcome! Type 'help' to see available commands.\n";

    // Command loop
    std::string input;
    while (TRUE) {
        std::cout << "--> ";
        std::getline(std::cin, input); // Read input

        std::cout << input << std::endl;

        if (input == "exit") break; // Exit condition

        parse_command(input); // Parse and execute the command
    }

    release_alloced();
    return 0;

}