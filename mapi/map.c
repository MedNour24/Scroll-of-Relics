#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "map.h"
#include "perso.h"
#include "ennemi.h"

void initMinimap(minimap* m, SDL_Surface* screen, int level) {
    m->surface_level1 = NULL;
    m->surface_level2 = NULL;
    m->mask_level1 = NULL;
    m->mask_level2 = NULL;
    m->mini_perso = NULL;
    m->mini_ennemi = NULL;

    if (level == 1) {
        m->surface_level1 = IMG_Load("minimap.bmp");
        if (!m->surface_level1) {
            printf("Failed to load minimap1.bmp: %s\n", IMG_GetError());
        }
        m->mask_level1 = IMG_Load("bgMasque.bmp");
        if (!m->mask_level1) {
            printf("Failed to load mask1.bmp: %s\n", IMG_GetError());
        }
    } else if (level == 2) {
        m->surface_level2 = IMG_Load("minimap2.bmp");
        if (!m->surface_level2) {
            printf("Failed to load minimap2.bmp: %s\n", IMG_GetError());
        }
        m->mask_level2 = IMG_Load("bg2Masque.bmp");
        if (!m->mask_level2) {
            printf("Failed to load mask2.bmp: %s\n", IMG_GetError());
        }
    }

    m->pos_map.x = 1000;
    m->pos_map.y = 100;

    m->mini_perso = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 20, 20, 32, 0, 0, 0, 0);
    SDL_FillRect(m->mini_perso, NULL, SDL_MapRGB(screen->format, 255, 0, 0));

    m->pos_mini_perso.x = 0;
    m->pos_mini_perso.y = 0;

    m->mini_ennemi = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_DOUBLEBUF, 20, 20, 32, 0, 0, 0, 0);
    SDL_FillRect(m->mini_ennemi, NULL, SDL_MapRGB(screen->format, 0, 0, 255));
    m->pos_mini_ennemi.x = 0;
    m->pos_mini_ennemi.y = 0;

    m->level = level;
}

void perso_map(minimap* m, joueur* p) {
    m->pos_mini_perso.x = p->pos_j.x / 3 + m->pos_map.x;
    m->pos_mini_perso.y = p->pos_j.y / 3 + m->pos_map.y;
}

void ennemi_map(minimap* m, ennemi* e) {
    m->pos_mini_ennemi.x = e->pos_e.x / 3 + m->pos_map.x;
    m->pos_mini_ennemi.y = e->pos_e.y / 3 + m->pos_map.y;
}

void afficher(minimap m, SDL_Surface* screen) {
      SDL_BlitSurface(m.surface_level1, NULL, screen, &m.pos_map);
      SDL_BlitSurface(m.surface_level2, NULL, screen, &m.pos_map);

      SDL_BlitSurface(m.mini_perso, NULL, screen, &m.pos_mini_perso);
      SDL_BlitSurface(m.mini_ennemi, NULL, screen, &m.pos_mini_ennemi);
    
}

void freeMinimap(minimap* m) {

    if (m->surface_level1) SDL_FreeSurface(m->surface_level1);
    if (m->surface_level2) SDL_FreeSurface(m->surface_level2);
    if (m->mask_level1) SDL_FreeSurface(m->mask_level1);
    if (m->mask_level2) SDL_FreeSurface(m->mask_level2);
   SDL_FreeSurface(m->mini_perso);
   SDL_FreeSurface(m->mini_ennemi);
   freeAnimation(&m->anim);
}

SDL_Color getpixel(SDL_Surface *pSurface, int x, int y) {
    SDL_Color color = {0, 0, 0};
    if (!pSurface || x < 0 || y < 0 || x >= pSurface->w || y >= pSurface->h) {
        printf("Invalid pixel access at (%d,%d) for surface %p\n", x, y, pSurface);
        return color;
    }
    Uint32 col = 0;
    char *pPosition = (char *)pSurface->pixels;
    pPosition += (pSurface->pitch * y);
    pPosition += (pSurface->format->BytesPerPixel * x);
    memcpy(&col, pPosition, pSurface->format->BytesPerPixel);
    SDL_GetRGB(col, pSurface->format, &color.r, &color.g, &color.b);
    return color;
}

int collisionparfaite(SDL_Surface *mask, joueur p) {
       int tabx[8], taby[8];
    SDL_Color color, color_obstacle = {0, 0, 0};


    tabx[0] = p.pos_j.x;           
    taby[0] = p.pos_j.y;  
             

    tabx[1] = p.pos_j.x + p.pos_j.w / 2; 
    taby[1] = p.pos_j.y;   
     

    tabx[2] = p.pos_j.x + p.pos_j.w; 
    taby[2] = p.pos_j.y;   
    

    tabx[3] = p.pos_j.x;           
    taby[3] = p.pos_j.y + p.pos_j.h / 2;


    tabx[4] = p.pos_j.x;           
    taby[4] = p.pos_j.y + p.pos_j.h; 
    

    tabx[5] = p.pos_j.x + p.pos_j.w / 2; 
    taby[5] = p.pos_j.y + p.pos_j.h;
     

    tabx[6] = p.pos_j.x + p.pos_j.w; 
    taby[6] = p.pos_j.y + p.pos_j.h;    
    

    tabx[7] = p.pos_j.x + p.pos_j.w; 
    taby[7] = p.pos_j.y + p.pos_j.h / 2; 

    for (int i = 0; i < 8; i++) {
        color = getpixel(mask, tabx[i], taby[i]);
        printf("Test point %d: RGB(%d,%d,%d)\n", i, color.r, color.g, color.b);
        if (color.r < 30 && color.g < 30 && color.b < 30) {
    return i; 
}
    }
    return -1; 
}

void collisionpos(SDL_Surface *mask, joueur *p, int direction) {
    SDL_Rect next_pos = p->pos_j;

    switch (direction) {
        case SDLK_RIGHT: next_pos.x += 5; break;
        case SDLK_LEFT:  next_pos.x -= 5; break;
        case SDLK_UP:    next_pos.y -= 5; break;
        case SDLK_DOWN:  next_pos.y += 5; break;
    }

    // Vérifie si cette nouvelle position est en collision
    joueur temp = *p;
    temp.pos_j = next_pos;

    if (collisionparfaite(mask, temp) == -1) {
        p->pos_j = next_pos; // Aucun problème, on déplace
    } else {
        printf("Collision parfaite détectée, déplacement bloqué\n");
    }
}
int collision_bb(SDL_Rect a, SDL_Rect b) {
    if ((a.x + a.w <= b.x) || // a complètement à gauche de b
        (a.x >= b.x + b.w) || // a complètement à droite de b
        (a.y + a.h <= b.y) || // a complètement au-dessus de b
        (a.y >= b.y + b.h)) { // a complètement en-dessous de b
        return 0; 
    }
    return 1;
}

void initAnimation(Animation* anim, const char* filename, int frame_width, int frame_height, int frame_count) {
    anim->sprite = IMG_Load(filename);
    if (!anim->sprite) {
        printf("Failed to load %s: %s\n", filename, IMG_GetError());
        return;
    }

    anim->posSprite.x = 0;
    anim->posSprite.y = 0;
    anim->posSprite.w = frame_width;
    anim->posSprite.h = frame_height;
    anim->frame_count = frame_count;
    anim->current_frame = 0;
    anim->active = 1;
}

void updateAnimation(Animation* anim) {
    static int counter = 0;
    counter++;
    
    if (counter >= 10) {
        anim->current_frame = (anim->current_frame + 1) % anim->frame_count;
        
        int cols = anim->sprite->w / anim->posSprite.w;
        if (cols == 0) cols = 1;

        anim->posSprite.x = (anim->current_frame % cols) * anim->posSprite.w;
        anim->posSprite.y = (anim->current_frame / cols) * anim->posSprite.h;
        
        counter = 0;
    }
}

void freeAnimation(Animation* anim) {
    if (anim->sprite) {
        SDL_FreeSurface(anim->sprite);
        anim->sprite = NULL;
    }
}
