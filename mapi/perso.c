#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "perso.h"

void initialiserperso(joueur *p) {
    p->img_j = IMG_Load("hero.bmp");
    if (!p->img_j) {
        printf("Failed to load hero.bmp: %s\n", IMG_GetError());
        return;
    }
    p->pos_j.x = 0;
    p->pos_j.y = 400;
    p->pos_j.w = 100;
    p->pos_j.h = 100;
}

void afficher_perso(SDL_Surface *screen, joueur p) {
    SDL_BlitSurface(p.img_j, NULL, screen, &p.pos_j);
}

void deplacer_perso(joueur *p, int direction) {
    switch (direction) {
        case SDLK_UP:
            p->pos_j.y -= 5;
            break;
        case SDLK_DOWN:
            p->pos_j.y += 5;
            break;
        case SDLK_RIGHT:
            p->pos_j.x += 5;
            break;
        case SDLK_LEFT:
            p->pos_j.x -= 5;
            break;
    }
}

void free_perso(joueur p) {
    if (p.img_j) {
        SDL_FreeSurface(p.img_j);
    }
}
