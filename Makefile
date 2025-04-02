CC=gcc
CCOPTIONS=-Wall -lpthread
OBJ=main.o search.o save.o util.o

pokemon: $(OBJ)
	$(CC) $(CCOPTIONS) -o pokemon $(OBJ)

main.o: main.c pokemon.h
	$(CC) $(CCOPTIONS) -c main.c

search.o: search.c pokemon.h
	$(CC) $(CCOPTIONS) -c search.c

save.o: save.c pokemon.h
	$(CC) $(CCOPTIONS) -c save.c

util.o: util.c pokemon.h
	$(CC) $(CCOPTIONS) -c util.c

clean:
	rm -f $(OBJ) pokemon