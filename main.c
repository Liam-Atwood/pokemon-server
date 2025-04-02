// Liam Atwood
// Mini Project
// COIS 3380

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "pokemon.h" 

int main() {
    FILE *infile = NULL;
    char filename[MAX_FSTR];
    int input = 0;
    int numSearches = 0;
    int numFiles = 0;
    pthread_mutex_init(&mutex, NULL);

    // Get the name of the pokemon file from the user
    while (infile == NULL) {
        printf("Enter the name of the pokemon file or 'exit' to quit: ");
        scanf("%s", filename);

        if (strcmp(filename, "exit") == 0) {
            printf("Exiting program...\n");
            exit(1);
        }

        infile = fopen(filename, "r");
        
        // Check if the file was opened
        if (infile == NULL) {
            printf("\nPokemon file is not found. Please enter the name of the file again.\n");
        }
    }
    fclose(infile);

    // Menu loop
    while (input != 3) {
        printf ("\n1. Type search\n2. Save results\n3. Exit the program\n");
        printf("\nEnter choice: ");
        scanf("%d", &input);

        switch (input) {
            // Search
            case 1: {
                char type[MAX_STR];
                printf("Enter the type of pokemon you want to search for: ");
                scanf("%s", type);

                Search *search = malloc(sizeof(Search));
                strcpy(search->filename, filename);
                strcpy(search->type, type);

                // Create a thread to search for the pokemon
                pthread_t thread1;
                int r = pthread_create(&thread1, NULL, searchThread, search);
                if (r != 0) {
                    printf("Unable to start the query!\n");
                    free(search);
                    break;
                }
                
                pthread_mutex_lock(&mutex);
                threads[numSearchThreads++] = thread1;
                pthread_mutex_unlock(&mutex);

                numSearches++;
                break;
            }
            // Save
            case 2: {
                char filename2[MAX_FSTR];
                int valid = 0;

                // Test if the file can be written to
                while (!valid) {
                    printf("Enter the name of the file you want to save the results to: ");
                    scanf("%s", filename2);
                    FILE *test = fopen(filename2, "w");
                    if (test == NULL) {
                        printf("Unable to create the new file. " 
                            "Please enter the name of the file again.\n");
                    } 
                    else {
                        fclose(test);
                        valid = 1;
                    }
                }

                // Create a thread to save the results
                pthread_t thread2;

                // Allocate memory for the filename for threading
                char *filename2copy = malloc(strlen(filename2) + 1);
                strcpy(filename2copy, filename2);
                
                pthread_create(&thread2, NULL, saveThread, filename2copy);
                pthread_detach(thread2); // Detach the thread to allow it to run in the background

                // Store the filename in array
                strcpy(files[numFiles], filename2);
                numFiles++;
                break;
            }
            // Exit
            case 3:
                printf("\nNumber of successful queries: %d\n", numSearches);
                printf("Names of new files created:\n");
                for (int i = 0; i < numFiles; i++) {
                    printf(" * %s\n", files[i]);
                }
                printf("\nExiting program...\n");
                cleanup(); // Clean up resources
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}