#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "map.h"
#include "perso.h"
#include "ennemi.h"

int main() {
    int continuer = 1;
    SDL_Surface *screen = NULL, *backg = NULL, *mask=NULL;
    SDL_Rect backg_pos = {0, 0, 0, 0}; // Position du fond
    Animation bg_anim;                 // Animation du background

    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation de l’écran
    screen = SDL_SetVideoMode(1184, 765, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (!screen) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    if (!(screen->flags & SDL_DOUBLEBUF)) {
        printf("Warning: Double buffering not enabled\n");
    }
    printf("Screen set up successfully.\n");

    // Get level input
    int level;
    printf("ecrire 1 si lvl1 ou ecrire 2 si lvl2: ");
    scanf("%d", &level);

    // Load background based on level
    if (level == 1) {
        backg = IMG_Load("bg.bmp");
        if (!backg) {
            printf("Failed to load bg.bmp: %s\n", IMG_GetError());
            SDL_Quit();
            return 1;
        }
    } else if (level == 2) {
        backg = IMG_Load("bg2.bmp");
        if (!backg) {
            printf("Failed to load bg2.bmp: %s\n", IMG_GetError());
            SDL_Quit();
            return 1;
        }
    } else {
        printf("Invalid level choice. Use 1 or 2.\n");
        SDL_Quit();
        return 1;
    }

    // Initialisation de l'animation du background
    initAnimation(&bg_anim, "animation.bmp", 308, 202, 4); // 4 frames de 308x202
    if (!bg_anim.sprite) {
        printf("Failed to initialize background animation\n");
        SDL_FreeSurface(backg);
        SDL_Quit();
        return 1;
    }

    // Initialisation des éléments du jeu
    minimap m;
    joueur p;
    ennemi e;

    initMinimap(&m, screen, level);
    initialiserperso(&p);
    initialiserEnnemi(&e, screen);

    // Reset positions for level 2 to ensure valid starting points
    if (level == 2) {
        p.pos_j.x = 100;
        p.pos_j.y = 100;
        e.pos_e.x = 300;
        e.pos_e.y = 300;
        printf("Level 2: Player initialized at (%d,%d), Enemy at (%d,%d)\n",
               p.pos_j.x, p.pos_j.y, e.pos_e.x, e.pos_e.y);
    }

    // Exemple de plateforme mobile pour collision BB
    SDL_Rect platform_bb = {500, 300, 50, 50};

    SDL_Event event;
    int direction = 0; // Pour la collision parfaite
    int anim_timer = 0; // Timer pour l'animation après collision

    while (continuer) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            continuer = 0;
                            break;
                        case SDLK_RIGHT:
                            direction = SDLK_RIGHT;
                            deplacer_perso(&p, direction);
                            break;
                        case SDLK_LEFT:
                            direction = SDLK_LEFT;
                            deplacer_perso(&p, direction);
                            break;
                        case SDLK_UP:
                            direction = SDLK_UP;
                            deplacer_perso(&p, direction);
                            break;
                        case SDLK_DOWN:
                            direction = SDLK_DOWN;
                            deplacer_perso(&p, direction);
                            break;
                        case SDLK_z:
                        case SDLK_q:
                        case SDLK_s:
                        case SDLK_d:
                            deplacer_ennemi(&e, event); // Déplacer l'ennemi
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_RIGHT:
                        case SDLK_LEFT:
                        case SDLK_UP:
                        case SDLK_DOWN:
                            direction = 0; // Réinitialiser la direction
                            break;
                    }
                    break;
            }
        }

        // Vérification de la collision parfaite avec le masque
        if (direction) {
            collisionpos(mask, &p, direction); // Use minimap's mask
        }

        // Vérification de la collision BB avec l'ennemi
        if (collision_bb(p.pos_j, e.pos_e)) {
            bg_anim.active = 1; // Activer l'animation lors de la collision
            anim_timer = 0;     // Réinitialiser le timer
            printf("Collision BB détectée, animation activée\n");
        }

        // Gérer la durée de l'animation (ex. 40 frames ~ 400ms)
        if (bg_anim.active) {
            updateAnimation(&bg_anim);
            anim_timer++;
            if (anim_timer >= 40) {
                bg_anim.active = 0; // Désactiver après 400ms
                bg_anim.current_frame = 0;
                bg_anim.posSprite.x = 0;
                bg_anim.posSprite.y = 0;
                printf("Animation terminée\n");
            }
        }

        // Mise à jour de la minimap
        perso_map(&m, &p);
        ennemi_map(&m, &e);

        // Clear screen to prevent sprite repetition
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        // Affichage
        SDL_BlitSurface(backg, NULL, screen, &backg_pos); // D'abord le fond
        if (bg_anim.sprite && bg_anim.active) {
            SDL_Rect bg_anim_pos = {0, 0, 617, 404}; // Animation au-dessus
            SDL_BlitSurface(bg_anim.sprite, &bg_anim.posSprite, screen, &bg_anim_pos);
        }
        afficher_perso(screen, p);
        afficher_ennemi(screen, e);
        afficher(m, screen); // Minimap en dernier

        SDL_Flip(screen);
    }

    // Libération des ressources
    freeAnimation(&bg_anim);
    freeMinimap(&m);
    free_perso(p);
    free_ennemi(e);
    SDL_FreeSurface(backg);
    SDL_Quit();

    return 0;
}
