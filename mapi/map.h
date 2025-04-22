// map.h
#ifndef MAP_H
#define MAP_H

#include <SDL/SDL.h>
#include "perso.h"
#include "ennemi.h"



typedef struct {
    SDL_Surface* sprite;
    SDL_Rect posSprite;
    int frame_count;
    int current_frame;
    int active;
} Animation;

typedef struct {
    SDL_Surface* surface_level1; // Minimap du niveau 1
    SDL_Surface* surface_level2; // Minimap du niveau 2
    SDL_Surface* mask_level1;    // Masque de collision du niveau 1
    SDL_Surface* mask_level2;    // Masque de collision du niveau 2
    SDL_Rect pos_map;
    SDL_Surface* mini_perso;
    SDL_Rect pos_mini_perso;
    SDL_Surface* mini_ennemi;
    SDL_Rect pos_mini_ennemi;
    Animation anim;
    int level; // Niveau actuel (1 ou 2)
} minimap;

void initMinimap(minimap* m, SDL_Surface* screen, int level);
void perso_map(minimap* m, joueur* p);
void ennemi_map(minimap* m, ennemi* e);
void afficher(minimap m, SDL_Surface* screen);
void freeMinimap(minimap* m);
void niveau(minimap* m, joueur* p); // Nouvelle fonction

SDL_Color getpixel(SDL_Surface *psurface, int x, int y);
int collisionparfaite(SDL_Surface *mask, joueur p);
void collisionpos(SDL_Surface *mask, joueur *p, int direction);
int collision_bb(SDL_Rect a, SDL_Rect b);

void initAnimation(Animation* anim, const char* filename, int frame_width, int frame_height, int frame_count);
void updateAnimation(Animation* anim);
void freeAnimation(Animation* anim);

#endif
