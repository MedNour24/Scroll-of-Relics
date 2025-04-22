prog: main.o game.o
	gcc main.o game.o -o game -lSDL -lSDL_image -lSDL_ttf -lSDL_mixer

main.o: main.c
	gcc -c main.c -g

game.o: game.c
	gcc -c game.c -g

clean:
	rm -f *.o game

