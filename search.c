// Liam Atwood
// Mini Project
// COIS 3380

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "pokemon.h"

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

    Results *searchResults = malloc(sizeof(Results));
    searchResults->numPokemon = 0;

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
            if (searchResults->numPokemon < MAX_POKEMON) {
                searchResults->pokemon[searchResults->numPokemon++] = p;
            } else {
                printf("Maximum number of pokemon reached. Cannot add more pokemon.\n");
                break;
            }
        }
    }
    fclose(infile);

    // Lock the mutex to safely add results]
    pthread_mutex_lock(&mutex);
    if (numResults < MAX_RESULTS) {
        results[numResults++] = searchResults;
    } else {
        printf("Maximum number of results reached. Cannot add more results.\n");
        free(searchResults);
    }
    pthread_mutex_unlock(&mutex);
}

void *searchThread(void *str) {
    // Copy the filename and type and run searchPokemon in the thread
    Search *search = (Search*)str;
    
    searchPokemon(search->filename, search->type);

    free(search);
    return NULL;
}