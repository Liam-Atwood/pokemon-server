// Liam Atwood
// Mini Project
// COIS 3380

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "pokemon.h"

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
    pthread_mutex_lock(&mutex);
    for (int i=0; i<numResults; i++) {
        Results *result = results[i];
        for (int j=0; j<result->numPokemon; j++) {
            Pokemon p = result->pokemon[j];
            fprintf(outfile, "%d,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%s\n",
                p.id, p.name, p.type1, p.type2,
                p.total, p.hp, p.attack, p.defense,
                p.spAtk, p.spDef, p.speed, p.generation,
                p.legendary);
        }
    }
    pthread_mutex_unlock(&mutex);

    fclose(outfile);
    return 1;
}

/*
Purpose: Thread function to save the pokemon results to a file

input:
    str - the name of the file to save the results to
*/
void *saveThread(void *str) {
    pthread_mutex_lock(&mutex);
    for (int i=0; i<numSearchThreads; i++) {
        pthread_join(threads[i], NULL);
    }
    numSearchThreads = 0;
    pthread_mutex_unlock(&mutex);

    // Copy the filename and run savePokemon in the thread
    char *filename = (char*)str;
    int response = savePokemon(filename);

    if (response == 0) {
    printf("Unable to create new file.\n");
    }

    free(filename);
    return NULL;
}
