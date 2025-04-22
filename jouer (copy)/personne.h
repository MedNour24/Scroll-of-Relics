#ifndef PERSONNE_H
#define PERSONNE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define STAT_SOL 0
#define STAT_AIR 1
#define STAT_ATTACK 2

typedef struct {
    SDL_Rect position;              
    int frame;                      
    double vitesse;             
    double acceleration;            
    int up;                         
    double vy;                      
    int status;                     
    int vie;                        
    int score;                      
    int direction;                  
    int attack_frame;              
    int jump_frame;                 
    SDL_Surface *tab_right[8];      
    SDL_Surface *tab_left[8];       
    SDL_Surface *tab_attack_right[5]; 
    SDL_Surface *tab_attack_left[5];  
    SDL_Surface *tab_jump_right[1];   
    SDL_Surface *tab_jump_left[1];    
    int show_guide;                 
} personnage;

void initialiser_personnage(personnage *p);
void afficher_personnage(personnage p, SDL_Surface *ecran);
void animate_personnage(personnage *p);
void movePerso(personnage *p, Uint32 dt);
void handle_input(SDL_Event event, personnage *p1, personnage *p2, int *running, int active_player);

#endif
