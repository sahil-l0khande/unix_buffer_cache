#include "buffer_cache.h"

#define MAX_LINE_SIZE 1024

void  init();
int parse_usr_input(char *cmd, char **argv);

int main() 
{
    char user_string[MAX_LINE_SIZE];

    //initialise data structures
    init();

    while(1) {
        //Prompt for user
        printf("➜ ");
        if (fgets(user_string, MAX_LINE_SIZE, stdin) == NULL) {
            exit(EXIT_FAILURE);
        }

        // Parse user input properly
        char *argv[MAX_LINE_SIZE];
        
    }
}