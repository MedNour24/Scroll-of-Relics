prog: main.o personne.o
	gcc main.o personne.o -o prog -lSDL -g -lSDL_image -lSDL_mixer -lSDL_ttf

main.o: main.c personne.h
	gcc -c main.c -g

personne.o: personne.c personne.h
	gcc -c personne.c -g

clean:
	rm -f prog *.o
