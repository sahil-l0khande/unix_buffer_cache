#include "command_handle.hpp"


// Command functions
void Command::buf_handle() { std::cout << "Executing buf command\n"; }
void Command::hash_handle() { std::cout << "Executing show_hash command\n"; }
void Command::free_handle() { std::cout << "Executing show_free command\n"; }
void Command::wait_handle() { std::cout << "Executing wait command\n"; }
void Command::reset_handle() { std::cout << "Executing reset command\n"; }
void Command::help_handle()
{ 

    for (auto it : cmd_map_without_arg) {
        std::cout << it.first << std::endl; 
    }

    for (auto it : cmd_map_with_arg) {
        std::cout << it.first << std::endl; 
    }

}

void Command::exit_handle() { std::cout << "Executing exit command\n"; }

void Command::set_handle(std::string argv) { std::cout << "Executing set command with argument: " << argv << "\n"; }
void Command::getblk_handle(std::string argv) {
     std::cout << "Executing getblk command with argument: " << argv << "\n"; 

    // try {
    //     // Convert the string argument to an integer using stoi
    //     int blk_num = std::stoi(argv);

    //     // Create a buffer_header instance
    //     buffer_header* allocated_buf = getblk(blk_num);

    //     if (allocated_buf != NULL) {
    //         std::cout << "Buffer Allocated Successfully.\n";
    //     } else {
    //         std::cout << "Failed to allocate buffer.\n";
    //     }

    // } catch (const std::invalid_argument& e) {
    //     std::cerr << "Error: Invalid argument for block number. Please provide a valid integer.\n";
    // } catch (const std::out_of_range& e) {
    //     std::cerr << "Error: Block number is out of range. Please provide a smaller integer.\n";
    // }
}

void Command::brelease_handle(std::string argv) { std::cout << "Executing brealease command with argument: " << argv << "\n"; }

// Populate the command map with the functions using labda for this
Command::Command() : 
    cmd_map_without_arg({
        {"buf", [this]() { buf_handle(); }},
        {"show_hash", [this]() { hash_handle(); }},
        {"show_free", [this]() { free_handle(); }},
        {"wait", [this]() { wait_handle(); }},
        {"reset", [this]() { reset_handle(); }},
        {"help", [this]() { help_handle(); }},
        {"exit", [this]() { exit_handle(); }}
    }),
    cmd_map_with_arg({
        {"set", [this](std::string argv) { set_handle(argv); }},
        {"getblk", [this](std::string argv) { getblk_handle(argv); }},
        {"brelease", [this](std::string argv) { brelease_handle(argv); }}
    })
{

}

void Command::handle_command(const std::string& input)
{
    parse_command(input);
}

// Parse user input and execute the command
void Command::parse_command(const std::string& input) {
    std::cout << "cmd rec: " << input << std::endl;

    std::istringstream iss(input);
    std::string cmd;
    std::string arg;

    // Parse the command name
    iss >> cmd;

    // Check if the command exists in the map with arguments
    auto it_with_arg = cmd_map_with_arg.find(cmd);
    if (it_with_arg != cmd_map_with_arg.end()) {
        // Capture the rest of the string as the argument for commands that require it
        std::getline(iss, arg); // Get the rest of the line after the command name

        // Remove leading whitespace (if any) from the argument
        if (!arg.empty() && arg[0] == ' ') {
            arg = arg.substr(1);
        }

        if (!arg.empty()) {
            // Call the function with the string argument
            it_with_arg->second(arg);
        } else {
            std::cout << "Error: Missing argument for command: " << cmd << "\n";
        }
    } else {
        // Check if the command exists in the map without arguments
        auto it_without_arg = cmd_map_without_arg.find(cmd);
        if (it_without_arg != cmd_map_without_arg.end()) {
            // Call the function without arguments
            it_without_arg->second();
        } else {
            std::cout << "Error: Unknown command: " << cmd << "\n";
        }
    }
}
