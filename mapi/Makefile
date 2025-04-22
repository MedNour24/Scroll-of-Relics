prog: main.o map.o perso.o ennemi.o
	gcc main.o map.o perso.o ennemi.o -o prog -lSDL -g -lSDL_image -lSDL_mixer -lSDL_ttf

main.o: main.c map.h perso.h ennemi.h
	gcc -c main.c -g

map.o: map.c map.h
	gcc -c map.c -g

perso.o: perso.c perso.h
	gcc -c perso.c -g

ennemi.o: ennemi.c ennemi.h map.h
	gcc -c ennemi.c -g
clean:
	rm -f *.o prog
