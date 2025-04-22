#ifndef ENNEMI_H
#define ENNEMI_H
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>



typedef struct {
    SDL_Surface* img_e;       // Image de l'ennemi dans le jeu
    SDL_Rect pos_e;           // Position de l'ennemi dans le jeu
    SDL_Surface* mini_ennemi; // Surface de l'ennemi sur la minimap
    SDL_Rect pos_mini_ennemi; // Position de l'ennemi sur la minimap
} ennemi;

void initialiserEnnemi(ennemi* e, SDL_Surface* screen);
void afficher_ennemi(SDL_Surface* screen, ennemi e);
void deplacer_ennemi(ennemi* e, SDL_Event event);
void free_ennemi(ennemi e);

#endif
