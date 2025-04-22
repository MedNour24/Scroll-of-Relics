#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <math.h>
#include "personne.h"


void initialiser_personnage(personnage *p) {
    p->position.x = 0;         
    p->position.y = 300;       
    p->frame = 0;             
    p->vitesse = 0.0;          
    p->acceleration = 0.0;     
    p->up = 0;                 
    p->vy = 0.0;               
    p->status = STAT_SOL;      
    p->vie = 3;                
    p->score = 0;              
    p->direction = 0;          
    p->attack_frame = 0;       
    p->jump_frame = 0;         
    p->show_guide = 0;        


    char nomFich[20];
    for (int i = 0; i < 8; i++) {
        sprintf(nomFich, "%d.png", i + 1); 
        p->tab_right[i] = IMG_Load(nomFich);
        if (p->tab_right[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image droite %s: %s\n", nomFich, IMG_GetError());
        }
    }

   
    for (int i = 0; i < 8; i++) {
        sprintf(nomFich, "%d_left.png", i + 1); 
        p->tab_left[i] = IMG_Load(nomFich);
        if (p->tab_left[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image gauche %s: %s\n", nomFich, IMG_GetError());
        }
    }

    
    for (int i = 0; i < 6; i++) {
        sprintf(nomFich, "attack%d.png", i + 1); 
        p->tab_attack_right[i] = IMG_Load(nomFich);
        if (p->tab_attack_right[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image d'attaque droite %s: %s\n", nomFich, IMG_GetError());
        }
    }

    
    for (int i = 0; i < 6; i++) {
        sprintf(nomFich, "attack%d_left.png", i + 1); 
        p->tab_attack_left[i] = IMG_Load(nomFich);
        if (p->tab_attack_left[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image d'attaque gauche %s: %s\n", nomFich, IMG_GetError());
        }
    }

    
    for (int i = 0; i < 1; i++) {
        sprintf(nomFich, "jump%d.png", i + 1); 
        p->tab_jump_right[i] = IMG_Load(nomFich);
        if (p->tab_jump_right[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image de saut droite %s: %s\n", nomFich, IMG_GetError());
        }
    }

    
    for (int i = 0; i < 1; i++) {
        sprintf(nomFich, "jump%d_left.png", i + 1); 
        p->tab_jump_left[i] = IMG_Load(nomFich);
        if (p->tab_jump_left[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de l'image de saut gauche %s: %s\n", nomFich, IMG_GetError());
        }
    }
}


void afficher_personnage(personnage p, SDL_Surface *ecran) {
    SDL_Surface *current_frame = NULL;
    if (p.status == STAT_ATTACK) {
       
        if (p.direction >= 0) { 
            current_frame = p.tab_attack_right[p.attack_frame];
        } else { 
            current_frame = p.tab_attack_left[p.attack_frame];
        }
    } else if (p.status == STAT_AIR) {
        
        if (p.direction >= 0) {
            current_frame = p.tab_jump_right[p.jump_frame];
        } else {
            current_frame = p.tab_jump_left[p.jump_frame];
        }
    } else {
        
        if (p.direction >= 0) { 
            current_frame = p.tab_right[p.frame];
        } else { 
            current_frame = p.tab_left[p.frame];
        }
    }

    if (current_frame) {
        SDL_BlitSurface(current_frame, NULL, ecran, &p.position);
    } else {
        fprintf(stderr, "Aucune surface à afficher pour frame %d, attack_frame %d, jump_frame %d, status %d, direction %d\n", 
                p.frame, p.attack_frame, p.jump_frame, p.status, p.direction);
    }


    if (p.show_guide) {
        TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
        if (!font) {
            fprintf(stderr, "Erreur de chargement de la police: %s\n", TTF_GetError());
            return;
        }
        SDL_Color textColor = {255, 255, 255, 0}; 
        const char *lines[] = {
            "Guide des Mouvements:",
            "Joueur 1:",
            "DROITE: Marcher a droite",
            "GAUCHE: Marcher a gauche",
            "Shift + DROITE/GAUCHE: Sprinter",
            "HAUT: Sauter",
            "Joueur 2:",
            "D: Marcher a droite",
            "A: Marcher a gauche",
            "Shift + D/A: Sprinter",
            "W: Sauter",
            "G: Afficher/Cacher ce guide",
            "ECHAP: Quitter"
        };
        int num_lines = 13;
        SDL_Rect textPos = {100, 200, 0, 0}; 
        for (int i = 0; i < num_lines; i++) {
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, lines[i], textColor);
            if (textSurface) {
                SDL_BlitSurface(textSurface, NULL, ecran, &textPos);
                SDL_FreeSurface(textSurface);
                textPos.y += 30; 
            }
        }
        TTF_CloseFont(font);
    }
}


void animate_personnage(personnage *p) {
    p->frame = (p->frame + 1) % 8; 
}


void movePerso(personnage *p, Uint32 dt) {
    double dt_seconds = dt / 1000.0;
    const double max_speed = 200.0;       
    const double sprint_speed = 400.0;    
    const double gravity = 500.0;
    const int ground_level = 300; 
    static Uint32 last_anim_time = 0;
    const Uint32 anim_interval = 100; 

    
    double current_max_speed = max_speed;
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) {
        current_max_speed = sprint_speed;
    }

    
    p->vitesse += p->acceleration * dt_seconds;
    
   
    if (p->vitesse > current_max_speed) p->vitesse = current_max_speed;
    if (p->vitesse < -current_max_speed) p->vitesse = -current_max_speed;
    
  
    p->position.x += p->vitesse * dt_seconds;


    if (p->status == STAT_AIR) {
        p->vy += gravity * dt_seconds;
        p->position.y += p->vy * dt_seconds;
        if (p->position.y >= ground_level) {
            p->position.y = ground_level;
            p->vy = 0.0;
            p->status = STAT_SOL;
            p->up = 0;
            p->jump_frame = 0;
        }
    }

    
    Uint32 current_time = SDL_GetTicks();
    if (p->status == STAT_ATTACK) {

        if (current_time - last_anim_time >= anim_interval) {
            p->attack_frame++;
            last_anim_time = current_time;
            if (p->attack_frame >= 5) { 
                p->attack_frame = 0;
                p->status = STAT_SOL;
            }
        }
    } else if (p->status == STAT_AIR) {
        
        if (current_time - last_anim_time >= anim_interval) {
            p->jump_frame = (p->jump_frame + 1) % 1; 
            last_anim_time = current_time;
        }
    } else if (p->acceleration != 0 && current_time - last_anim_time >= anim_interval) {
        
        animate_personnage(p);
        last_anim_time = current_time;
    } else if (p->acceleration == 0) {
        p->frame = 0; 
    }

    
    printf("Joueur: vitesse=%.2f, x=%d, y=%d, accel=%.2f, frame=%d, attack_frame=%d, jump_frame=%d, status=%d, dir=%d\n", 
           p->vitesse, p->position.x, p->position.y, p->acceleration, p->frame, p->attack_frame, p->jump_frame, p->status, p->direction);
}


void handle_input(SDL_Event event, personnage *p1, personnage *p2, int *running, int active_player) {
    const double jump_speed = -300.0;
    const double accel = 400.0;      
    const double sprint_accel = 800.0; 

    if (event.type == SDL_QUIT) {
        *running = 0;
    }
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT: 
                if (active_player == 1) {
                    
                    Uint8 *keystate = SDL_GetKeyState(NULL);
                    if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) {
                        p1->acceleration = sprint_accel;
                    } else {
                        p1->acceleration = accel;
                    }
                    p1->direction = 1;
                }
                break;
            case SDLK_LEFT: 
                if (active_player == 1) {
                    
                    Uint8 *keystate = SDL_GetKeyState(NULL);
                    if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) {
                        p1->acceleration = -sprint_accel;
                    } else {
                        p1->acceleration = -accel;
                    }
                    p1->direction = -1;
                }
                break;
            case SDLK_UP:
                if (active_player == 1 && p1->status == STAT_SOL) {
                    p1->vy = jump_speed;
                    p1->status = STAT_AIR;
                    p1->up = 1;
                    p1->jump_frame = 0; 
                }
                break;
            case SDLK_d: 
                if (active_player == 2) {
                    
                    Uint8 *keystate = SDL_GetKeyState(NULL);
                    if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) {
                        p2->acceleration = sprint_accel;
                    } else {
                        p2->acceleration = accel;
                    }
                    p2->direction = 1;
                }
                break;
            case SDLK_a: 
                if (active_player == 2) {
                    
                    Uint8 *keystate = SDL_GetKeyState(NULL);
                    if (keystate[SDLK_LSHIFT] || keystate[SDLK_RSHIFT]) {
                        p2->acceleration = -sprint_accel;
                    } else {
                        p2->acceleration = -accel;
                    }
                    p2->direction = -1;
                }
                break;
            case SDLK_w:
                if (active_player == 2 && p2->status == STAT_SOL) {
                    p2->vy = jump_speed;
                    p2->status = STAT_AIR;
                    p2->up = 1;
                    p2->jump_frame = 0; 
                }
                break;
            case SDLK_g:
                
                if (active_player == 1) {
                    p1->show_guide = !p1->show_guide;
                } else if (active_player == 2) {
                    p2->show_guide = !p2->show_guide;
                }
                break;
            case SDLK_ESCAPE: 
                *running = 0; 
                break;
        }
    }
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
            case SDLK_LEFT: 
                if (active_player == 1) {
                    p1->acceleration = 0.0; 
                    p1->vitesse = 0.0;
                    p1->direction = 0;
                }
                break;
            case SDLK_d:
            case SDLK_a: 
                if (active_player == 2) {
                    p2->acceleration = 0.0; 
                    p2->vitesse = 0.0;
                    p2->direction = 0;
                }
                break;
        }
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        if (active_player == 1 && p1->status == STAT_SOL) {
            p1->status = STAT_ATTACK;
            p1->attack_frame = 0;
        } else if (active_player == 2 && p2->status == STAT_SOL) {
            p2->status = STAT_ATTACK;
            p2->attack_frame = 0;
        }
    }
}
