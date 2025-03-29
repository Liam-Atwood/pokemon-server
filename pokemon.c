// Liam Atwood
// Mini Project
// COIS 3380

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

// Define constants
#define MAX_STR 32
#define MAX_FSTR 128
#define MAX_POKEMON 1000
#define MAX_FILES 10

// Define the Pokemon details structure
typedef struct {
    int id;
    char name[MAX_STR];
    char type1[MAX_STR];
    char type2[MAX_STR];
    int total;
    int hp;
    int attack;
    int defense;
    int spAtk;
    int spDef;
    int speed;
    int generation;
    char legendary[MAX_STR];
} Pokemon;

// Define global variables
Pokemon results[MAX_POKEMON];
int numPokemon = 0;

char files[MAX_FILES][MAX_FSTR];
int numFiles = 0;

// Forward declaration of methods
void searchPokemon(char*, char*);
int savePokemon(char*);
void *saveThread(void*);

int main() {
    FILE *infile = NULL;
    char filename[MAX_FSTR];
    int input = 0;
    int numSearches = 0;

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
                searchPokemon(filename, type);
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
                pthread_t thread;

                // Allocate memory for the filename for threading
                char *filename2copy = malloc(strlen(filename2) + 1);
                strcpy(filename2copy, filename2);
                
                pthread_create(&thread, NULL, saveThread, filename2copy);
                pthread_detach(thread); // Detach the thread to allow it to run in the background

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
                exit(0);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }

    return 0;
}

/*
Purpose: Search for pokemon of a specific type 1 in the given file

input:
    filename - the name of the file to search
    type - type 1 of pokemon to search for
*/
void searchPokemon(char* filename, char* type) {
    FILE *infile = fopen(filename, "r");
    if (infile == NULL) {
        printf("Error opening file. Please try again\n");
        return;
    }

    char line[512]; // Buffer for each line

    fgets(line, sizeof(line), infile); // Skip the first line (headers)

    // Read each line of the file
    while (fgets(line, sizeof(line), infile) != NULL) {
        Pokemon p;
        char *ptr = line;
        char *token;
        int column = 0;

        memset(&p, 0, sizeof(Pokemon)); // Ensure values are empty

        // Tokenize the line by commas
        while (column < 13) {
            token = strsep(&ptr, ",");

            // Handle empty columns
            if (token == NULL) {
                token = "";
            }

            // Add each column to the pokemon struct
            switch (column) {
                case 0:
                    p.id = atoi(token);
                    break;
                case 1:
                    strcpy(p.name, token);
                    break;
                case 2:
                    strcpy(p.type1, token);
                    break;
                case 3:
                    strcpy(p.type2, token);
                    break;
                case 4:
                    p.total = atoi(token);
                    break;
                case 5:
                    p.hp = atoi(token);
                    break;
                case 6:
                    p.attack = atoi(token);
                    break;
                case 7:
                    p.defense = atoi(token);
                    break;
                case 8:
                    p.spAtk = atoi(token);
                    break;
                case 9:
                    p.spDef = atoi(token);
                    break;
                case 10:
                    p.speed = atoi(token);
                    break;
                case 11:
                    p.generation = atoi(token);
                    break;
                case 12:
                    // Remove newline character from last column
                    if (token != NULL && strlen(token) > 0) {
                        token[strcspn(token, "\n")] = '\0';
                    }
                    strcpy(p.legendary, token);
                    break;
            }
            column++;
        }

        // Find the pokemon type 1 matches the search type
        if (strcasecmp(p.type1, type) == 0) {
            if (numPokemon < MAX_POKEMON) {
                results[numPokemon++] = p;
            } else {
                printf("Maximum number of pokemon reached. Cannot add more pokemon.\n");
                break;
            }
        }
    }

    fclose(infile);
}

/*
Purpose: Write the pokemon results to a file

input:
    outFilename - the name of the file to save the results to

return:
    int - 1 if successful, 0 if not
*/
int savePokemon(char* outFilename) {
    FILE *outfile = fopen(outFilename, "w");

    if (outfile == NULL) {
        return 0; // Unable to write file
    }

    // Add headers
    fprintf(outfile, "#,Name,Type 1,Type 2,Total,HP,Attack,Defense,"
        "Sp. Atk,Sp. Def,Speed,Generation,Legendary\n");

    // Write each pokemon to file
    for (int i=0; i<numPokemon; i++) {
        fprintf(outfile, "%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
            results[i].id, results[i].name, results[i].type1, results[i].type2,
            results[i].total, results[i].hp, results[i].attack, results[i].defense,
            results[i].spAtk, results[i].spDef, results[i].speed, results[i].generation,
            results[i].legendary);
    }

    fclose(outfile);
    return 1;
}

/*
Purpose: Thread function to save the pokemon results to a file

input:
    str - the name of the file to save the results to
*/
void *saveThread(void *str) {
    // Copy the filename and run savePokemon in the thread
    char *filename = (char*)str;
    int response = savePokemon(filename);

    if (response == 0) {
    printf("Unable to create new file.\n");
    }

    free(filename);
    return NULL;
}
