// perso.h
#ifndef PERSO_H
#define PERSO_H

#include <SDL/SDL.h>

typedef struct {
    SDL_Surface* img_j; // Image du joueur
    SDL_Rect pos_j;     // Position et dimensions du joueur
} joueur;

void initialiserperso(joueur *p);
void afficher_perso(SDL_Surface *screen, joueur p);
void deplacer_perso(joueur *p, int direction);
void free_perso(joueur p);

#endif
