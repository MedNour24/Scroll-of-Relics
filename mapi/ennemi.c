#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "ennemi.h"
#include "map.h"

void initialiserEnnemi(ennemi* e, SDL_Surface* screen) {
    e->img_e = IMG_Load("ennemi.bmp");
    if (!e->img_e) {
        printf("Failed to load ennemi.bmp: %s\n", IMG_GetError());
        return;
    }
    e->pos_e.x = 300;
    e->pos_e.y = 400;
    e->pos_e.w = 100;
    e->pos_e.h = 100;
}

void afficher_ennemi(SDL_Surface* screen, ennemi e) {
    SDL_BlitSurface(e.img_e, NULL, screen, &e.pos_e);
}

void deplacer_ennemi(ennemi* e, SDL_Event event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_z:
                e->pos_e.y -= 5;
                break;
            case SDLK_s:
                e->pos_e.y += 5;
                break;
            case SDLK_d:
                e->pos_e.x += 5;
                break;
            case SDLK_q:
                e->pos_e.x -= 5;
                break;
        }
    }
}

void free_ennemi(ennemi e) {
    if (e.img_e) {
        SDL_FreeSurface(e.img_e);
    }
}
