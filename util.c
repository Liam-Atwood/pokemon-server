// Liam Atwood
// Mini Project
// COIS 3380

#include <stdlib.h>

#include "pokemon.h"

Results *results[MAX_RESULTS];
int numResults = 0;
pthread_mutex_t mutex;
char files[MAX_FILES][MAX_FSTR];
pthread_t threads[MAX_SEARCHES];
int numSearchThreads = 0;

void cleanup() {
    for (int i = 0; i < numSearchThreads; i++) {
        pthread_cancel(threads[i]);
    }

    for (int i = 0; i < numResults; i++) {
        free(results[i]);
    }
    pthread_mutex_destroy(&mutex);
}