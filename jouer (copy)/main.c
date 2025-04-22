#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h> 
#include <SDL/SDL_image.h> 
#include <stdio.h>
#include "personne.h"

int main(int argc, char *argv[]) {
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Échec de l'initialisation SDL : %s\n", SDL_GetError());
        return 1;
    }

    
    if (TTF_Init() < 0) {
        fprintf(stderr, "Échec de l'initialisation SDL_ttf : %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    
    SDL_Surface* screen = SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (screen == NULL) {
        fprintf(stderr, "Impossible de définir le mode vidéo : %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (font == NULL) {
        fprintf(stderr, "Erreur de chargement de la police : %s\n", TTF_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    
    SDL_Surface* heart = IMG_Load("heart.png");
    if (heart == NULL) {
        fprintf(stderr, "Erreur de chargement de heart.png : %s\n", TTF_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    
    personnage p1, p2;
    initialiser_personnage(&p1);
    initialiser_personnage(&p2);
    p2.position.x = 100;

    
    int active_player = 0;
    printf("Choisissez votre joueur :\n");
    printf("1 - Joueur 1 (Touches fléchées)\n");
    printf("2 - Joueur 2 (WASD)\n");
    printf("Entrez 1 ou 2 : ");
    scanf("%d", &active_player);
    while (active_player != 1 && active_player != 2) {
        printf("Choix invalide. Entrez 1 ou 2 : ");
        scanf("%d", &active_player);
    }
    printf("Vous avez choisi Joueur %d\n", active_player);

    int running = 1;
    SDL_Event event;
    Uint32 t_prev = SDL_GetTicks();

    
    while (running) {
        Uint32 t_now = SDL_GetTicks();
        Uint32 dt = t_now - t_prev;
        t_prev = t_now;

        
        while (SDL_PollEvent(&event)) {
            handle_input(event, &p1, &p2, &running, active_player);
        }

        
        if (active_player == 1) {
            movePerso(&p1, dt);
        }
        if (active_player == 2) {
            movePerso(&p2, dt);
        }

       
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        
        if (active_player == 1) {
            afficher_personnage(p1, screen);
        }
        if (active_player == 2) {
            afficher_personnage(p2, screen);
        }

        
        char score_text[32];
        SDL_Color white = {255, 255, 255, 0}; 
        if (active_player == 1) {
            sprintf(score_text, "Score: %d", p1.score);
        }
        if (active_player == 2) {
            sprintf(score_text, "Score: %d", p2.score);
        }
        SDL_Surface* score_surface = TTF_RenderText_Solid(font, score_text, white);
        if (score_surface != NULL) {
            SDL_Rect score_pos = {10, 10, 0, 0}; 
            SDL_BlitSurface(score_surface, NULL, screen, &score_pos);
            SDL_FreeSurface(score_surface);
        }

        
        int lives;
        if (active_player == 1) {
            lives = p1.vie;
        }
        if (active_player == 2) {
            lives = p2.vie;
        }
        SDL_Rect heart_pos = {800 - 10, 10, 0, 0}; 
        for (int i = 0; i < lives && i < 3; i++) {
            heart_pos.x = heart_pos.x - 30; 
            SDL_BlitSurface(heart, NULL, screen, &heart_pos);
        }

        
        SDL_Flip(screen);
    }

    
    for (int i = 0; i < 8; i++) {
        if (p1.tab_right[i]) SDL_FreeSurface(p1.tab_right[i]);
        if (p1.tab_left[i]) SDL_FreeSurface(p1.tab_left[i]);
        if (p2.tab_right[i]) SDL_FreeSurface(p2.tab_right[i]);
        if (p2.tab_left[i]) SDL_FreeSurface(p2.tab_left[i]);
    }
    for (int i = 0; i < 5; i++) {
        if (p1.tab_attack_right[i]) SDL_FreeSurface(p1.tab_attack_right[i]);
        if (p1.tab_attack_left[i]) SDL_FreeSurface(p1.tab_attack_left[i]);
        if (p2.tab_attack_right[i]) SDL_FreeSurface(p2.tab_attack_right[i]);
        if (p2.tab_attack_left[i]) SDL_FreeSurface(p2.tab_attack_left[i]);
    }
    for (int i = 0; i < 1; i++) {
        if (p1.tab_jump_right[i]) SDL_FreeSurface(p1.tab_jump_right[i]);
        if (p1.tab_jump_left[i]) SDL_FreeSurface(p1.tab_jump_left[i]);
        if (p2.tab_jump_right[i]) SDL_FreeSurface(p2.tab_jump_right[i]);
        if (p2.tab_jump_left[i]) SDL_FreeSurface(p2.tab_jump_left[i]);
    }


    SDL_FreeSurface(heart);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}
