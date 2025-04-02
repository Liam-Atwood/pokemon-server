// Liam Atwood
// Mini Project
// COIS 3380

#include <pthread.h>

// Define constants
#define MAX_STR 32
#define MAX_FSTR 128
#define MAX_POKEMON 1000
#define MAX_FILES 10
#define MAX_RESULTS 10
#define MAX_SEARCHES 10

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

typedef struct {
    char filename[MAX_FSTR];
    char type[MAX_STR];
} Search;

typedef struct {
    Pokemon pokemon[MAX_POKEMON];
    int numPokemon;
} Results;

// Global variables
extern Results *results[MAX_RESULTS];
extern int numResults;
extern pthread_mutex_t mutex;
extern char files[MAX_FILES][MAX_FSTR];
extern pthread_t threads[MAX_SEARCHES];
extern int numSearchThreads;

// Methods
void searchPokemon(char*, char*);
void *searchThread(void*);
int savePokemon(char*);
void *saveThread(void*);
void cleanup();