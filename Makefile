CC=gcc
CFLAGS=-Wall
game:
	echo "Compiling the game"
	$(CC) $(CFLAGS) main.c -lraylib -lm -o bin/game

clean:
	echo "Cleaning the build directory"
	rm -f game 
